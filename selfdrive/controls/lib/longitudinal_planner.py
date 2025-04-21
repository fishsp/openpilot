#!/usr/bin/env python3
import math
import numpy as np
from openpilot.common.numpy_fast import clip, interp
from openpilot.common.params import Params
from cereal import car

import cereal.messaging as messaging
from openpilot.common.conversions import Conversions as CV
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.selfdrive.controls.lib.sunnypilot.common import Source
from openpilot.selfdrive.controls.lib.sunnypilot.speed_limit_controller import SpeedLimitController
from openpilot.selfdrive.car.interfaces import ACCEL_MIN, ACCEL_MAX
from openpilot.selfdrive.controls.lib.longcontrol import LongCtrlState
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import LongitudinalMpc, N
from openpilot.selfdrive.controls.lib.longitudinal_mpc_lib.long_mpc import T_IDXS as T_IDXS_MPC
from openpilot.selfdrive.controls.lib.drive_helpers import V_CRUISE_MAX, CONTROL_N, get_speed_error
from openpilot.selfdrive.controls.lib.vision_turn_controller import VisionTurnController
from openpilot.selfdrive.controls.lib.turn_speed_controller import TurnSpeedController
from openpilot.selfdrive.controls.lib.dynamic_experimental_controller import DynamicExperimentalController
from openpilot.selfdrive.controls.lib.events import Events
from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.controls.lib.drive_helpers import V_CRUISE_MAX, V_CRUISE_UNSET
from openpilot.selfdrive.fishsp.traffic_light import CarrotPlanner, XState

LON_MPC_STEP = 0.2  # first step is 0.2s
A_CRUISE_MIN = -2.5 #-1.2
#A_CRUISE_MAX_VALS = [1.6, 1.2, 0.8, 0.6]
#A_CRUISE_MAX_BP =   [0., 10.0, 25., 40.]
#            km/h    0    36    90   144

#A_CRUISE_MAX_VALS = [1.4, 1.4,  1.4,  1.2,  1.0,   0.9,   0.8,   0.7,   0.6,  .53,  .42,  .13]
#A_CRUISE_ECO_VALS = [1.2, 1.2,  1.2,  1.1,  1.0,   0.85,  0.7,   0.65,  0.5,  .43,  .32,  .088]
A_CRUISE_MAX_VALS = [1.6, 1.6,  1.6,  1.4,  1.0,   0.9,   0.8,   0.7,   0.6,  .53,  .42,  .13]
A_CRUISE_ECO_VALS = [1.4, 1.4,  1.4,  1.2,  1.0,   0.9,   0.8,   0.7,   0.6,  .53,  .42,  .13]
A_JERK_MAX_VALS =   [0.4, 0.6,  0.6,  0.8,  0.8,   0.8,   0.7,   0.6,   0.5,   0.4,  0.2,   0.1] # 仅在启动巡航时平滑用
A_JERK_ECO_VALS =   [0.3, 0.5,  0.5,  0.5,  0.4,   0.4,   0.3,   0.2,   0.2,   0.2,  0.2,   0.1]
A_CRUISE_MAX_BP =   [0.,  2.78, 5.56, 8.33, 11.11, 13.33, 15.55, 17.78, 22.22, 25.,  27.8,  55.6]
#            km/h    0    10    20    30    40     50     60     70     80     90    100    200

# Lookup table for turns
_A_TOTAL_MAX_V = [1.7, 3.2]
_A_TOTAL_MAX_BP = [20., 40.]


EventName = car.CarEvent.EventName


def get_max_accel(v_ego, eco):
  if eco:
    return interp(v_ego, A_CRUISE_MAX_BP, A_CRUISE_ECO_VALS)
  else:
    return interp(v_ego, A_CRUISE_MAX_BP, A_CRUISE_MAX_VALS)

def get_max_jerk(v_ego, eco):
  if eco:
    return interp(v_ego, A_CRUISE_MAX_BP, A_JERK_ECO_VALS)
  else:
    return interp(v_ego, A_CRUISE_MAX_BP, A_JERK_MAX_VALS)

