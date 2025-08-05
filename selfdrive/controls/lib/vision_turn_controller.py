# PFEIFER - VTSC

# Acknowledgements:
# Past versions of this code were based on the move-fast teams vtsc
# implementation. (https://github.com/move-fast/openpilot) Huge thanks to them
# for their initial implementation. I also used sunnypilot as a reference.
# (https://github.com/sunnyhaibin/sunnypilot) Big thanks for sunny's amazing work

import numpy as np
import time
import math
from cereal import custom
from openpilot.common.conversions import Conversions as CV
from openpilot.common.params import Params
from openpilot.selfdrive.controls.lib.sunnypilot.helpers import debug
from openpilot.common.logger import logger

TARGET_LAT_A = 1.9  # m/s^2
MIN_TARGET_V = 5  # m/s 18km/h
MIN_STEER_TARGET_V = 4  # m/s 14.4km/h

PARAMS_UPDATE_PERIOD = 5.

VisionTurnControllerState = custom.LongitudinalPlanSP.VisionTurnControllerState


def safe_int(x, default=-1):
  try:
    return int(x)
  except (ValueError, TypeError):
    logger.log("[ERROR] Convert to int failed")
    print("Convert to int failed")
    return default

def _description_for_state(turn_controller_state):
  if turn_controller_state == VisionTurnControllerState.disabled:
    return 'DISABLED'
  if turn_controller_state == VisionTurnControllerState.entering:
    return 'ENTERING'
  if turn_controller_state == VisionTurnControllerState.turning:
    return 'TURNING'
  if turn_controller_state == VisionTurnControllerState.leaving:
    return 'LEAVING'


