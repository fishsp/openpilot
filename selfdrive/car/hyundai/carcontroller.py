from cereal import car
import cereal.messaging as messaging
from openpilot.common.conversions import Conversions as CV
from openpilot.common.numpy_fast import clip, interp
from openpilot.common.params import Params
from openpilot.common.realtime import DT_CTRL
from opendbc.can.packer import CANPacker
from openpilot.selfdrive.car import apply_driver_steer_torque_limits, common_fault_avoidance
from openpilot.selfdrive.car.hyundai import hyundaicanfd, hyundaican
from openpilot.selfdrive.car.hyundai.hyundaicanfd import CanBus
from openpilot.selfdrive.car.hyundai.values import HyundaiFlags, HyundaiFlagsSP, Buttons, CarControllerParams, CANFD_CAR, CAR, CAMERA_SCC_CAR, LEGACY_SAFETY_MODE_CAR
from openpilot.selfdrive.car.interfaces import CarControllerBase
from openpilot.selfdrive.controls.lib.drive_helpers import HYUNDAI_V_CRUISE_MIN
from openpilot.common.logger import logger
from openpilot.selfdrive.controls.lib.longitudinal_planner import get_max_accel, get_max_jerk

VisualAlert = car.CarControl.HUDControl.VisualAlert
LongCtrlState = car.CarControl.Actuators.LongControlState

# EPS faults if you apply torque while the steering angle is above 90 degrees for more than 1 second
# All slightly below EPS thresholds to avoid fault
MAX_ANGLE = 85
MAX_ANGLE_FRAMES = 89
MAX_ANGLE_CONSECUTIVE_FRAMES = 2

def process_hud_alert(enabled, fingerprint, hud_control):
  sys_warning = (hud_control.visualAlert in (VisualAlert.steerRequired, VisualAlert.ldw))

  # initialize to no line visible
  # TODO: this is not accurate for all cars
  sys_state = 1
  if hud_control.leftLaneVisible and hud_control.rightLaneVisible or sys_warning:  # HUD alert only display when LKAS status is active
    sys_state = 3 if enabled or sys_warning else 4
  elif hud_control.leftLaneVisible:
    sys_state = 5
  elif hud_control.rightLaneVisible:
    sys_state = 6

  # initialize to no warnings
  left_lane_warning = 0
  right_lane_warning = 0
  if hud_control.leftLaneDepart:
    left_lane_warning = 1 if fingerprint in (CAR.GENESIS_G90, CAR.GENESIS_G80) else 2
  if hud_control.rightLaneDepart:
    right_lane_warning = 1 if fingerprint in (CAR.GENESIS_G90, CAR.GENESIS_G80) else 2

  return sys_warning, sys_state, left_lane_warning, right_lane_warning