def limit_accel_in_turns(v_ego, angle_steers, a_target, CP):
  """
  This function returns a limited long acceleration allowed, depending on the existing lateral acceleration
  this should avoid accelerating when losing the target in turns
  """

  # FIXME: This function to calculate lateral accel is incorrect and should use the VehicleModel
  # The lookup table for turns should also be updated if we do this
  a_total_max = interp(v_ego, _A_TOTAL_MAX_BP, _A_TOTAL_MAX_V)
  a_y = v_ego ** 2 * angle_steers * CV.DEG_TO_RAD / (CP.steerRatio * CP.wheelbase)
  a_x_allowed = math.sqrt(max(a_total_max ** 2 - a_y ** 2, 0.))

  return [a_target[0], min(a_target[1], a_x_allowed)]


class LongitudinalPlanner:
  def __init__(self, CP, init_v=0.0, init_a=0.0, dt=DT_MDL):
    self.CP = CP
    self.mpc = LongitudinalMpc(dt=dt)
    self.fcw = False
    self.dt = dt

    self.a_desired = init_a
    self.v_desired_filter = FirstOrderFilter(init_v, 2.0, self.dt)
    self.v_model_error = 0.0

    self.v_desired_trajectory = np.zeros(CONTROL_N)
    self.a_desired_trajectory = np.zeros(CONTROL_N)
    self.j_desired_trajectory = np.zeros(CONTROL_N)
    self.solverExecutionTime = 0.0
    self.params = Params()
    self.param_read_counter = 0
    self.read_param()

    self.cruise_source = 'cruise'
    self.vision_turn_controller = VisionTurnController(CP)
    self.speed_limit_controller = SpeedLimitController(CP)
    self.events = Events()
    self.turn_speed_controller = TurnSpeedController()
    self.dynamic_experimental_controller = DynamicExperimentalController()
    self.eco = self.params.get_bool("SubaruManualParkingBrakeSng") #斯巴鲁驻车作为eco开关
    self.stock_long_toyota = self.params.get_bool("StockLongToyota")
    self.vCluRatio = 1.0
    self.v_cruise_kph = 0.0
    self.disable_carrot = False
    self.frame = 0
    self.is_turning = False
    self.turn_score = 0.0
    self.turn_enable = False

  def read_param(self):
    self.eco = self.params.get_bool("SubaruManualParkingBrakeSng")
    self.stock_long_toyota = self.params.get_bool("StockLongToyota")
    try:
      self.dynamic_experimental_controller.set_enabled(self.params.get_bool("DynamicExperimentalControl"))
    except AttributeError:
      self.dynamic_experimental_controller = DynamicExperimentalController()

  @staticmethod
  def parse_model(model_msg, model_error):
    if (len(model_msg.position.x) == ModelConstants.IDX_N and
       len(model_msg.velocity.x) == ModelConstants.IDX_N and
       len(model_msg.acceleration.x) == ModelConstants.IDX_N):
      x = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model_msg.position.x) - model_error * T_IDXS_MPC
      v = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model_msg.velocity.x) - model_error
      a = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model_msg.acceleration.x)
      j = np.zeros(len(T_IDXS_MPC))
    else:
      x = np.zeros(len(T_IDXS_MPC))
      v = np.zeros(len(T_IDXS_MPC))
      a = np.zeros(len(T_IDXS_MPC))
      j = np.zeros(len(T_IDXS_MPC))
    return x, v, a, j

  @staticmethod
  def compute_turn_score(model, heading_thresh_rad=0.1, lateral_offset_thresh=1.5):
    if (len(model.position.x) == ModelConstants.IDX_N and
       len(model.position.y) == ModelConstants.IDX_N):
      x = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model.position.x)
      y = np.interp(T_IDXS_MPC, ModelConstants.T_IDXS, model.position.y)
    else:
      x = np.zeros(len(T_IDXS_MPC))
      y = np.zeros(len(T_IDXS_MPC))
      return {
        'score': 0,
        'is_turning': False,
        'max_heading_change': 0,
        'max_lateral_offset': 0,
      }

    if len(x) != len(y):
      return {
        'score': 0,
        'is_turning': False,
        'max_heading_change': 0,
        'max_lateral_offset': 0,
      }

    dt = DT_MDL
    max_time = len(x) * dt
    desired_time_steps = [0.5, 1.0, 1.5, 1.65]
    time_steps = [t for t in desired_time_steps if t < max_time]

    turn_score = 0.0
    max_offset = 0.0
    max_heading = 0.0
    is_turning = False

    for t in time_steps:
      idx = int(t / dt)
      dx = x[idx] - x[0]
      dy = y[idx] - y[0]
      heading_change = np.arctan2(dy, dx)
      lateral_offset = abs(y[idx])

      max_offset = max(max_offset, lateral_offset)
      max_heading = max(max_heading, abs(heading_change))

      if abs(heading_change) > heading_thresh_rad or lateral_offset > lateral_offset_thresh:
        score = min(1.0, (abs(heading_change) / heading_thresh_rad + lateral_offset / lateral_offset_thresh) * 0.5)
        turn_score = max(turn_score, score)
        is_turning = True

    return {
      'score': np.clip(turn_score, 0.0, 1.0),
      'is_turning': is_turning,
      'max_heading_change': max_heading,
      'max_lateral_offset': max_offset,
    }

  def update(self, sm, carrot):
    if self.param_read_counter % 50 == 0:
      self.read_param()
    self.param_read_counter += 1
    if self.dynamic_experimental_controller.is_enabled() and sm['controlsState'].experimentalMode:
      self.mpc.mode = self.dynamic_experimental_controller.get_mpc_mode(self.CP.radarUnavailable, sm['carState'], sm['radarState'].leadOne, sm['modelV2'], sm['controlsState'], sm['navInstruction'].maneuverDistance)
    else:
      self.mpc.mode = 'blended' if sm['controlsState'].experimentalMode else 'acc'

    v_ego = sm['carState'].vEgo
    v_cruise_kph = min(sm['controlsState'].vCruise, V_CRUISE_MAX)
    v_cruise = v_cruise_kph * CV.KPH_TO_MS
    v_ego_kph = v_ego * CV.MS_TO_KPH

    self.disable_carrot = False
    car_state = sm['carState']
    if not car_state.cruiseState.enabled:
      self.disable_carrot = True

    # === 新增：判断是否即将转弯 ===
    self.turn_enable = True
    if self.turn_enable:
      model = sm['modelV2']
      turn_info = self.compute_turn_score(model)
      self.is_turning = turn_info['is_turning']
      self.turn_score = turn_info['score']

      if self.is_turning:
        #print(f"[Turn] score: {self.turn_score:.2f}, heading_change: {np.degrees(turn_info['max_heading_change']):.1f}°, lateral_offset: {turn_info['max_lateral_offset']:.2f}m")

        # 转弯评分大为0.4时，切换为 blended 模式
        if self.turn_score > 0.4:
          self.disable_carrot = True  # 转弯时关闭carrot功能，切换到sp的DEC自动模式
    # === 新增：判断是否即将转弯 ===

    #carrot
    calib_v_cruise = True
    if not self.disable_carrot: #没有禁用carrot时
      if not carrot.blended_request: #无blended请求时
        v_cruise = carrot.update(sm, v_cruise_kph, False)
        calib_v_cruise = False
        if not carrot.blended_request:
          self.mpc.mode = carrot.mode
          carrot.enable = True
        else: #carrot在红车停车后切换到baended模式，则禁止carrot控制
          carrot.enable = False
      else:
        if v_ego_kph > 6.0: #大于6km/h超过倒计时时间(3.0秒)后切换到carrot控制
          if carrot.blended_count == 0:
            carrot.xState = XState.e2ePrepare
            carrot.blended_request = False
            v_cruise = carrot.update(sm, v_cruise_kph, False)
            calib_v_cruise = False
            self.mpc.mode = carrot.mode
            carrot.enable = True
          else:
            carrot.enable = False
            carrot.update(sm, v_cruise_kph, True)
          carrot.blended_count = max(0, carrot.blended_count - 1)  # 倒计时
        else:
          carrot.enable = False
          carrot.update(sm, v_cruise_kph, True)
    else:
      carrot.enable = False
      carrot.update(sm, v_cruise_kph, True)

    if not carrot.enable and calib_v_cruise:
      vCluRatio = sm['carState'].vCluRatio
      if vCluRatio > 0.5:
        self.vCluRatio = vCluRatio
        v_cruise *= vCluRatio

    long_control_off = sm['controlsState'].longControlState == LongCtrlState.off
    force_slow_decel = sm['controlsState'].forceDecel

    # Reset current state when not engaged, or user is controlling the speed
    reset_state = long_control_off if self.CP.openpilotLongitudinalControl else not sm['carControl'].hudControl.speedVisible

    #fishsp add
    # PCM cruise speed may be updated a few cycles later, check if initialized
    v_cruise_initialized = sm['controlsState'].vCruise != V_CRUISE_UNSET
    reset_state = reset_state or not v_cruise_initialized or carrot.soft_hold_active
    # fishsp add

    # No change cost when user is controlling the speed, or when standstill
    prev_accel_constraint = not (reset_state or sm['carState'].standstill)

    # 获取加速度限制
    accel_limits = [A_CRUISE_MIN, get_max_accel(v_ego, self.eco)]
    accel_limits_turns = limit_accel_in_turns(v_ego, sm['carState'].steeringAngleDeg, accel_limits, self.CP)

    #if self.mpc.mode == 'acc':
    #  accel_limits = [A_CRUISE_MIN, get_max_accel(v_ego, False)]
    #  accel_limits_turns = limit_accel_in_turns(v_ego, sm['carState'].steeringAngleDeg, accel_limits, self.CP)
    #else:
    #  accel_limits = [ACCEL_MIN, ACCEL_MAX]
    #  accel_limits_turns = [ACCEL_MIN, ACCEL_MAX]

    if reset_state:
      self.v_desired_filter.x = v_ego
      # Clip aEgo to cruise limits to prevent large accelerations when becoming active
      self.a_desired = clip(sm['carState'].aEgo, accel_limits[0], accel_limits[1])

      self.mpc.prev_a = np.full(N + 1, self.a_desired)  ## carrot
      accel_limits_turns[0] = accel_limits_turns[0] = 0.0  ## carrot

    # Prevent divergence, smooth in current v_ego
    self.v_desired_filter.x = max(0.0, self.v_desired_filter.update(v_ego))
    # Compute model v_ego error
    self.v_model_error = get_speed_error(sm['modelV2'], v_ego)
    x, v, a, j = self.parse_model(sm['modelV2'], self.v_model_error)

    if force_slow_decel:
      v_cruise = 0.0

    # Get active solutions for custom long mpc.
    v_cruise_limit = self.cruise_solutions(
      not reset_state and (self.CP.openpilotLongitudinalControl or not self.CP.pcmCruiseSpeed),
      self.v_desired_filter.x, self.a_desired, v_cruise, sm)

    v_cruise_org = v_cruise
    v_cruise =min(v_cruise_limit, v_cruise)

    #打印调试信息
    if self.frame % 4 == 0:
      v_cruise_org_kph_show = v_cruise_org*CV.MS_TO_KPH
      v_cruise_limit_kph_show = v_cruise_limit * CV.MS_TO_KPH
      #print(f"traffic: {carrot.trafficState}, mode: {self.mpc.mode}, xState: {carrot.xState}")
      #print(f"v_cruise: {v_cruise_org_kph_show:.2f}, v_cruise_limit: {v_cruise_limit_kph_show:.2f}, dist: {carrot.stop_dist:.2f}")

    # clip limits, cannot init MPC outside of bounds
    accel_limits_turns[0] = min(accel_limits_turns[0], self.a_desired + 0.05)
    accel_limits_turns[1] = max(accel_limits_turns[1], self.a_desired - 0.05)

    self.mpc.set_weights(prev_accel_constraint, personality=sm['controlsState'].personality)
    self.mpc.set_accel_limits(accel_limits_turns[0], accel_limits_turns[1])
    self.mpc.set_cur_state(self.v_desired_filter.x, self.a_desired)
    self.mpc.update(carrot, reset_state, sm['radarState'], v_cruise, x, v, a, j, personality=sm['controlsState'].personality)

    self.v_desired_trajectory_full = np.interp(ModelConstants.T_IDXS, T_IDXS_MPC, self.mpc.v_solution)
    self.a_desired_trajectory_full = np.interp(ModelConstants.T_IDXS, T_IDXS_MPC, self.mpc.a_solution)
    self.v_desired_trajectory = self.v_desired_trajectory_full[:CONTROL_N]
    self.a_desired_trajectory = self.a_desired_trajectory_full[:CONTROL_N]
    self.j_desired_trajectory = np.interp(ModelConstants.T_IDXS[:CONTROL_N], T_IDXS_MPC[:-1], self.mpc.j_solution)

    # TODO counter is only needed because radar is glitchy, remove once radar is gone
    self.fcw = self.mpc.crash_cnt > 2 and not sm['carState'].standstill
    if self.fcw:
      cloudlog.info("FCW triggered")

    # Interpolate 0.05 seconds and save as starting point for next iteration
    a_prev = self.a_desired
    self.a_desired = float(interp(self.dt, ModelConstants.T_IDXS[:CONTROL_N], self.a_desired_trajectory))
    self.v_desired_filter.x = self.v_desired_filter.x + self.dt * (self.a_desired + a_prev) / 2.0

    self.e2e_events(sm)

    self.frame += 1

  def publish(self, sm, pm):
    plan_send = messaging.new_message('longitudinalPlan')

    plan_send.valid = sm.all_checks(service_list=['carState', 'controlsState'])

    longitudinalPlan = plan_send.longitudinalPlan
    longitudinalPlan.modelMonoTime = sm.logMonoTime['modelV2']
    longitudinalPlan.processingDelay = (plan_send.logMonoTime / 1e9) - sm.logMonoTime['modelV2']

    longitudinalPlan.speeds = self.v_desired_trajectory.tolist()
    longitudinalPlan.accels = self.a_desired_trajectory.tolist()
    longitudinalPlan.jerks = self.j_desired_trajectory.tolist()

    longitudinalPlan.hasLead = sm['radarState'].leadOne.status
    longitudinalPlan.longitudinalPlanSource = self.mpc.source
    longitudinalPlan.fcw = self.fcw

    longitudinalPlan.solverExecutionTime = self.mpc.solve_time

    pm.send('longitudinalPlan', plan_send)

    plan_sp_send = messaging.new_message('longitudinalPlanSP')

    plan_sp_send.valid = sm.all_checks(service_list=['carState', 'controlsState'])

    longitudinalPlanSP = plan_sp_send.longitudinalPlanSP

    longitudinalPlanSP.longitudinalPlanSource = self.mpc.source if self.mpc.source != 'cruise' else self.cruise_source

    longitudinalPlanSP.e2eX = self.mpc.e2e_x.tolist()

    longitudinalPlanSP.visionTurnControllerState = self.vision_turn_controller.state
    longitudinalPlanSP.visionTurnSpeed = float(self.vision_turn_controller.v_target)
    longitudinalPlanSP.visionCurrentLatAcc = float(self.vision_turn_controller.current_lat_acc)
    longitudinalPlanSP.visionMaxPredLatAcc = float(self.vision_turn_controller.max_pred_lat_acc)

    longitudinalPlanSP.speedLimitControlState = self.speed_limit_controller.state
    longitudinalPlanSP.speedLimit = float(self.speed_limit_controller.speed_limit)
    longitudinalPlanSP.speedLimitOffset = float(self.speed_limit_controller.speed_limit_offset)
    longitudinalPlanSP.distToSpeedLimit = float(self.speed_limit_controller.distance)
    longitudinalPlanSP.isMapSpeedLimit = bool(self.speed_limit_controller.source not in (Source.none, Source.nav))
    longitudinalPlanSP.events = self.events.to_msg()

    longitudinalPlanSP.turnSpeedControlState = self.turn_speed_controller.state
    longitudinalPlanSP.turnSpeed = float(self.turn_speed_controller.v_target)

    longitudinalPlanSP.e2eBlended = self.mpc.mode

    pm.send('longitudinalPlanSP', plan_sp_send)

  def cruise_solutions(self, enabled, v_ego, a_ego, v_cruise, sm):
    # Update controllers
    self.vision_turn_controller.update(enabled, v_ego, v_cruise, sm)
    self.events = Events()
    self.speed_limit_controller.update(enabled, v_ego, a_ego, sm, v_cruise, self.events)
    self.turn_speed_controller.update(enabled, v_ego, sm, v_cruise)

    v_tsc_target = self.vision_turn_controller.v_target if self.vision_turn_controller.is_active else 255
    slc_target = self.speed_limit_controller.speed_limit_offseted if self.speed_limit_controller.is_active else 255
    m_tsc_target = self.turn_speed_controller.v_target if self.turn_speed_controller.is_active else 255

    # Pick solution with the lowest velocity target.
    v_solutions = min(v_cruise, v_tsc_target, slc_target, m_tsc_target)

    return v_solutions

  def e2e_events(self, sm):
    e2e_long_status = sm['e2eLongStateSP'].status

    if e2e_long_status in (1, 2):
      self.events.add(EventName.e2eLongStart)
