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

TARGET_LAT_A = 1.5  # m/s^2
MIN_TARGET_V = 5  # m/s

PARAMS_UPDATE_PERIOD = 5.

VisionTurnControllerState = custom.LongitudinalPlanSP.VisionTurnControllerState


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
    self._last_params_update = 0.
    self._v_ego = 0.
    self._v_target = MIN_TARGET_V
    #new
    self._soft_v_target = 255.
    self._soft_v_target_kmh = 255.
    self._soft_v_target_filtered_kmh = 255.
    self.margin_factor = 0.0
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
      self._last_params_update = t

  def calculate_margin_factor(self, max_pred_lat_acc):
    # 使用 Sigmoid 函数平滑调整 margin_factor
    # 参数决定了平滑的区间和范围
    steepness = 10  # 决定平滑的陡峭程度，较大的值让变化更快
    shift = 0.3  # 控制从哪个横向加速度值开始快速变化
    max_value = 0.75  # 最终的最大值

    margin_factor = max_value / (1 + math.exp(-steepness * (max_pred_lat_acc - shift)))
    return margin_factor

  def _update_calculations(self, sm):
    #读取 modelV2 预测的横摆角速度（yaw rate） 和预测速度（x方向），rate_plan和vel_plan均为33个元素的数组
    rate_plan = np.array(np.abs(sm['modelV2'].orientationRate.z))
    vel_plan = np.array(sm['modelV2'].velocity.x)

    #计算当前车辆横向加速度（使用方向盘角度计算曲率）
    current_curvature = abs(
      sm['carState'].steeringAngleDeg * CV.DEG_TO_RAD / (self._CP.steerRatio * self._CP.wheelbase))
    self._current_lat_acc = current_curvature * self._v_ego**2

    #根据 model 预测的 yaw rate * speed 推出预测横向加速度
    # get the maximum lat accel from the model
    predicted_lat_accels = rate_plan * vel_plan
    self._max_pred_lat_acc = np.amax(predicted_lat_accels) #使用amax函数找出数组中最大的值

    # 经典目标速度计算（保留）
    #计算出最小允许的转弯半径曲率，并反推出一个“能在目标横向加速度下通过这个弯道的安全速度”
    # get the maximum curve based on the current velocity
    v_ego = max(self._v_ego, 0.1)  # ensure a value greater than 0 for calculations
    max_curve = self.max_pred_lat_acc / (v_ego**2)

    # “如果最大模型预测横向加速度是 3m/s²，我们希望限制它到 1.9m/s²，那就得减速到 sqrt(1.9/3) 倍当前速度”。
    # Get the target velocity for the maximum curve
    self._v_target = (TARGET_LAT_A / max_curve) ** 0.5
    self._v_target = max(self._v_target, MIN_TARGET_V)

    # == 智能软限速逻辑 ==
    v_cruise_kmh = self._v_cruise * CV.MS_TO_KPH
    # margin_factor 的横向加速度阈值和比例也可根据测试动态调节
    #self.margin_factor = 0.0
    #if self._max_pred_lat_acc > 0.2:
    #  self.margin_factor = 0.25
    #if self._max_pred_lat_acc > 0.4:
    #  self.margin_factor = 0.5
    #if self._max_pred_lat_acc > 0.6:
    #  self.margin_factor = 0.75

    # 计算 margin_factor
    self.margin_factor = self.calculate_margin_factor(self._max_pred_lat_acc)

    # 限速区间逻辑
    if v_cruise_kmh > 90:
      target_v_kmh = max(60, min(v_cruise_kmh * (1 - self.margin_factor)))
    elif v_cruise_kmh > 80:
      target_v_kmh = max(50, min(v_cruise_kmh * (1 - self.margin_factor)))
    elif v_cruise_kmh > 70:
      target_v_kmh = max(40, min(v_cruise_kmh * (1 - self.margin_factor)))
    elif v_cruise_kmh > 60:
      target_v_kmh = max(35, min(v_cruise_kmh * (1 - self.margin_factor)))
    elif v_cruise_kmh > 50:
      target_v_kmh = max(30, min(v_cruise_kmh * (1 - self.margin_factor)))
    else:
      target_v_kmh = max(20, v_cruise_kmh * (1 - self.margin_factor))

    self._soft_v_target_kmh = target_v_kmh

    # == 快速平滑滤波器 ==
    alpha = 0.4 #alpha = 0.4：平滑速度，可调为 0.3～0.5，响应越快就越接近目标速度
    self._soft_v_target_filtered_kmh = alpha * self._soft_v_target_kmh + (1 - alpha) * self._soft_v_target_filtered_kmh

    #km/h速度换算成m/s
    self._soft_v_target = self._soft_v_target_filtered_kmh * KPH_TO_MS

    v_target_kmh = self._soft_v_target * CV.MS_TO_KPH
    print(f"[VTC] lat_acc: {self._max_pred_lat_acc:5.1f} m/s^2 | v_cruise: {int(v_cruise_kmh):3d} km/h | v_soft: {int(self._soft_v_target_filtered_kmh):2d} km/h | vtc: {int(v_target_kmh):2d} km/h")

    #从经典算法的目标速度和智能软限速逻辑逻辑中取最小速度
    #self._v_target = min(self._v_target, self._soft_v_target)

  def _state_transition(self):
    if not self._op_enabled or not self._is_enabled or self._gas_pressed or self._v_ego < MIN_TARGET_V:
      self.state = VisionTurnControllerState.disabled
      return

    # DISABLED
    if self.state == VisionTurnControllerState.disabled:
      if self._v_cruise > self._v_target or self.margin_factor > 0.08:
        self.state = VisionTurnControllerState.turning
    # TURNING
    elif self.state == VisionTurnControllerState.turning:
      if not (self._v_cruise > self._v_target) and self.margin_factor < 0.05:
        self.state = VisionTurnControllerState.disabled

  def update(self, enabled, v_ego, v_cruise, sm):
    self._op_enabled = enabled
    self._gas_pressed = sm['carState'].gasPressed
    self._v_ego = v_ego
    self._v_cruise = v_cruise

    self._update_params()
    self._update_calculations(sm)
    self._state_transition()