class CarController(CarControllerBase):
  def __init__(self, dbc_name, CP, VM):
    self.CP = CP
    self.CAN = CanBus(CP)
    self.params = CarControllerParams(CP)
    self.packer = CANPacker(dbc_name)
    self.angle_limit_counter = 0
    self.frame = 0

    self.accel_last = 0
    self.apply_steer_last = 0
    self.car_fingerprint = CP.carFingerprint
    self.last_button_frame = 0

    self.disengage_blink = 0.
    self.lat_disengage_init = False
    self.lat_active_last = False

    self.accel_ramp_time = 0.0
    self.target_accel = 0.0
    self.jerk_limit = 0.0
    self.cruiseState_last = False
    self.accel_limit = 0.0
    self.accel_start = 0.0
    self.clip_accel = False
    self.long_control_time = 0.0
    self.long_log = False
    self.jerk_limit_org = 0.0
    self.accel_limit_org = 0.0
    self.normal_log_num = 0
    self.pcmCruiseSpeed_last = False
    self.log_enable = False  # 是否允许日志记录
    self.gasPressed = False
    self.gasPressed_last = False
    self.gas_change_smooth = False

    sub_services = ['longitudinalPlanSP', 'carState']
    if CP.openpilotLongitudinalControl:
      sub_services.append('radarState')
    # TODO: Always true, prep for future conditional refactoring
    if sub_services:
      self.sm = messaging.SubMaster(sub_services)

    self.param_s = Params()
    self.is_metric = self.param_s.get_bool("IsMetric")
    self.speed_limit_control_enabled = False
    self.last_speed_limit_sign_tap = False
    self.last_speed_limit_sign_tap_prev = False
    self.speed_limit = 0.
    self.speed_limit_offset = 0
    self.timer = 0
    self.final_speed_kph = 0
    self.init_speed = 0
    self.current_speed = 0
    self.v_set_dis = 0
    self.v_cruise_min = 0
    self.button_type = 0
    self.button_select = 0
    self.button_count = 0
    self.target_speed = 0
    self.t_interval = 7
    self.slc_active_stock = False
    self.sl_force_active_timer = 0
    self.v_tsc_state = 0
    self.slc_state = 0
    self.m_tsc_state = 0
    self.cruise_button = None
    self.speed_diff = 0
    self.v_tsc = 0
    self.m_tsc = 0
    self.steady_speed = 0
    self.lead_distance = 0
    self.hkg_can_smooth_stop = self.param_s.get_bool("HkgSmoothStop")
    self.accel_eco = self.param_s.get_bool("SubaruManualParkingBrakeSng") #ECO加速模式
    self.cruise_smooth_dis = self.param_s.get_bool("StockLongToyota") #巡航平滑
    self.custom_accel_limit = self.param_s.get_bool("LkasToggle") #用户限制加速度

    self.jerk = 0.0
    self.jerk_l = 0.0
    self.jerk_u = 0.0
    self.jerkStartLimit = 2.0
    self.cb_upper = 0.0
    self.cb_lower = 0.0
    self.jerk_count = 0.0

    self.accel_raw = 0
    self.accel_val = 0
    self.accel_last_jerk = 0
    self.base_time = 0.13  # 基础平滑时间 0.1(加速约1.57秒) 0.13(加速约2秒)
    self.k1 = 0.04  # 速度影响因子 0.03(加速约1.57秒) 0.04(加速约1.57秒)
    self.brake_factor = 0.4  # 减速时的快速响应因子(减速约0.46s)
    self.last_accel = 0.0  # 记录上一次的加速度

    self.lkas_toggle = self.param_s.get_bool("LkasToggle")
    logger.log("lkas", LkasToggle=self.lkas_toggle)

  def calculate_lead_distance(self, hud_control: car.CarControl.HUDControl) -> float:
    lead_one = self.sm["radarState"].leadOne
    lead_two = self.sm["radarState"].leadTwo

    if lead_one.status and (not lead_two.status or lead_one.dRel < lead_two.dRel):
      return lead_one.dRel
    if lead_two.status:
      return lead_two.dRel

    return 19 if hud_control.leadVisible else 0

  def update(self, CC, CS, now_nanos):
    if not self.CP.pcmCruiseSpeed or (self.CP.openpilotLongitudinalControl and self.frame % 5 == 0):
      self.sm.update(0)

    self.gasPressed = self.sm['carState'].gasPressed
    gas_press_change = not self.gasPressed and self.gasPressed_last and CS.out.cruiseState.enabled  # 由踩油门变成未踩油门并且开启了巡航
    if gas_press_change:
      logger.log("gas press change")
    self.gasPressed_last = self.gasPressed

    if not self.CP.pcmCruiseSpeed:
      if self.sm.updated['longitudinalPlanSP']:
        self.v_tsc_state = self.sm['longitudinalPlanSP'].visionTurnControllerState
        self.slc_state = self.sm['longitudinalPlanSP'].speedLimitControlState
        self.m_tsc_state = self.sm['longitudinalPlanSP'].turnSpeedControlState
        self.speed_limit = self.sm['longitudinalPlanSP'].speedLimit
        self.speed_limit_offset = self.sm['longitudinalPlanSP'].speedLimitOffset
        self.v_tsc = self.sm['longitudinalPlanSP'].visionTurnSpeed
        self.m_tsc = self.sm['longitudinalPlanSP'].turnSpeed

      if self.frame % 200 == 0:
        self.speed_limit_control_enabled = self.param_s.get_bool("EnableSlc")
        self.is_metric = self.param_s.get_bool("IsMetric")
      self.last_speed_limit_sign_tap = self.param_s.get_bool("LastSpeedLimitSignTap")
      self.v_cruise_min = HYUNDAI_V_CRUISE_MIN[self.is_metric] * (CV.KPH_TO_MPH if not self.is_metric else 1)

    if self.frame % 200 == 0:
      self.accel_eco = self.param_s.get_bool("SubaruManualParkingBrakeSng")  # ECO加速模式
      self.cruise_smooth_dis = self.param_s.get_bool("StockLongToyota")  # 巡航平滑
      self.custom_accel_limit = self.param_s.get_bool("LkasToggle")  # 用户限制加速度
    
    actuators = CC.actuators
    hud_control = CC.hudControl

    # steering torque
    new_steer = int(round(actuators.steer * self.params.STEER_MAX))
    apply_steer = apply_driver_steer_torque_limits(new_steer, self.apply_steer_last, CS.out.steeringTorque, self.params)

    # >90 degree steering fault prevention
    self.angle_limit_counter, apply_steer_req = common_fault_avoidance(abs(CS.out.steeringAngleDeg) >= MAX_ANGLE, CC.latActive,
                                                                       self.angle_limit_counter, MAX_ANGLE_FRAMES,
                                                                       MAX_ANGLE_CONSECUTIVE_FRAMES)

    if not CC.latActive:
      apply_steer = 0

    # Hold torque with induced temporary fault when cutting the actuation bit
    torque_fault = CC.latActive and not apply_steer_req

    self.apply_steer_last = apply_steer

    # accel + longitudinal
    accel = clip(actuators.accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX)
    stopping = actuators.longControlState == LongCtrlState.stopping
    set_speed_in_units = hud_control.setSpeed * (CV.MS_TO_KPH if CS.is_metric else CV.MS_TO_MPH)

    # HUD messages
    sys_warning, sys_state, left_lane_warning, right_lane_warning = process_hud_alert(CC.enabled, self.car_fingerprint,
                                                                                      hud_control)

    # show LFA "white_wheel" and LKAS "White car + lanes" when not CC.latActive
    lateral_paused = CS.madsEnabled and not CC.latActive
    if CC.latActive:
      self.lat_disengage_init = False
    elif self.lat_active_last:
      self.lat_disengage_init = True

    if not self.lat_disengage_init:
      self.disengage_blink = self.frame

    blinking_icon = (self.frame - self.disengage_blink) * DT_CTRL < 1.0 if self.lat_disengage_init else False

    if not self.CP.pcmCruiseSpeed:
      if not self.last_speed_limit_sign_tap_prev and self.last_speed_limit_sign_tap:
        self.sl_force_active_timer = self.frame
        self.param_s.put_bool_nonblocking("LastSpeedLimitSignTap", False)
      self.last_speed_limit_sign_tap_prev = self.last_speed_limit_sign_tap

      sl_force_active = self.speed_limit_control_enabled and (self.frame < (self.sl_force_active_timer * DT_CTRL + 2.0))
      sl_inactive = not sl_force_active and (not self.speed_limit_control_enabled or (True if self.slc_state == 0 else False))
      sl_temp_inactive = not sl_force_active and (self.speed_limit_control_enabled and (True if self.slc_state == 1 else False))
      slc_active = not sl_inactive and not sl_temp_inactive

      self.slc_active_stock = slc_active

    self.lat_active_last = CC.latActive

    escc = self.CP.spFlags & HyundaiFlagsSP.SP_ENHANCED_SCC.value

    can_sends = []

    # *** common hyundai stuff ***

    # tester present - w/ no response (keeps relevant ECU disabled)
    if self.frame % 100 == 0 and not ((self.CP.flags & HyundaiFlags.CANFD_CAMERA_SCC.value) or escc) and \
      self.CP.carFingerprint not in CAMERA_SCC_CAR and self.CP.openpilotLongitudinalControl:
      # for longitudinal control, either radar or ADAS driving ECU
      addr, bus = 0x7d0, 0
      if self.CP.flags & HyundaiFlags.CANFD_HDA2.value:
        addr, bus = 0x730, self.CAN.ECAN
      can_sends.append([addr, 0, b"\x02\x3E\x80\x00\x00\x00\x00\x00", bus])

      # for blinkers
      if self.CP.flags & HyundaiFlags.ENABLE_BLINKERS:
        can_sends.append([0x7b1, 0, b"\x02\x3E\x80\x00\x00\x00\x00\x00", self.CAN.ECAN])

    self.clip_accel = False
    if self.CP.openpilotLongitudinalControl:
      speed = CS.out.vEgoRaw  # 当前车速（m/s）
      # 定义车速区间对应的 jerk 和 accel 限制值
      accel_limits_tb = {
        0: {"jerk": 0.4, "accel": 0.8},  # 0 km/h
        0.56: {"jerk": 0.5, "accel": 1.0},  # 2 km/h
        1.11: {"jerk": 0.6, "accel": 1.2},  # 4 km/h
        1.67: {"jerk": 0.6, "accel": 1.4},  # 6 km/h
        2.22: {"jerk": 0.6, "accel": 1.4},  # 8 km/h
        2.78: {"jerk": 0.6, "accel": 1.4},  # 10 km/h
        4.17: {"jerk": 0.7, "accel": 1.4},  # 15 km/h
        5.56: {"jerk": 0.7, "accel": 1.3},  # 20 km/h
        6.94: {"jerk": 0.7, "accel": 1.3},  # 25 km/h
        8.33: {"jerk": 0.7, "accel": 1.2},  # 30 km/h
        10.0: {"jerk": 0.7, "accel": 1.1},  # 35 km/h
        11.11: {"jerk": 0.7, "accel": 1.0},  # 40 km/h
        12.22: {"jerk": 0.7, "accel": 1.0},  # 45 km/h
        13.33: {"jerk": 0.6, "accel": 1.0},  # 50 km/h
        14.44: {"jerk": 0.4, "accel": 0.9},  # 55 km/h
        15.55: {"jerk": 0.3, "accel": 0.8},  # 60 km/h
        16.67: {"jerk": 0.3, "accel": 0.7},  # 65 km/h
        17.78: {"jerk": 0.2, "accel": 0.6},  # 70 km/h
        18.89: {"jerk": 0.2, "accel": 0.5},  # 75 km/h
        22.22: {"jerk": 0.2, "accel": 0.5},  # 80 km/h
      }
      eco_accel_limits_tb = {
        0: {"jerk": 0.3, "accel": 0.8},  # 0 km/h
        0.56: {"jerk": 0.3, "accel": 0.9},  # 2 km/h
        1.11: {"jerk": 0.3, "accel": 1.0},  # 4 km/h
        1.67: {"jerk": 0.3, "accel": 1.1},  # 6 km/h
        2.22: {"jerk": 0.4, "accel": 1.2},  # 8 km/h
        2.78: {"jerk": 0.5, "accel": 1.3},  # 10 km/h
        4.17: {"jerk": 0.5, "accel": 1.3},  # 15 km/h
        5.56: {"jerk": 0.5, "accel": 1.2},  # 20 km/h
        6.94: {"jerk": 0.5, "accel": 1.1},  # 25 km/h
        8.33: {"jerk": 0.5, "accel": 1.0},  # 30 km/h
        10.0: {"jerk": 0.5, "accel": 1.0},  # 35 km/h
        11.11: {"jerk": 0.4, "accel": 1.0},  # 40 km/h
        12.22: {"jerk": 0.4, "accel": 1.0},  # 45 km/h
        13.33: {"jerk": 0.3, "accel": 0.9},  # 50 km/h
        14.44: {"jerk": 0.3, "accel": 0.9},  # 55 km/h
        15.55: {"jerk": 0.3, "accel": 0.8},  # 60 km/h
        16.67: {"jerk": 0.3, "accel": 0.7},  # 65 km/h
        17.78: {"jerk": 0.2, "accel": 0.6},  # 70 km/h
        18.89: {"jerk": 0.2, "accel": 0.6},  # 75 km/h
        22.22: {"jerk": 0.2, "accel": 0.5},  # 80 km/h
      }

      # 纵向控制日志计时
      #if speed < 0.05: # 速度小于0.05m/s时认为停车了
      if CS.out.standstill:
        if self.long_log:
          if self.log_enable:
            logger.log("long log end", aEgo=CS.out.aEgo, speed=speed)
            logger.log("=======================================================")
        self.long_control_time = 0
        self.long_log = False
      elif self.long_control_time < 15.0: # 纵向控制的前15秒快速记录日志
        self.long_control_time += DT_CTRL
        if not self.long_log:
          if self.log_enable:
            logger.log("=======================================================")
            logger.log("long log start", aEgo=CS.out.aEgo, speed=speed)
          self.normal_log_num = 0
        self.long_log = True
      else:
        if self.long_log:
          if self.log_enable:
            logger.log("long log end", aEgo=CS.out.aEgo, speed=speed)
            logger.log("=======================================================")
        self.long_log = False

      # 默认的加速度限制和jerk限制
      stock_accel_limit = CarControllerParams.ACCEL_MAX
      stock_jerk_limit = 1.0
      eco_accel_limit = CarControllerParams.ACCEL_MAX/2
      eco_jerk_limit = 1.0
      accel_limit = stock_accel_limit
      jerk_limit = stock_jerk_limit

      # 分别查表得到stock和eco的jerk以及accel限制值
      if speed <= 0:  # 车速小于 0 km/h
        stock_jerk_limit = accel_limits_tb[0]["jerk"]  # 最大 jerk
        stock_accel_limit = accel_limits_tb[0]["accel"]  # 最大加速度
      elif speed >= 22.22:  # 车速大于 80 km/h (22.22 m/s)
        stock_jerk_limit = accel_limits_tb[22.22]["jerk"]  # 最小 jerk
        stock_accel_limit = accel_limits_tb[22.22]["accel"]  # 最小加速度
      else:
        stock_jerk_limit, stock_accel_limit = self.get_jerk_accel(speed, accel_limits_tb)  # 根据速度查表并插值

      if speed <= 0:  # 车速小于 0 km/h
        eco_jerk_limit = eco_accel_limits_tb[0]["jerk"]  # 最大 jerk
        eco_accel_limit = eco_accel_limits_tb[0]["accel"]  # 最大加速度
      elif speed >= 22.22:  # 车速大于 80 km/h (22.22 m/s)
        eco_jerk_limit = eco_accel_limits_tb[22.22]["jerk"]  # 最小 jerk
        eco_accel_limit = eco_accel_limits_tb[22.22]["accel"]  # 最小加速度
      else:
        eco_jerk_limit, eco_accel_limit = self.get_jerk_accel(speed, eco_accel_limits_tb)  # 根据速度查表并插值

      #根据模式选择加速度限制
      if self.accel_eco:
        if self.custom_accel_limit:
          accel_limit = eco_accel_limit
          jerk_limit = eco_jerk_limit
        else:
          accel_limit = get_max_accel(speed, True)
          jerk_limit = get_max_jerk(speed, True)
      else:
        if self.custom_accel_limit:
          accel_limit = stock_accel_limit
          jerk_limit = stock_jerk_limit
        else:
          accel_limit = get_max_accel(speed, False)
          jerk_limit = get_max_jerk(speed, False)

      # TEST
      if self.cruiseState_last != CS.out.cruiseState.enabled:
        if CS.out.cruiseState.enabled:
          print("cruiseState.enabled True")
        else:
          print("cruiseState.enabled False")
      if self.pcmCruiseSpeed_last != self.CP.pcmCruiseSpeed:
        if self.CP.pcmCruiseSpeed:
          print("pcmCruiseSpeed True")
        else:
          print("pcmCruiseSpeed False")
      self.pcmCruiseSpeed_last = self.CP.pcmCruiseSpeed
      # TEST

      # 非巡航状态则重置self.accel_ramp_time
      if not CS.out.cruiseState.enabled:
        self.accel_ramp_time = 0.0

      # 由非巡航状态变为巡航状态
      cruise_ramp = False
      cruise_state_change = not self.cruiseState_last and CS.out.cruiseState.enabled

      if cruise_state_change or gas_press_change or (CS.out.cruiseState.enabled and self.gasPressed): # 巡航状态开启时 或 巡航状态下从踩油门到释放油门 或 巡航状态下用户踩着油门
        if gas_press_change: # 巡航状态下从踩油门到释放油门
          self.gas_change_smooth = True
        elif self.gasPressed:
          self.gas_change_smooth = False

        self.accel_ramp_time = 0.0  # 计时清0
        self.accel_start = min(CS.out.aEgo, accel_limit)  # 在CS.out.aEgo, accel_limit中选小的作为初始加速度限制
        if self.accel_start < 0.1:
          self.accel_start = 0.1

        if self.log_enable and not self.gasPressed: # 在用户未踩油门时才允许打印日志
          logger.log("cruise start", speed=speed, aEgo=CS.out.aEgo, accel_start=self.accel_start,
                     accel_limit=accel_limit, jerk_limit=jerk_limit)

      if (CS.out.cruiseState.enabled and not self.cruise_smooth_dis and not self.gasPressed) or self.gas_change_smooth:  # 巡航状态开启时进行平滑 或者 从踩油门到释放油门时进行平滑
        accel_ramp_time_max = 5.0
        if self.accel_ramp_time < accel_ramp_time_max:
          cruise_ramp = True
          self.accel_ramp_time += DT_CTRL
          self.accel_ramp_time = min(self.accel_ramp_time, accel_ramp_time_max)  # 确保不会超过accel_ramp_time_max
          self.accel_limit = interp(self.accel_ramp_time, [0, accel_ramp_time_max], [self.accel_start, max(self.accel_start, accel_limit)])
          self.jerk_limit = interp(self.accel_ramp_time, [0, accel_ramp_time_max], [0.2, max(0.2, jerk_limit)])
          self.jerk = self.jerk_limit # 赋值给self.jerk，目的是为了防止平滑结束后cal_jerk函数计算结果self.jerk发生突变

          if self.frame % 10 == 0:
            if self.log_enable:
              logger.log("cruise ramp", time=self.accel_ramp_time, speed=speed, aEgo=CS.out.aEgo, self_accel_limit=self.accel_limit,
                         self_jerk_limit=self.jerk_limit, accel_limit=accel_limit, jerk_limit=jerk_limit)

          if self.accel_ramp_time >= accel_ramp_time_max:
            self.gas_change_smooth = False #结束除油门变化平滑
            logger.log("cruise ramp end")
        else:
          self.accel_limit = accel_limit  # 3秒后直接使用PID加速度
          self.jerk_limit = jerk_limit  # 3秒后直接使用jerk
      else:
        self.accel_limit = accel_limit
        self.jerk_limit = jerk_limit
        self.accel_ramp_time = 0  # 复位

      self.cruiseState_last = CS.out.cruiseState.enabled  # 记录状态
      self.jerk_limit_org = jerk_limit
      self.accel_limit_org = accel_limit

      if (actuators.accel >= 0) and (self.custom_accel_limit or cruise_ramp):  # 加速度大于指定值时 并且 (开启了用户限制(即HKG平滑停车开关) 或 在平滑巡航开启)
        accel = clip(actuators.accel, CarControllerParams.ACCEL_MIN, self.accel_limit) # 使用 clip 限制加速度，确保加速度在指定范围内
        self.clip_accel = True

      # 通过算法对加速度进行平滑
      #if self.hkg_can_smooth_stop:
        #accel = self.smooth_accel(accel, speed, DT_CTRL)
      accel = self.smooth_accel(accel, speed, DT_CTRL)

      self.make_jerk(CS, accel, actuators)

    # CAN-FD platforms
    if self.CP.carFingerprint in CANFD_CAR:
      hda2 = self.CP.flags & HyundaiFlags.CANFD_HDA2
      hda2_long = hda2 and self.CP.openpilotLongitudinalControl

      # steering control
      can_sends.extend(hyundaicanfd.create_steering_messages(self.packer, self.CP, self.CAN, CC.enabled, apply_steer_req, apply_steer,
                                                             lateral_paused, blinking_icon))

      # prevent LFA from activating on HDA2 by sending "no lane lines detected" to ADAS ECU
      if self.frame % 5 == 0 and hda2:
        can_sends.append(hyundaicanfd.create_suppress_lfa(self.packer, self.CAN, CS.hda2_lfa_block_msg,
                                                          self.CP.flags & HyundaiFlags.CANFD_HDA2_ALT_STEERING))

      # LFA and HDA icons
      if self.frame % 5 == 0 and (not hda2 or hda2_long):
        can_sends.append(hyundaicanfd.create_lfahda_cluster(self.packer, self.CAN, CC.enabled and CS.out.cruiseState.enabled, CC.latActive,
                                                            lateral_paused, blinking_icon))

      # blinkers
      if hda2 and self.CP.flags & HyundaiFlags.ENABLE_BLINKERS:
        can_sends.extend(hyundaicanfd.create_spas_messages(self.packer, self.CAN, self.frame, CC.leftBlinker, CC.rightBlinker))

      if self.CP.openpilotLongitudinalControl:
        if hda2:
          can_sends.extend(hyundaicanfd.create_adrv_messages(self.packer, self.CAN, self.frame))
        if self.frame % 2 == 0:
          can_sends.append(hyundaicanfd.create_new_acc_control(self.packer, self.CAN, CS, CC.enabled and CS.out.cruiseState.enabled, self.accel_last, accel, stopping, CC.cruiseControl.override,
                                                           set_speed_in_units, hud_control, self.jerk_u, self.jerk_l))
          self.accel_last = accel
      else:
        # button presses
        can_sends.extend(self.create_button_messages(CC, CS, use_clu11=False))
        if not (CC.cruiseControl.cancel or CC.cruiseControl.resume) and CS.out.cruiseState.enabled and not self.CP.pcmCruiseSpeed:
          if self.CP.flags & HyundaiFlags.CANFD_ALT_BUTTONS:
            # TODO: resume for alt button cars
            pass
          else:
            self.cruise_button = self.get_cruise_buttons(CS, CC.vCruise)
            if self.cruise_button is not None:
              if self.frame % 2 == 0:
                can_sends.append(hyundaicanfd.create_buttons(self.packer, self.CP, self.CAN, ((self.frame // 2) + 1) % 0x10, self.cruise_button))
    else:
      can_sends.append(hyundaican.create_lkas11(self.packer, self.frame, self.CP, apply_steer, apply_steer_req,
                                                torque_fault, CS.lkas11, sys_warning, sys_state, CC.enabled,
                                                hud_control.leftLaneVisible, hud_control.rightLaneVisible,
                                                left_lane_warning, right_lane_warning,
                                                lateral_paused, blinking_icon))

      if not self.CP.openpilotLongitudinalControl:
        can_sends.extend(self.create_button_messages(CC, CS, use_clu11=True))
        if not (CC.cruiseControl.cancel or CC.cruiseControl.resume) and CS.out.cruiseState.enabled and not self.CP.pcmCruiseSpeed:
          self.cruise_button = self.get_cruise_buttons(CS, CC.vCruise)
          if self.cruise_button is not None:
            if self.CP.carFingerprint in LEGACY_SAFETY_MODE_CAR:
              send_freq = 1
              if not (self.v_tsc_state != 0 or self.m_tsc_state > 1) and abs(self.target_speed - self.v_set_dis) <= 2:
                send_freq = 5
              # send resume at a max freq of 10Hz
              if (self.frame - self.last_button_frame) * DT_CTRL > 0.1 * send_freq:
                # send 25 messages at a time to increases the likelihood of cruise buttons being accepted
                can_sends.extend([hyundaican.create_clu11(self.packer, self.frame, CS.clu11, self.cruise_button, self.CP)] * 25)
                if (self.frame - self.last_button_frame) * DT_CTRL >= 0.15 * send_freq:
                  self.last_button_frame = self.frame
            elif self.frame % 2 == 0:
              can_sends.extend([hyundaican.create_clu11(self.packer, (self.frame // 2) + 1, CS.clu11, self.cruise_button, self.CP)] * 25)

      # Parse lead distance from radarState and display the corresponding distance in the car's cluster
      if self.CP.openpilotLongitudinalControl and self.sm.updated['radarState'] and self.frame % 5 == 0:
        self.lead_distance = self.calculate_lead_distance(hud_control)

      if self.frame % 2 == 0 and self.CP.openpilotLongitudinalControl:
        #if self.hkg_can_smooth_stop:
        #  stopping = stopping and CS.out.vEgoRaw < 0.05
        # TODO: unclear if this is needed
        jerk = 3.0 if actuators.longControlState == LongCtrlState.pid else 1.0
        use_fca = self.CP.flags & HyundaiFlags.USE_FCA.value
        self.make_accel(CS, actuators)

        if CS.out.cruiseState.enabled and self.log_enable:
          vego_kmh = CS.out.vEgo * 3.6
          if self.long_log:
            if self.frame % 10 == 0:
              logger.log("fast long log", speed=vego_kmh, accel=accel, aEgo=CS.out.aEgo,
                         actuators_accel=actuators.accel,
                         accel_raw=self.accel_raw, accel_val=self.accel_val, accel_limit=self.accel_limit_org,
                         self_accel_limit=self.accel_limit, jerk_limit=self.jerk_limit_org,
                         self_jerk_limit=self.jerk_limit, jerk_l=self.jerk_l, jerk_u=self.jerk_u)
          elif (self.frame % 200 == 0) and (self.normal_log_num < 30) and (
                  actuators.longControlState != LongCtrlState.off):  # 平常每2秒记录一次日志(共记录300条，10分钟)
            self.normal_log_num += 1
            logger.log("normal long log", speed=vego_kmh, accel=accel, aEgo=CS.out.aEgo,
                       actuators_accel=actuators.accel, accel_raw=self.accel_raw, accel_val=self.accel_val,
                       accel_limit=self.accel_limit_org,
                       self_accel_limit=self.accel_limit, jerk_limit=self.jerk_limit_org,
                       self_jerk_limit=self.jerk_limit, jerk_l=self.jerk_l, jerk_u=self.jerk_u)

        if self.clip_accel:
          #jerk = min(self.jerk_u, self.jerk_limit) #确保self.jerk_u不会超过self.jerk_limit
          jerk = self.jerk_limit
          can_sends.extend(hyundaican.create_new_acc_commands(self.packer, CC.enabled and CS.out.cruiseState.enabled, accel, accel, max(self.jerk_l, jerk), jerk, int(self.frame / 2),
                                                              hud_control, set_speed_in_units, stopping,
                                                              CC.cruiseControl.override, use_fca, CS, escc, self.CP, self.lead_distance, self.cb_lower, self.cb_upper))
        else:
          can_sends.extend(hyundaican.create_new_acc_commands(self.packer, CC.enabled and CS.out.cruiseState.enabled, self.accel_raw, self.accel_val, self.jerk_l, self.jerk_u, int(self.frame / 2),
                                                              hud_control, set_speed_in_units, stopping,
                                                              CC.cruiseControl.override, use_fca, CS, escc, self.CP, self.lead_distance, self.cb_lower, self.cb_upper))

      # 20 Hz LFA MFA message
      if self.frame % 5 == 0 and self.CP.flags & HyundaiFlags.SEND_LFA.value:
        can_sends.append(hyundaican.create_lfahda_mfc(self.packer, CC.enabled, CC.latActive, lateral_paused, blinking_icon))

      # 5 Hz ACC options
      if self.frame % 20 == 0 and self.CP.openpilotLongitudinalControl:
        can_sends.extend(hyundaican.create_acc_opt(self.packer, escc, CS, self.CP))

      # 2 Hz front radar options
      if self.frame % 50 == 0 and self.CP.openpilotLongitudinalControl and not escc:
        can_sends.append(hyundaican.create_frt_radar_opt(self.packer))

    new_actuators = actuators.as_builder()
    new_actuators.steer = apply_steer / self.params.STEER_MAX
    new_actuators.steerOutputCan = apply_steer
    new_actuators.accel = accel

    self.frame += 1
    return new_actuators, can_sends

  def create_button_messages(self, CC: car.CarControl, CS: car.CarState, use_clu11: bool):
    can_sends = []
    if use_clu11:
      if CC.cruiseControl.cancel:
        can_sends.append(hyundaican.create_clu11(self.packer, self.frame, CS.clu11, Buttons.CANCEL, self.CP))
      elif CC.cruiseControl.resume:
        # send resume at a max freq of 10Hz
        if (self.frame - self.last_button_frame) * DT_CTRL > 0.1:
          # send 25 messages at a time to increases the likelihood of resume being accepted
          can_sends.extend([hyundaican.create_clu11(self.packer, self.frame, CS.clu11, Buttons.RES_ACCEL, self.CP)] * 25)
          if (self.frame - self.last_button_frame) * DT_CTRL >= 0.15:
            self.last_button_frame = self.frame
    else:
      if (self.frame - self.last_button_frame) * DT_CTRL > 0.25:
        # cruise cancel
        if CC.cruiseControl.cancel:
          if self.CP.flags & HyundaiFlags.CANFD_ALT_BUTTONS:
            can_sends.append(hyundaicanfd.create_acc_cancel(self.packer, self.CP, self.CAN, CS.cruise_info))
            self.last_button_frame = self.frame
          else:
            for _ in range(20):
              can_sends.append(hyundaicanfd.create_buttons(self.packer, self.CP, self.CAN, CS.buttons_counter+1, Buttons.CANCEL))
            self.last_button_frame = self.frame

        # cruise standstill resume
        elif CC.cruiseControl.resume:
          if self.CP.flags & HyundaiFlags.CANFD_ALT_BUTTONS:
            # TODO: resume for alt button cars
            pass
          else:
            for _ in range(20):
              can_sends.append(hyundaicanfd.create_buttons(self.packer, self.CP, self.CAN, CS.buttons_counter+1, Buttons.RES_ACCEL))
            self.last_button_frame = self.frame

    return can_sends

  # multikyd methods, sunnyhaibin logic
  def get_cruise_buttons_status(self, CS):
    if not CS.out.cruiseState.enabled or CS.cruise_buttons[-1] != Buttons.NONE:
      self.timer = 40
    elif self.timer:
      self.timer -= 1
    else:
      return 1
    return 0

  def get_target_speed(self, v_cruise_kph_prev):
    v_cruise_kph = v_cruise_kph_prev
    if self.slc_state > 1:
      v_cruise_kph = (self.speed_limit + self.speed_limit_offset) * CV.MS_TO_KPH
      if not self.slc_active_stock:
        v_cruise_kph = v_cruise_kph_prev
    return v_cruise_kph

  def get_button_type(self, button_type):
    self.type_status = "type_" + str(button_type)
    self.button_picker = getattr(self, self.type_status, lambda: "default")
    return self.button_picker()

  def reset_button(self):
    if self.button_type != 3:
      self.button_type = 0

  def type_default(self):
    self.button_type = 0
    return None

  def type_0(self):
    self.button_count = 0
    self.target_speed = self.init_speed
    self.speed_diff = self.target_speed - self.v_set_dis
    if self.target_speed > self.v_set_dis:
      self.button_type = 1
    elif self.target_speed < self.v_set_dis and self.v_set_dis > self.v_cruise_min:
      self.button_type = 2
    return None

  def type_1(self):
    cruise_button = Buttons.RES_ACCEL
    self.button_count += 1
    if self.target_speed <= self.v_set_dis:
      self.button_count = 0
      self.button_type = 3
    elif self.button_count > 5:
      self.button_count = 0
      self.button_type = 3
    return cruise_button

  def type_2(self):
    cruise_button = Buttons.SET_DECEL
    self.button_count += 1
    if self.target_speed >= self.v_set_dis or self.v_set_dis <= self.v_cruise_min:
      self.button_count = 0
      self.button_type = 3
    elif self.button_count > 5:
      self.button_count = 0
      self.button_type = 3
    return cruise_button

  def type_3(self):
    cruise_button = None
    self.button_count += 1
    if self.button_count > self.t_interval:
      self.button_type = 0
    return cruise_button

  def get_curve_speed(self, target_speed_kph, v_cruise_kph_prev):
    if self.v_tsc_state != 0:
      vision_v_cruise_kph = self.v_tsc * CV.MS_TO_KPH
      if int(vision_v_cruise_kph) == int(v_cruise_kph_prev):
        vision_v_cruise_kph = 255
    else:
      vision_v_cruise_kph = 255
    if self.m_tsc_state > 1:
      map_v_cruise_kph = self.m_tsc * CV.MS_TO_KPH
      if int(map_v_cruise_kph) == 0.0:
        map_v_cruise_kph = 255
    else:
      map_v_cruise_kph = 255
    curve_speed = self.curve_speed_hysteresis(min(vision_v_cruise_kph, map_v_cruise_kph) + 2 * CV.MPH_TO_KPH)
    return min(target_speed_kph, curve_speed)

  def get_button_control(self, CS, final_speed, v_cruise_kph_prev):
    self.init_speed = round(min(final_speed, v_cruise_kph_prev) * (CV.KPH_TO_MPH if not self.is_metric else 1))
    self.v_set_dis = round(CS.out.cruiseState.speed * (CV.MS_TO_MPH if not self.is_metric else CV.MS_TO_KPH))
    cruise_button = self.get_button_type(self.button_type)
    return cruise_button

  def curve_speed_hysteresis(self, cur_speed: float, hyst=(0.75 * CV.MPH_TO_KPH)):
    if cur_speed > self.steady_speed:
      self.steady_speed = cur_speed
    elif cur_speed < self.steady_speed - hyst:
      self.steady_speed = cur_speed
    return self.steady_speed

  def get_cruise_buttons(self, CS, v_cruise_kph_prev):
    cruise_button = None
    if not self.get_cruise_buttons_status(CS):
      pass
    elif CS.out.cruiseState.enabled:
      set_speed_kph = self.get_target_speed(v_cruise_kph_prev)
      if self.slc_state > 1:
        target_speed_kph = set_speed_kph
      else:
        target_speed_kph = min(v_cruise_kph_prev, set_speed_kph)
      if self.v_tsc_state != 0 or self.m_tsc_state > 1:
        self.final_speed_kph = self.get_curve_speed(target_speed_kph, v_cruise_kph_prev)
      else:
        self.final_speed_kph = target_speed_kph

      cruise_button = self.get_button_control(CS, self.final_speed_kph, v_cruise_kph_prev)  # MPH/KPH based button presses
    return cruise_button

  # jerk calculations thanks to apilot!
  def cal_jerk(self, accel, actuators):
    self.accel_raw = accel
    if actuators.longControlState == LongCtrlState.off:
      accel_diff = 0.0
    elif actuators.longControlState == LongCtrlState.stopping:# or hud_control.softHold > 0:
      accel_diff = 0.0
    else:
      accel_diff = self.accel_raw - self.accel_last_jerk

    accel_diff /= DT_CTRL
    self.jerk = self.jerk * 0.9 + accel_diff * 0.1
    return self.jerk

  def make_jerk(self, CS, accel, actuators):
    jerk = self.cal_jerk(accel, actuators)
    a_error = accel - CS.out.aEgo
    jerk = jerk + (a_error * 2.0)

    startingJerk = 0.5
    jerkLimit = 5.0
    self.jerk_count += DT_CTRL
    jerk_max = interp(self.jerk_count, [0, 1.5, 2.5], [startingJerk, startingJerk, jerkLimit])
    if actuators.longControlState == LongCtrlState.off:
      self.jerk_u = jerkLimit
      self.jerk_l = jerkLimit
      self.jerk_count = 0
    else:
      self.jerk_u = min(max(0.5, jerk * 2.0), jerk_max)
      self.jerk_l = min(max(1.0, -jerk * 3.0), jerkLimit)

  def make_accel(self, CS, actuators):
    long_control = actuators.longControlState
    if long_control == LongCtrlState.off or (long_control == LongCtrlState.stopping and CS.out.standstill):
      self.accel_raw, self.accel_val = 0, 0
    else:
      self.accel_val = self.accel_raw
    self.accel_last = self.accel_val
    self.accel_last_jerk = self.accel_val

  def get_jerk_accel(self, speed, speed_limits):
    speed_keys = sorted(speed_limits.keys())  # 获取所有速度点，已排序
    low_speed = high_speed = speed_keys[0]

    # 处理超出范围的情况
    if speed <= speed_keys[0]:  # 低于最小值
      low_speed = high_speed = speed_keys[0]
    elif speed >= speed_keys[-1]:  # 超过最大值
      low_speed = high_speed = speed_keys[-1]
    else:
      # 查找 speed 所在的区间
      for i in range(len(speed_keys) - 1):
        if speed_keys[i] <= speed <= speed_keys[i + 1]:
          low_speed, high_speed = speed_keys[i], speed_keys[i + 1]
          break

    # 获取对应的 jerk 和 accel
    low_limits = speed_limits[low_speed]
    high_limits = speed_limits[high_speed]

    # 计算插值比例
    if low_speed == high_speed:  # 速度刚好匹配表中的值
      jerk = low_limits["jerk"]
      accel_limit = low_limits["accel"]
    else:
      ratio = (speed - low_speed) / (high_speed - low_speed)  # 线性插值系数
      jerk = low_limits["jerk"] + (high_limits["jerk"] - low_limits["jerk"]) * ratio
      accel_limit = low_limits["accel"] + (high_limits["accel"] - low_limits["accel"]) * ratio

    return jerk, accel_limit

  def smooth_accel(self, accel, speed, dt=0.01):  # 默认 dt = 0.01s (OpenPilot)
    # 计算基础平滑时间
    smoothing_time = self.base_time * (1 + speed * self.k1)

    # 如果加速度在减少（减速），缩短平滑时间
    if accel < self.last_accel:
      smoothing_time *= self.brake_factor

    # 计算平滑系数
    alpha = dt / (smoothing_time + dt)

    # 低通滤波
    smoothed_accel = (1 - alpha) * self.last_accel + alpha * accel
    self.last_accel = smoothed_accel  # 记录当前平滑加速度

    return smoothed_accel