class VisionTurnController:
  def __init__(self, CP):
    self._params = Params()
    self._CP = CP
    self._op_enabled = False
    self._gas_pressed = False
    self._is_enabled = self._params.get_bool("TurnVisionControl")
    self._is_turn_vision_cruise = self._params.get_bool("TurnVisionCruise")
    self._is_steer_cruise_tune = self._params.get_bool("SteerCruiseTune")
    #new
    try:
      val = Params().get("StartTurnLatA")
      self.start_turn_lat_accel = float(val)/10 if val is not None and val != b'' else 1.5
      val = Params().get("TargetTurnLatA")
      self.target_turn_lat_accel = float(val)/10 if val is not None and val != b'' else 1.9
      val = Params().get("TurnSteepNess")
      self.turn_steep_ness = float(val)/10 if val is not None and val != b'' else 8.0
      val = Params().get("TurnLatAccel")
      self.turn_lat_acc = float(val) / 10 if val is not None and val != b'' else 1.3
      val = Params().get("EndTurnLatAccel")
      self.end_turn_lat_accel = float(val) / 10 if val is not None and val != b'' else 1.7
      val = Params().get("TurnMaxFactor")
      self.turn_max_factor = float(val) / 10 if val is not None and val != b'' else 0.6
      val = Params().get("SteerTurnThr")
      self.steer_turn_thr = float(val) / 100 if val is not None and val != b'' else 0.7
      val = Params().get("SteerMaxFactor")
      self.steer_max_factory = float(val) / 100 if val is not None and val != b'' else 0.3
    except AttributeError:
      self.start_turn_lat_accel = 1.5
      self.target_turn_lat_accel = 1.9
      self.turn_steep_ness = 8.0
      self.turn_lat_acc = 1.3
      self.end_turn_lat_accel = 1.7
      self.turn_max_factor = 0.6
      self.steer_turn_thr = 0.7
      self.steer_max_factory = 0.3
    #new
    self._last_params_update = 0.
    self._v_ego = 0.
    self._v_target = MIN_TARGET_V
    #new
    self._v_target_tmp = MIN_TARGET_V
    self._soft_v_target = 255.
    self._soft_v_target_kmh = 255.
    self._soft_v_target_kmh_tmp = 255.
    self._soft_v_target_filtered_kmh = 255.
    self.margin_factor = 0.0
    self.frame = 0
    #new
    self._current_lat_acc = 0.
    self._max_pred_lat_acc = 0.
    self._v_cruise = 0.
    self._state = VisionTurnControllerState.disabled

    self._reset()

  @property
  def state(self):
    return self._state

  @state.setter
  def state(self, value):
    if value != self._state:
      debug(f"V-TSC: VisionTurnControllerState state: {_description_for_state(value)}")
      if value == VisionTurnControllerState.disabled:
        self._reset()
    self._state = value

  @property
  def v_target(self):
    return min(self._v_target, self._soft_v_target)
    #return self._v_target

  #new
  @property
  def soft_v_target(self):
    return min(self._v_target, self._soft_v_target)
    #return self._soft_v_target
  #new

  @property
  def is_active(self):
    return self._state != VisionTurnControllerState.disabled and self._is_enabled

  @property
  def current_lat_acc(self):
    return self._current_lat_acc

  @property
  def max_pred_lat_acc(self):
    return self._max_pred_lat_acc

  def _reset(self):
    self._current_lat_acc = 0.
    self._max_pred_lat_acc = 0.
    #new
    self._soft_v_target_filtered_kmh = 255.
    #new

  def _update_params(self):
    t = time.monotonic()
    if t > self._last_params_update + PARAMS_UPDATE_PERIOD:
      self._is_enabled = self._params.get_bool("TurnVisionControl")
      self._is_turn_vision_cruise = self._params.get_bool("TurnVisionCruise")
      self._is_steer_cruise_tune = self._params.get_bool("SteerCruiseTune")
      try:
        val = Params().get("StartTurnLatA")
        self.start_turn_lat_accel = float(val) / 10 if val is not None and val != b'' else 1.5
        val = Params().get("TargetTurnLatA")
        self.target_turn_lat_accel = float(val) / 10 if val is not None and val != b'' else 1.9
        val = Params().get("TurnSteepNess")
        self.turn_steep_ness = float(val) / 10 if val is not None and val != b'' else 8.0
        val = Params().get("TurnLatAccel")
        self.turn_lat_acc = float(val) / 10 if val is not None and val != b'' else 1.3
        val = Params().get("EndTurnLatAccel")
        self.end_turn_lat_accel = float(val) / 10 if val is not None and val != b'' else 1.7
        val = Params().get("TurnMaxFactor")
        self.turn_max_factor = float(val) / 10 if val is not None and val != b'' else 0.6
        val = Params().get("SteerTurnThr")
        self.steer_turn_thr = float(val) / 100 if val is not None and val != b'' else 0.7
        val = Params().get("SteerMaxFactor")
        self.steer_max_factory = float(val) / 100 if val is not None and val != b'' else 0.3
      except AttributeError:
        self.start_turn_lat_accel = 1.5
        self.target_turn_lat_accel = 1.9
        self.turn_steep_ness = 8.0
        self.turn_lat_acc = 1.3
        self.end_turn_lat_accel = 1.7
        self.turn_max_factor = 0.6
        self.steer_turn_thr = 0.7
        self.steer_max_factory = 0.3
      # new
      self._last_params_update = t

  def calculate_margin_factor(self, max_pred_lat_acc, turn_lat_acc):
    #self.turn_steep_ness = 9
    #self.turn_lat_acc = 1.0
    #self.turn_max_factor = 0.6
    # 使用 Sigmoid 函数平滑调整 margin_factor
    # 参数决定了平滑的区间和范围
    steepness = self.turn_steep_ness  #9  # 决定平滑的陡峭程度，较大的值让变化更快
    #shift = self.turn_lat_acc         #1.0  # 控制从哪个横向加速度值开始快速变化
    shift = turn_lat_acc  # 1.0  # 控制从哪个横向加速度值开始快速变化
    max_value = self.turn_max_factor  #0.6  # 最终的最大值

    # 安全防护：判空/判NaN/范围限制
    if max_pred_lat_acc is None or math.isnan(max_pred_lat_acc):
      logger.log("[WARN] max_pred_lat_acc is None or NaN")
      print("[WARN] max_pred_lat_acc is None or NaN")
      return 0.0

    # 限制最大/最小值，防止 exp 溢出
    delta = max_pred_lat_acc - shift
    delta = max(min(delta, 100.0), -100.0)  # 防止 overflow

    try:
      margin_factor = max_value / (1 + math.exp(-steepness * delta))
    except OverflowError as e:
      logger.log("[WARN] Overflow in margin_factor calc")
      print(f"[WARN] Overflow in margin_factor calc: {e}, acc={max_pred_lat_acc}")
      margin_factor = 0.0

    return margin_factor

  def get_steering_saturation_factor(self, steer):
    # 防御式编程：避免 None 或 NaN 崩溃
    if steer is None:
      logger.log("[WARN] steer is None")
      print("[WARN] steer is None")
      return 0.0

    if isinstance(steer, float) and math.isnan(steer):
      logger.log("[WARN] steer is NaN")
      print("[WARN] steer is NaN")
      return 0.0

    try:
      saturation = abs(steer)
    except Exception as e:
      logger.log("[WARN] abs(steer) failed")
      print(f"[WARN] abs(steer) failed: {e}")
      return 0.0

    if saturation <= self.steer_turn_thr:
      return 0.0

    factor = saturation - self.steer_turn_thr
    if factor < 0.0:
      factor = 0.0
    factor_div = 1.01 - self.steer_turn_thr
    if factor_div <= 0:
      factor_div = 0.01
    factor_ret = factor / factor_div * self.steer_max_factory
    return min(self.steer_max_factory, factor_ret)

  def _update_calculations(self, sm):
    #读取 modelV2 预测的横摆角速度（yaw rate） 和预测速度（x方向），rate_plan和vel_plan均为33个元素的数组
    rate_plan = np.array(np.abs(sm['modelV2'].orientationRate.z))
    vel_plan = np.array(sm['modelV2'].velocity.x)

    #计算当前车辆横向加速度（使用方向盘角度计算曲率）
    try:
      sr = self._CP.steerRatio
      wb = self._CP.wheelbase
      if sr is None or wb is None or sr == 0 or wb == 0:
        print(f"[WARN] Invalid steerRatio ({sr}) or wheelbase ({wb})")
        current_curvature = 0.0  # 或者用一个默认值，比如 0.001
      else:
        current_curvature = abs(sm['carState'].steeringAngleDeg * CV.DEG_TO_RAD / (sr * wb))
    except Exception as e:
      print(f"[CRASH-GUARD] curvature calc failed: {e}")
      current_curvature = 0.0

    self._current_lat_acc = current_curvature * self._v_ego**2

    #根据 model 预测的 yaw rate * speed 推出预测横向加速度
    # get the maximum lat accel from the model
    predicted_lat_accels = rate_plan * vel_plan
    self._max_pred_lat_acc = np.amax(predicted_lat_accels) #使用amax函数找出数组中最大的值

    # === 改写：经典限速逻辑，基于弯道“进入程度”动态调整目标横向加速度 ===
    # 计算当前速度
    v_ego = max(self._v_ego, 0.1)

    # 分段横向加速度数组（预测轨迹）
    n_total = len(predicted_lat_accels)
    n_section = n_total // 3

    # 最近段（车辆即将进入）和最远段（远处预判）
    recent_third = predicted_lat_accels[2 * n_section:]
    far_third = predicted_lat_accels[:n_section]

    # 横向加速度阈值判断
    threshold = 0.8

    # 是否最近段已经明显进入弯道
    recent_score = np.count_nonzero(recent_third > threshold) / len(recent_third)
    far_score = np.count_nonzero(far_third > threshold) / len(far_third)

    # === 根据“进入弯道程度”动态调整目标横向加速度 ===

    # 插值因子，表示车辆“已进入弯道的程度”
    entering_score = recent_score  # 越大代表越深入弯道
    entering_score = np.clip(entering_score, 0.0, 1.0)

    # 从提前减速的 1.0 动态过渡到 self.target_turn_lat_accel（如 1.9）
    # 限制self.target_turn_lat_accel不能小于self.start_turn_lat_accel
    _target_turn_lat_accel = max(self.start_turn_lat_accel, self.target_turn_lat_accel)
    # 限制_target_turn_lat_accel不小于1.0
    _target_turn_lat_accel = max(1.0, _target_turn_lat_accel)
    #限制self.start_turn_lat_accel不能大于_target_turn_lat_accel
    _start_turn_lat_accel = min(self.start_turn_lat_accel, _target_turn_lat_accel)
    target_turn_lat_accel_used = _start_turn_lat_accel + (_target_turn_lat_accel - _start_turn_lat_accel) * entering_score
    target_turn_lat_accel_used = np.clip(target_turn_lat_accel_used, _start_turn_lat_accel, _target_turn_lat_accel)

    # 使用最大预测横向加速度（全局）计算安全曲率（确保足够提前反应）
    max_curve = self._max_pred_lat_acc / (v_ego ** 2)

    # 计算经典目标速度
    self._v_target_tmp = (target_turn_lat_accel_used / max_curve) ** 0.5
    self._v_target_tmp = max(self._v_target_tmp, MIN_TARGET_V)
    self._v_target_tmp = min(self._v_target_tmp, self._v_cruise + CV.KPH_TO_MS)
    # === 改动结束 ===

    # === 修改点2：动态调整 soft 限速目标横向加速度 ===
    # 限制self.end_turn_lat_accel.turn_lat_acc
    _target_turn_lat_accel_soft = max(self.turn_lat_acc, self.end_turn_lat_accel)
    # 限制_target_turn_lat_accel_soft不小于1.0
    _target_turn_lat_accel_soft = max(1.0, _target_turn_lat_accel_soft)
    # 限制self.turn_lat_acc不能大于_target_turn_lat_accel_soft
    _start_turn_lat_accel_soft = min(self.turn_lat_acc, _target_turn_lat_accel_soft)
    turn_lat_acc_soft_used = _start_turn_lat_accel_soft + (_target_turn_lat_accel_soft - _start_turn_lat_accel_soft) * entering_score
    turn_lat_acc_soft_used = np.clip(turn_lat_acc_soft_used, _start_turn_lat_accel_soft, _target_turn_lat_accel_soft)

    # == 智能软限速逻辑 ==
    v_cruise_kmh = self._v_cruise * CV.MS_TO_KPH
    # 计算 margin_factor
    #self.margin_factor = self.calculate_margin_factor(self._max_pred_lat_acc)
    self.margin_factor = self.calculate_margin_factor(self._max_pred_lat_acc, turn_lat_acc_soft_used)

    # 限速区间逻辑
    LIMIT_TABLE = [
      (90, 60), (80, 50), (70, 45), (60, 40),
      (50, 35), (40, 30), (30, 25), (0, 20),
    ]
    target_v_kmh = v_cruise_kmh
    for v_threshold, v_min in LIMIT_TABLE:
      if v_cruise_kmh > v_threshold:
        target_v_kmh = max(v_min, min(v_cruise_kmh * (1 - self.margin_factor), v_cruise_kmh))
        break

    #限制计算的速度不超过设定的巡航速度
    target_v_kmh = min(target_v_kmh, v_cruise_kmh + 1.)

    self._soft_v_target_kmh = target_v_kmh

    # == 快速平滑滤波器 ==
    alpha = 0.4 #alpha = 0.4：平滑速度，可调为 0.3～0.5，响应越快就越接近目标速度
    self._soft_v_target_filtered_kmh = alpha * self._soft_v_target_kmh + (1 - alpha) * self._soft_v_target_filtered_kmh
    self._soft_v_target_kmh_tmp = self._soft_v_target_filtered_kmh

    #new 扭矩权重用于降速
    steer_limit = False
    steer = 0.
    steer_saturation_enable = True
    if steer_saturation_enable and sm.valid['carOutput']:
      try:
        raw_steer = sm['carOutput'].actuatorsOutput.steer
        # 判空、判 NaN
        if raw_steer is not None and not math.isnan(raw_steer):
          steer = abs(raw_steer)
          saturation_factor = self.get_steering_saturation_factor(steer)
          saturation_factor = max(0.0, min(saturation_factor, 1.0)) #限制最大为1.0，防止下面的计算成为负数
          if self._is_steer_cruise_tune: #扭矩用于控制巡航速度
            self._soft_v_target_kmh_tmp *= (1.0 - saturation_factor)
            self._v_target_tmp *= (1.0 - saturation_factor)
            if saturation_factor > 0.05:
              steer_limit = True
        else:
          logger.log("[WARN] steer value invalid")
          print(f"[WARN] steer value invalid: {raw_steer}")
      except Exception as e:
        logger.log("[WARN] Exception accessing carOutput steer")
        print(f"[WARN] Exception accessing carOutput steer: {e}")

    #经典算法的目标速度
    if not steer_limit:
      self._v_target_tmp = max(self._v_target_tmp, MIN_TARGET_V)
    else:
      self._v_target_tmp = max(self._v_target_tmp, MIN_STEER_TARGET_V)
    self._v_target = self._v_target_tmp
    v_target_kmh = self._v_target * CV.MS_TO_KPH

    #智能软限速逻辑的目标速度
    _soft_v_target = self._soft_v_target_kmh_tmp * CV.KPH_TO_MS #km/h速度换算成m/s
    if not steer_limit:
      _soft_v_target = max(_soft_v_target, MIN_TARGET_V)
    else:
      _soft_v_target = max(_soft_v_target, MIN_STEER_TARGET_V)
    soft_v_target_kmh = _soft_v_target * CV.MS_TO_KPH

    if self._is_turn_vision_cruise: #模型预测轨迹控制巡航速度
      self._soft_v_target = _soft_v_target
    else:
      self._soft_v_target = 255.

    #调试信息打印
    #if self.frame % 4 == 0:
    #  try:
    #    print(f"[VTC] lat_acc: {self._max_pred_lat_acc:5.1f} m/s^2 | steer: {steer:3.1f} | v_cruise: {safe_int(v_cruise_kmh):3d} km/h | v_soft: {safe_int(soft_v_target_kmh):2d} km/h | vtc: {safe_int(v_target_kmh):2d} km/h")
    #  except Exception as e:
    #    logger.log("[WARN] Exception in print VTC information")
    #    print(f"[WARN] Exception in print VTC information: {e}")

  def _state_transition(self):
    if not self._op_enabled or not self._is_enabled or self._gas_pressed or self._v_ego < MIN_TARGET_V:
      self.state = VisionTurnControllerState.disabled
      return

    # DISABLED
    if self.state == VisionTurnControllerState.disabled:
      #if self._v_cruise > self._v_target or self.margin_factor > 0.08:
      if self._v_cruise > self._v_target or (self._v_cruise > self._soft_v_target and self.margin_factor > 0.03):
        self.state = VisionTurnControllerState.turning
    # TURNING
    elif self.state == VisionTurnControllerState.turning:
      #if not (self._v_cruise > self._v_target) and self.margin_factor < 0.05:
      if self._v_target >= self._v_cruise and (self._soft_v_target >= self._v_cruise or self.margin_factor < 0.01):
        self.state = VisionTurnControllerState.disabled

  def update(self, enabled, v_ego, v_cruise, sm):
    self._op_enabled = enabled
    self._gas_pressed = sm['carState'].gasPressed
    self._v_ego = v_ego
    self._v_cruise = v_cruise

    self._update_params()
    self._update_calculations(sm)
    self._state_transition()

    self.frame += 1
