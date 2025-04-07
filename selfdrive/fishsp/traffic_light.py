from enum import Enum

from cereal import log
from openpilot.common.params import Params
import numpy as np
from openpilot.common.realtime import DT_MDL
from openpilot.common.conversions import Conversions as CV
from openpilot.common.filter_simple import StreamingMovingAverage
from openpilot.selfdrive.controls.lib.events import Events

#EventName = log.OnroadEvent.EventName
LaneChangeState = log.LaneChangeState
STANDSTILL_COUNT = 1.0 #车辆静止倒计时

class XState(Enum):
  lead = 0
  cruise = 1
  e2eCruise = 2
  e2eStop = 3
  e2ePrepare = 4
  e2eStopped = 5

  def __str__(self):
    return self.name

class DrivingMode(Enum):
  Eco = 1
  Safe = 2
  Normal = 3
  High = 4

  def __str__(self):
    return self.name

class TrafficState(Enum):
  off = 0
  red = 1
  green = 2

  def __str__(self):
    return self.name

class CarrotPlanner:
  def __init__(self):
    self.params = Params()
    self.params_count = 0
    self.frame = 0

    self.trafficState = TrafficState.off
    self.xStopFilter = StreamingMovingAverage(3)
    self.xStopFilter2 = StreamingMovingAverage(15)
    self.vFilter = StreamingMovingAverage(10)

    self.stop_distance = 6.0
    self.fakeCruiseDistance = 0.0
    self.xState = XState.cruise
    self.xStop = 0.0
    self.actual_stop_distance = 0.0
    self.stopping_count = 0
    self.traffic_starting_count = 0
    self.user_stop_distance = -1

    self.startSignCount = 0
    self.stopSignCount = 0

    self.stop_distance = 6.0
    self.trafficStopDistanceAdjust = 1.0 #params.get_float("TrafficStopDistanceAdjust") / 100.
    self.comfortBrake = 2.4 #params.get_float("ComfortBrake") / 100.
    self.comfort_brake = self.comfortBrake

    self.soft_hold_active = 0
    self.events = Events()

    #self.myDrivingMode = DrivingMode(self.params.get_int("MyDrivingMode"))
    #self.myDrivingMode_last = self.myDrivingMode
    #self.myDrivingMode_disable_auto = False
    #self.myEcoModeFactor = 0.9
    #self.mySafeModeFactor = 0.8
    #self.myHighModeFactor = 1.2
    #self.drivingModeDetector = DrivingModeDetector()
    self.mySafeFactor = 1.0

    self.tFollowGap1 = 1.1
    self.tFollowGap2 = 1.3
    self.tFollowGap3 = 1.45
    self.tFollowGap4 = 1.6

    self.dynamicTFollow = 0.0
    self.dynamicTFollowLC = 0.9

    self.cruiseMaxVals1 = 1.6
    self.cruiseMaxVals2 = 1.2
    self.cruiseMaxVals3 = 1.0
    self.cruiseMaxVals4 = 0.8
    self.cruiseMaxVals5 = 0.7
    self.cruiseMaxVals6 = 0.6

    self.trafficLightDetectMode = 2 # 0: None, 1:Stop, 2:Stop&Go
    self.trafficState_carrot = 0
    self.carrot_stay_stop = False

    self.eco_over_speed = 2
    self.eco_target_speed = 0

    self.desireState = 0.0
    self.jerk_factor = 1.0
    self.jerk_factor_apply = 1.0

    self.activeCarrot = 0
    self.xDistToTurn = 0
    self.atcType = ""
    self.atc_active = False

    self.v_cruise = 0
    self.stop_dist = 0
    self.mode = ""
    self.trafficStopAdjustRatio = 1
    self.enable = False
    self.blended_request = False
    self.blended_count = 0
    self.standstill_count = 0

  def _params_update(self):
    self.frame += 1
    self.params_count += 1

  # meta.desireState 是什么？
  # meta.desireState 是一个长度为8的数组，表示 E2E 模型预测的各种“驾驶意图”的概率分布。
  # 这些意图是 OpenPilot 中定义的 8 个“Desire”，其顺序如下（来自 selfdrive/modeld/constants.py）：
  # 索引	意图名称	含义
  # 0	none	没有特定意图
  # 1	keepLeft	保持偏左行驶
  # 2	keepRight	保持偏右行驶
  # 3	laneChangeLeft	准备或正在向左变道
  # 4	laneChangeRight	准备或正在向右变道
  # 5	turnLeft	准备或正在左转（路口）
  # 6	turnRight	准备或正在右转（路口）
  # 7	uturnLeft	准备或正在掉头
  def _update_model_desire(self, sm):
    meta = sm['modelV2'].meta
    carState = sm['carState']
    if meta.laneChangeState == LaneChangeState.laneChangeStarting: # laneChangig
      #如果你打了左转向灯，就去取 laneChangeLeft 的置信度（索引3）
      #如果你打了右转向灯，就取 laneChangeRight 的置信度（索引4）
      #这值通常是 0.0 ~ 1.0 之间的浮点数，代表“模型有多确定当前需要变道”，值越大说明变道意图很强
      self.desireState = meta.desireState[3] if carState.leftBlinker else meta.desireState[4]
    else:
      self.desireState = 0.0

  def dynamic_t_follow(self, t_follow, lead, desired_follow_distance):

    self.jerk_factor_apply = self.jerk_factor
    if self.desireState > 0.9:  # lane change state, desireState > 0.9说明模型变道意图强
      t_follow *= self.dynamicTFollowLC  # 变更车道时减少t_follow。
      self.jerk_factor_apply = self.jerk_factor * self.dynamicTFollowLC  # 变更车道时减少jerk factor aggresive
    elif lead.status:
      if self.dynamicTFollow > 0.0:
        gap_dist_adjust = np.clip((desired_follow_distance - lead.dRel) * self.dynamicTFollow, - 0.1, 1.0)
        t_follow += gap_dist_adjust
        if gap_dist_adjust < 0:
          self.jerk_factor_apply = self.jerk_factor * 0.5  # 跟随前方车辆的时候aggressive。

    return t_follow

  def update_stop_dist(self, stop_x):
    stop_x = self.xStopFilter.process(stop_x, median=True)
    stop_x = self.xStopFilter2.process(stop_x)
    return stop_x


  def check_model_stopping(self, v, v_ego, a_ego, model_x, y, d_rel):
    v_ego_kph = v_ego * CV.MS_TO_KPH
    model_v = self.vFilter.process(v[-1])
    startSign = model_v > 5.0 or model_v > (v[0]+2)

    if v_ego_kph < 1.0:
      stopSign = model_x < 20.0 and model_v < 10.0
    elif v_ego_kph < 85.0:
      stopSign = (model_x < d_rel - 3.0 and
                  model_x < np.interp(v[0], [60/3.6, 80/3.6], [120.0, 150]) and
                  ((model_v < 3.0) or (model_v < v[0]*0.7)) and
                  abs(y[-1]) < 5.0)

      if self.xState == XState.e2eCruise and a_ego < -1.0:
        stopSign = False
    else:
      stopSign = False

    self.stopSignCount = self.stopSignCount + 1 if stopSign else 0
    self.startSignCount = self.startSignCount + 1 if startSign and not stopSign else 0

    if self.stopSignCount * DT_MDL > 0.0:
      self.trafficState = TrafficState.red
    elif self.startSignCount * DT_MDL > 0.2:
      self.trafficState = TrafficState.green
    else:
      self.trafficState = TrafficState.off

  def update(self, sm, v_cruise_kph, skip_state_machine):
    self._params_update()
    self._update_model_desire(sm)
    self.events = Events()

    carstate = sm['carState']
    vCluRatio = carstate.vCluRatio
    #controlsState = sm['controlsState']
    radarstate = sm['radarState']
    model = sm['modelV2']

    #self.soft_hold_active = sm['carState'].softHoldActive  # carrot 2
    self.soft_hold_active = 0
    self.comfort_brake = self.comfortBrake

    v_ego = carstate.vEgo
    a_ego = carstate.aEgo
    v_ego_kph = v_ego * CV.MS_TO_KPH
    v_ego_cluster = carstate.vEgoCluster
    v_ego_cluster_kph = v_ego_cluster * CV.MS_TO_KPH

    v_cruise = v_cruise_kph * CV.KPH_TO_MS
    if vCluRatio > 0.5:
      v_cruise *= vCluRatio

    x = model.position.x
    y = model.position.y
    v = model.velocity.x

    self.fakeCruiseDistance = 0.0
    lead_detected = radarstate.leadOne.status #前方是否检测到车

    self.xStop = self.update_stop_dist(x[31])
    stop_model_x = self.xStop

    if skip_state_machine:
      return

    trafficState_last = self.trafficState
    self.check_model_stopping(v, v_ego, a_ego, x[-1], y, radarstate.leadOne.dRel if lead_detected else 1000)

    if carstate.gasPressed or carstate.brakePressed:
      self.user_stop_distance = -1

    if self.soft_hold_active > 0: #soft_hold_active > 0，则保持 e2eStopped
      self.xState = XState.e2eStopped
      #if trafficState_last in [TrafficState.off, TrafficState.red] and self.trafficState == TrafficState.green:
        #self.events.add(EventName.trafficSignChanged)
    elif self.xState == XState.e2eStopped: #如果处于 e2eStopped（完全停止）
      if carstate.gasPressed:
        self.xState = XState.e2ePrepare #如果检测到油门输入，进入 e2ePrepare（准备起步）
      elif lead_detected and (radarstate.leadOne.dRel - stop_model_x) < 2.0:
        self.xState = XState.lead #如果检测到前车接近，进入 lead（跟车模式）
      elif self.stopping_count == 0: #如果信号灯变绿，且没有特殊情况（如 carrot_stay_stop 或左转灯亮），进入 e2ePrepare
        if self.trafficState == TrafficState.green and not self.carrot_stay_stop and not carstate.leftBlinker:
          self.xState = XState.e2ePrepare
          self.blended_request = False
          #self.events.add(EventName.trafficSignGreen)
        elif carstate.standstill: #车辆静止时
          if not lead_detected: #没有前车
            if self.standstill_count == 0:
              self.blended_count = 3.0 / DT_MDL #计算3.0秒对应的周期次数
              self.blended_request = True
            self.standstill_count = max(0, self.standstill_count - 1)  # 倒计时
          else:
            self.standstill_count = 0
            self.blended_count = 0
        else:
          self.standstill_count = STANDSTILL_COUNT / DT_MDL #车辆静止后倒计时1秒
      self.stopping_count = max(0, self.stopping_count - 1)
      v_cruise = 0 #巡航速度设置为0
    elif self.xState == XState.e2eStop: #如果处于 e2eStop（预计停车
      self.stopping_count = 0 #重新计时 stopping_count = 0
      self.blended_count = 0 #blended模式计数
      self.standstill_count = 0
      self.blended_request = False
      if carstate.gasPressed:  # Stop detecting traffic signal for 10 seconds
        self.xState = XState.e2eCruise #如果驾驶员踩油门，直接进入 e2eCruise（巡航）
        self.traffic_starting_count = 10.0 / DT_MDL #设置 traffic_starting_count为10秒的计数
      elif lead_detected and (radarstate.leadOne.dRel - stop_model_x) < 2.0:
        self.xState = XState.lead #如果检测到前车接近，进入 lead（跟车模式）
      else:
        if self.trafficState == TrafficState.green: #如果信号灯变绿，触发 trafficSignGreen 事件，并进入 e2eCruise（巡航）
          #self.events.add(EventName.trafficSignGreen)
          self.xState = XState.e2eCruise
        else: #非绿灯，进入制动调整逻辑，计算停车距离 stop_dist
          self.comfort_brake = self.comfortBrake * 0.9 #self.comfortBrake = 2.4
          #self.comfort_brake = self.comfortBrake
          #self.comfort_brake = COMFORT_BRAKE
          #计算停车距离 stop_dist
          self.trafficStopAdjustRatio = np.interp(v_ego_kph, [0, 100], [1.0, 0.7]) #根据速度确定比例，trafficStopAdjustRatio: 1km - 1.0, 100km/h - 0.7
          stop_dist = self.xStop * np.interp(self.xStop, [0, 100], [1.0, self.trafficStopAdjustRatio])
          if stop_dist > 10.0:
            self.actual_stop_distance = stop_dist #设置实际停车距离为stop_dist
          stop_model_x = 0
          self.fakeCruiseDistance = 0 if self.actual_stop_distance > 10.0 else 10.0
          if v_ego < 0.3: #若车速 v_ego < 0.3，则进入 e2eStopped
            self.stopping_count = 0.5 / DT_MDL #计算停车时间0.5秒对应的次数
            self.xState = XState.e2eStopped #设置状态为e2eStopped
            self.standstill_count = STANDSTILL_COUNT / DT_MDL
    elif self.xState == XState.e2ePrepare: #如果处于 e2ePrepare（准备起步）
      if lead_detected: #如果前方有车，进入 lead（跟车）
        self.xState = XState.lead
      elif self.atc_active: #自动转弯激活 且 踩了油门则进入e2eCruise
        if carstate.gasPressed:
          self.xState = XState.e2eCruise
      elif v_ego_kph < 5.0 and self.trafficState != TrafficState.green: #如果 车速 < 5km/h 且 交通灯不是绿灯，进入 e2eStop（预计停车）
        self.xState = XState.e2eStop
        self.actual_stop_distance = 5.0 #设置实际停车距离为5米
      elif v_ego_kph > 5.0: # and stop_model_x > 30.0:
        self.xState = XState.e2eCruise #如果 车速 > 5km/h，进入 e2eCruise（巡航）
        self.blended_request = False
    else: #XState.lead, XState.cruise, XState.e2eCruise
      self.blended_request = False
      self.traffic_starting_count = max(0, self.traffic_starting_count - 1)
      if lead_detected: #有前车则进入XState.lead状态
        self.xState = XState.lead
      #如果红灯且方向盘角度 < 30°，触发 trafficStopping 事件，并进入 e2eStop
      elif self.trafficState == TrafficState.red and abs(carstate.steeringAngleDeg) < 30 and self.traffic_starting_count == 0:
        #self.events.add(EventName.trafficStopping)
        self.xState = XState.e2eStop
        self.actual_stop_distance = self.xStop
      else: #否则，保持 e2eCruise
        self.xState = XState.e2eCruise

    #如果 trafficState 为绿灯或关闭，或者 xState 不是 e2eStop/e2eStopped,则 stop_model_x = 1000.0（表示无需考虑停车）
    if self.trafficState in [TrafficState.off, TrafficState.green] or self.xState not in [XState.e2eStop, XState.e2eStopped]:
      stop_model_x = 1000.0

    #这段代码应该没生效，因为没有看到user_stop_distance可能大于0的情况
    if self.user_stop_distance >= 0:
      self.user_stop_distance = max(0, self.user_stop_distance - v_ego * DT_MDL)
      self.actual_stop_distance = self.user_stop_distance
      self.xState = XState.e2eStop if self.user_stop_distance > 0 else XState.e2eStopped

    # 如果 xState 是 e2ePrepare（准备起步），mode = 'blended', 否则，mode = 'acc'（自适应巡航模式）
    mode = 'blended' if (self.xState in [XState.e2ePrepare]) or self.blended_request else 'acc'

    self.comfort_brake *= self.mySafeFactor #self.mySafeFactor = 1.0

    #随着车辆前进，逐步减少 actual_stop_distance，v_ego * DT_MDL 表示在一个循环周期内车辆行驶的距离
    self.actual_stop_distance = max(0, self.actual_stop_distance - (v_ego * DT_MDL))

    if stop_model_x == 1000.0: ## 说明状态为e2eCruise, lead
      self.actual_stop_distance = 0.0
    elif self.actual_stop_distance > 0: ## e2eStop, e2eStopped
      stop_model_x = 0.0

    stop_dist =  stop_model_x + self.actual_stop_distance
    stop_dist = max(stop_dist, v_ego ** 2 / (self.comfort_brake * 2))

    self.v_cruise = v_cruise
    self.stop_dist = stop_dist
    self.mode = mode

    return v_cruise

#class DrivingModeDetector:
#  def __init__(self):
#    self.congested = False
#    self.speed_threshold = 2  # (km/h)
#    self.accel_threshold = 1.5  # (m/s^2)
#    self.distance_threshold = 12  # (m)
#    self.lead_speed_exit_threshold = 20  # (km/h)

#  def update_data(self, my_speed, lead_speed, my_accel, lead_accel, distance):
#    # 1. 停车条件:前车在附近而停车的情况
#    if distance <= self.distance_threshold and lead_speed <= self.speed_threshold:
#      self.congested = True
#
#    # 2. 行驶条件:前车加速或快速移动
#    if lead_accel > self.accel_threshold or my_speed > self.lead_speed_exit_threshold or distance >= 200:
#      self.congested = False
#
#  def get_mode(self):
#    return DrivingMode.Safe if self.congested else DrivingMode.Normal