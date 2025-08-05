from cereal import car, custom
from panda import Panda
from openpilot.common.params import Params
from openpilot.selfdrive.car.hyundai.enable_radar_tracks import enable_radar_tracks
from openpilot.selfdrive.car.hyundai.hyundaicanfd import CanBus
from openpilot.selfdrive.car.hyundai.values import HyundaiFlags, HyundaiFlagsSP, CAR, DBC, CANFD_CAR, CAMERA_SCC_CAR, CANFD_RADAR_SCC_CAR, \
                                         CANFD_UNSUPPORTED_LONGITUDINAL_CAR, NON_SCC_CAR, EV_CAR, HYBRID_CAR, LEGACY_SAFETY_MODE_CAR, \
                                         UNSUPPORTED_LONGITUDINAL_CAR, Buttons
from openpilot.selfdrive.car.hyundai.radar_interface import RADAR_START_ADDR
from openpilot.selfdrive.car import create_button_events, get_safety_config, create_mads_event
from openpilot.selfdrive.car.interfaces import CarInterfaceBase
from openpilot.selfdrive.car.disable_ecu import disable_ecu

Ecu = car.CarParams.Ecu
ButtonType = car.CarState.ButtonEvent.Type
EventName = car.CarEvent.EventName
GearShifter = car.CarState.GearShifter
ENABLE_BUTTONS = (Buttons.RES_ACCEL, Buttons.SET_DECEL, Buttons.CANCEL)
BUTTONS_DICT = {Buttons.RES_ACCEL: ButtonType.accelCruise, Buttons.SET_DECEL: ButtonType.decelCruise,
                Buttons.GAP_DIST: ButtonType.gapAdjustCruise, Buttons.CANCEL: ButtonType.cancel}
AccelPersonality = custom.AccelerationPersonality


class CarInterface(CarInterfaceBase):
  @staticmethod
  def _get_params(ret, candidate, fingerprint, car_fw, experimental_long, docs):
    ret.carName = "hyundai"
    ret.radarUnavailable = RADAR_START_ADDR not in fingerprint[1] or DBC[ret.carFingerprint]["radar"] is None
    ret.customStockLongAvailable = True

    # These cars have been put into dashcam only due to both a lack of users and test coverage.
    # These cars likely still work fine. Once a user confirms each car works and a test route is
    # added to selfdrive/car/tests/routes.py, we can remove it from this list.
    # FIXME: the Optima Hybrid 2017 uses a different SCC12 checksum
    ret.dashcamOnly = candidate in ({CAR.KIA_OPTIMA_H, })

    hda2 = Ecu.adas in [fw.ecu for fw in car_fw]
    CAN = CanBus(None, hda2, fingerprint)

    if candidate in CANFD_CAR:
      # detect if car is hybrid
      if 0x105 in fingerprint[CAN.ECAN]:
        ret.flags |= HyundaiFlags.HYBRID.value
      elif candidate in EV_CAR:
        ret.flags |= HyundaiFlags.EV.value

      # detect HDA2 with ADAS Driving ECU
      if hda2:
        ret.flags |= HyundaiFlags.CANFD_HDA2.value
        if 0x110 in fingerprint[CAN.CAM]:
          ret.flags |= HyundaiFlags.CANFD_HDA2_ALT_STEERING.value
      else:
        # non-HDA2
        if 0x1cf not in fingerprint[CAN.ECAN]:
          ret.flags |= HyundaiFlags.CANFD_ALT_BUTTONS.value
          ret.customStockLongAvailable = False
        # ICE cars do not have 0x130; GEARS message on 0x40 or 0x70 instead
        if 0x130 not in fingerprint[CAN.ECAN]:
          if 0x40 not in fingerprint[CAN.ECAN]:
            ret.flags |= HyundaiFlags.CANFD_ALT_GEARS_2.value
          else:
            ret.flags |= HyundaiFlags.CANFD_ALT_GEARS.value
        if candidate not in CANFD_RADAR_SCC_CAR:
          ret.flags |= HyundaiFlags.CANFD_CAMERA_SCC.value
    else:
      # TODO: detect EV and hybrid
      if candidate in HYBRID_CAR:
        ret.flags |= HyundaiFlags.HYBRID.value
      elif candidate in EV_CAR:
        ret.flags |= HyundaiFlags.EV.value

      # Send LFA message on cars with HDA
      if 0x485 in fingerprint[2]:
        ret.flags |= HyundaiFlags.SEND_LFA.value

      # These cars use the FCA11 message for the AEB and FCW signals, all others use SCC12
      if 0x38d in fingerprint[0] or 0x38d in fingerprint[2]:
        ret.flags |= HyundaiFlags.USE_FCA.value

      if 0x2AB in fingerprint[0]:
        ret.spFlags |= HyundaiFlagsSP.SP_ENHANCED_SCC.value

      if 0x53E in fingerprint[2]:
        ret.spFlags |= HyundaiFlagsSP.SP_LKAS12.value

    ret.steerActuatorDelay = 0.1  # Default delay
    ret.steerLimitTimer = 0.4
    CarInterfaceBase.configure_torque_tune(candidate, ret.lateralTuning)

    if candidate == CAR.KIA_OPTIMA_G4_FL:
      ret.steerActuatorDelay = 0.2

    # *** longitudinal control ***
    if candidate in CANFD_CAR:
      ret.longitudinalTuning.kpV = [0.1]
      ret.longitudinalTuning.kiV = [0.0]
      ret.experimentalLongitudinalAvailable = candidate not in (CANFD_UNSUPPORTED_LONGITUDINAL_CAR | NON_SCC_CAR)
      if ret.flags & HyundaiFlags.CANFD_CAMERA_SCC and not hda2:
        ret.spFlags |= HyundaiFlagsSP.SP_CAMERA_SCC_LEAD.value
    else:
      ret.longitudinalTuning.kpV = [0.5]
      ret.longitudinalTuning.kiV = [0.0]
      ret.experimentalLongitudinalAvailable = candidate not in (UNSUPPORTED_LONGITUDINAL_CAR | NON_SCC_CAR)
      if candidate in CAMERA_SCC_CAR:
        ret.spFlags |= HyundaiFlagsSP.SP_CAMERA_SCC_LEAD.value
    ret.openpilotLongitudinalControl = experimental_long and ret.experimentalLongitudinalAvailable
    ret.pcmCruise = not ret.openpilotLongitudinalControl

    ret.stoppingControl = True
    ret.startingState = True
    ret.vEgoStarting = 0.1

    #startAccel = 0
    #vEgoStopping = 0.5
    try:
      val = Params().get("vEgoStopping")
      vEgoStopping = float(val)/10 if val is not None and val != b'' else 0.5
      val = Params().get("StartAccel")
      startAccel = float(val) / 10 if val is not None and val != b'' else 0
      val = Params().get("StopAccel")
      stopAccel = float(val) / 10 if val is not None and val != b'' else 0
    except AttributeError:
      startAccel = 0
      vEgoStopping = 0.5
      stopAccel = 0.

    ret.vEgoStopping = vEgoStopping
    if stopAccel <= -0.1:
      ret.stopAccel = stopAccel

    val = Params().get("AccelPersonality")
    if val is not None:
      try:
        accel_personality = int(val)
      except ValueError:
        accel_personality = AccelPersonality.stock
    else:
      accel_personality = AccelPersonality.stock

    if startAccel < 0.1:
      if accel_personality == AccelPersonality.eco:
        ret.startAccel = 0.6
      elif accel_personality == AccelPersonality.normal:
        ret.startAccel = 0.8
      elif accel_personality == AccelPersonality.sport:
        ret.startAccel = 1.2
      else: #stock
        ret.startAccel = 1.0
    else:
      ret.startAccel = startAccel

    ret.longitudinalActuatorDelayLowerBound = 0.5
    ret.longitudinalActuatorDelayUpperBound = 0.5

    if DBC[ret.carFingerprint]["radar"] is None:
      if ret.spFlags & (HyundaiFlagsSP.SP_ENHANCED_SCC | HyundaiFlagsSP.SP_CAMERA_SCC_LEAD):
        ret.radarUnavailable = False

    # *** feature detection ***
    if candidate in CANFD_CAR:
      ret.enableBsm = 0x1e5 in fingerprint[CAN.ECAN]

      if 0x1fa in fingerprint[CAN.ECAN]:
        ret.spFlags |= HyundaiFlagsSP.SP_NAV_MSG.value
      if Params().get("DongleId", encoding='utf8') in ("012c95f06918eca4", "68d6a96e703c00c9"):
        ret.spFlags |= HyundaiFlagsSP.SP_UPSTREAM_TACO.value
    else:
      ret.enableBsm = 0x58b in fingerprint[0]

      if 0x544 in fingerprint[0]:
        ret.spFlags |= HyundaiFlagsSP.SP_NAV_MSG.value

      if ret.flags & HyundaiFlags.MANDO_RADAR and ret.radarUnavailable:
        ret.spFlags |= HyundaiFlagsSP.SP_RADAR_TRACKS.value
        ret.radarUnavailable = False

    # *** panda safety config ***
    if candidate in CANFD_CAR:
      cfgs = [get_safety_config(car.CarParams.SafetyModel.hyundaiCanfd), ]
      if CAN.ECAN >= 4:
        cfgs.insert(0, get_safety_config(car.CarParams.SafetyModel.noOutput))
      ret.safetyConfigs = cfgs

      if ret.flags & HyundaiFlags.CANFD_HDA2:
        ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_CANFD_HDA2
        if ret.flags & HyundaiFlags.CANFD_HDA2_ALT_STEERING:
          ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_CANFD_HDA2_ALT_STEERING
      if ret.flags & HyundaiFlags.CANFD_ALT_BUTTONS:
        ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_CANFD_ALT_BUTTONS
      if ret.flags & HyundaiFlags.CANFD_CAMERA_SCC:
        ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_CAMERA_SCC
      if ret.spFlags & HyundaiFlagsSP.SP_UPSTREAM_TACO:
        ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_UPSTREAM_TACO
    else:
      if candidate in LEGACY_SAFETY_MODE_CAR:
        # these cars require a special panda safety mode due to missing counters and checksums in the messages
        ret.safetyConfigs = [get_safety_config(car.CarParams.SafetyModel.hyundaiLegacy)]
      else:
        ret.safetyConfigs = [get_safety_config(car.CarParams.SafetyModel.hyundai, 0)]

      if candidate in CAMERA_SCC_CAR:
        ret.safetyConfigs[0].safetyParam |= Panda.FLAG_HYUNDAI_CAMERA_SCC

      if ret.spFlags & HyundaiFlagsSP.SP_ENHANCED_SCC:
        ret.safetyConfigs[0].safetyParam |= Panda.FLAG_HYUNDAI_ESCC
      if 0x391 in fingerprint[0]:
        ret.spFlags |= HyundaiFlagsSP.SP_CAN_LFA_BTN.value
        ret.safetyConfigs[0].safetyParam |= Panda.FLAG_HYUNDAI_LFA_BTN
      if candidate in NON_SCC_CAR:
        ret.safetyConfigs[0].safetyParam |= Panda.FLAG_HYUNDAI_NON_SCC

    if ret.openpilotLongitudinalControl:
      ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_LONG
    if ret.flags & HyundaiFlags.HYBRID:
      ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_HYBRID_GAS
    elif ret.flags & HyundaiFlags.EV:
      ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_EV_GAS

    if candidate in (CAR.HYUNDAI_KONA, CAR.HYUNDAI_KONA_EV, CAR.HYUNDAI_KONA_HEV, CAR.HYUNDAI_KONA_EV_2022, CAR.HYUNDAI_KONA_NON_SCC):
      ret.flags |= HyundaiFlags.ALT_LIMITS.value
      ret.safetyConfigs[-1].safetyParam |= Panda.FLAG_HYUNDAI_ALT_LIMITS

    ret.centerToFront = ret.wheelbase * 0.4

    # Detect smartMDPS, which bypasses EPS low speed lockout, allowing sunnypilot to send steering commands down to 0
    if 0x2AA in fingerprint[0]:
      ret.minSteerSpeed = 0.

    #if Params().get_bool("HkgSmoothStop"):
    #  ret.vEgoStopping = 0.1

    print(f"[interface]accel_personality: {accel_personality}, startAccel: {ret.startAccel}, vEgoStopping: {ret.vEgoStopping}")

    return ret

  @staticmethod
  def init(CP, logcan, sendcan):
    if CP.openpilotLongitudinalControl and not ((CP.flags & HyundaiFlags.CANFD_CAMERA_SCC.value) or (CP.spFlags & HyundaiFlagsSP.SP_ENHANCED_SCC)) and \
      CP.carFingerprint not in CAMERA_SCC_CAR:
      addr, bus = 0x7d0, CanBus(CP).ECAN if CP.carFingerprint in CANFD_CAR else 0
      if CP.flags & HyundaiFlags.CANFD_HDA2.value:
        addr, bus = 0x730, CanBus(CP).ECAN
      disable_ecu(logcan, sendcan, bus=bus, addr=addr, com_cont_req=b'\x28\x83\x01')

    # for blinkers
    if CP.flags & HyundaiFlags.ENABLE_BLINKERS:
      disable_ecu(logcan, sendcan, bus=CanBus(CP).ECAN, addr=0x7B1, com_cont_req=b'\x28\x83\x01')

    # for enabling radar tracks on startup
    # some CAN platforms are able to enable radar tracks config at the radar ECU,
    # but the config is reset after ignition cycle
    if CP.spFlags & HyundaiFlagsSP.SP_RADAR_TRACKS:
      enable_radar_tracks(logcan, sendcan, bus=0, addr=0x7d0, config_data_id=b'\x01\x42')

  def _update(self, c):
    #更新车辆当前状态，如速度、刹车、巡航状态等，即调用carstate.py中的update函数
    #这里的self.CS定义在interfaces.py的类CarInterfaceBase中，里面有一个self.CS = CarState(CP)
    ret = self.CS.update(self.cp, self.cp_cam)

    #更新一些特定参数，比如巡航速度或开关状态
    self.sp_update_params()

    #比较当前按钮状态和前一帧的状态，识别出按钮事件（如按下/释放某个按钮），按键不同时才会创建按键事件
    buttonEvents = create_button_events(self.CS.cruise_buttons[-1], self.CS.prev_cruise_buttons, BUTTONS_DICT)

    #判断当前是否应该启用自定义的巡航控制（非 PCM 控制），函数在interfaces.py中
    #主要是根据按键的类型判断巡航开启的逻辑，返回变量acc_enabled
    self.CS.accEnabled = self.get_sp_v_cruise_non_pcm_state(ret, self.CS.accEnabled,
                                                            buttonEvents, c.vCruise)

    #self.mads_main_toggle为参数"MadsCruiseMain"，默认为1，通过菜单设置，表示是否使用主巡航开关使MADS
    #如果未开启主开关，就强制关闭 mads_enabled
    self.CS.mads_enabled = False if not self.mads_main_toggle else self.CS.mads_enabled

    #CruiseSwState按键类型
    #0：无操作（No button pressed）
    #1：取消巡航（Cancel）
    #2：设置巡航/减速（Set/Decel）
    #3：加速（Accel）
    #4：恢复（Resume）
    #如果巡航功能可用，执行以下逻辑
    if ret.cruiseState.available: #来自carstate.py: cp.vl["TCS13"]["ACCEnable"] == 0 and self.mainEnabled
      if not self.CP.pcmCruiseSpeed:
        if self.CS.prev_main_buttons == 1: #main_buttons上次为1，这次不为1，则self.CS.accEnabled = True
          if self.CS.main_buttons[-1] != 1:
            self.CS.accEnabled = True
          elif self.CS.prev_cruise_buttons == 4: #cruise_buttons按键上次为4，这次不为4，则self.accEnabled = True
            if self.CS.cruise_buttons[-1] != 4:
              self.accEnabled = True
      if self.enable_mads:
        if not self.CS.prev_mads_enabled and self.CS.mads_enabled:
          self.CS.madsEnabled = True
        if self.CS.prev_lfa_enabled != 1 and self.CS.lfa_enabled == 1:
          self.CS.madsEnabled = not self.CS.madsEnabled
        self.CS.madsEnabled = self.get_acc_mads(ret.cruiseState.enabled, self.CS.accEnabled, self.CS.madsEnabled)
    else:
      self.CS.madsEnabled = False

    #处理取消巡航的逻辑,非原车巡航逻辑
    if not self.CP.pcmCruise or not self.CP.pcmCruiseSpeed:
      if not self.CP.pcmCruise:
        if any(b.type == ButtonType.cancel for b in buttonEvents): #取消按钮被按下，或 cruise 被关闭 → 禁用 MADS 和 ACC
          self.CS.madsEnabled, self.CS.accEnabled = self.get_sp_cancel_cruise_state(self.CS.madsEnabled)
      if not self.CP.pcmCruiseSpeed:
        if not ret.cruiseState.enabled:
          self.CS.madsEnabled, self.CS.accEnabled = self.get_sp_cancel_cruise_state(self.CS.madsEnabled)
    if self.get_sp_pedal_disengage(ret): #获取踩油门/刹车等解除操作
      self.CS.madsEnabled, self.CS.accEnabled = self.get_sp_cancel_cruise_state(self.CS.madsEnabled)
      #踩刹车时会在这里把cruiseState.enabled设置为了False
      ret.cruiseState.enabled = False if self.CP.pcmCruise else self.CS.accEnabled #如果是原厂巡航，则清除ret.cruiseState.enabled

    #sp的一些状态的处理，根据配置判断是否开启 MADS、ACC/判断油门刹车是否触发退出/判断是否满足变道、横向控制条件
    #判断是否允许启用控制系统（安全带、档位等）/返回更新后的 cs_out 和 CS，供主控逻辑使用
    #在函数里有控制cs_out.cruiseState.enabled的操作，根据CS.accEnabled决定cs_out.cruiseState.enabled的值
    #当检测到任何一个踏板解除控制状态时，如果MADS使能了，会设置CS.disengageByBrake = True
    ret, self.CS = self.get_sp_common_state(ret, self.CS, gap_button=(self.CS.cruise_buttons[-1] == 3))

    # MADS BUTTON 生成模拟 MADS 按钮事件
    if self.CS.out.madsEnabled != self.CS.madsEnabled:
      if self.mads_event_lock:
        buttonEvents.append(create_mads_event(self.mads_event_lock))
        self.mads_event_lock = False
    else:
      if not self.mads_event_lock:
        buttonEvents.append(create_mads_event(self.mads_event_lock))
        self.mads_event_lock = True

    #保存 buttonEvents 到 ret
    ret.buttonEvents = buttonEvents

    # On some newer model years, the CANCEL button acts as a pause/resume button based on the PCM state
    # To avoid re-engaging when openpilot cancels, check user engagement intention via buttons
    # Main button also can trigger an engagement on these cars
    allow_enable = any(btn in ENABLE_BUTTONS for btn in self.CS.cruise_buttons) or any(self.CS.main_buttons)

    #通用的事件处理
    #在这个函数里，如果检测到刹车，并且MADS开启，则设置cs_out.disengageByBrake = True
    # 在这个函数里，并且当前在巡航状态，则会添加事件events.add(EventName.brakeHold)
    events = self.create_common_events(ret, c, extra_gears=[GearShifter.sport, GearShifter.low, GearShifter.manumatic],
                                       pcm_enable=False, allow_enable=allow_enable)

    #SP的特有事件处理，主要是刹车保持之后恢复的时候MADS的恢复，或者用按键进行恢复的处理
    #函数会有几个事件处理，events.add(EventName.buttonCancel)/events.add(EventName.manualLongitudinalRequired)
    #events.add(EventName.manualSteeringRequired)
    #如果是enable_pressed，会触发events.add(EventName.silentButtonEnable)或者events.add(EventName.buttonEnable)
    events, ret = self.create_sp_events(self.CS, ret, events, main_enabled=True, allow_enable=allow_enable)

    # low speed steer alert hysteresis logic (only for cars with steer cut off above 10 m/s)
    # 当车速过低且转向控制被限制，触发低速提醒
    if ret.vEgo < (self.CP.minSteerSpeed + 2.) and self.CP.minSteerSpeed > 10.:
      self.low_speed_alert = True
    if ret.vEgo > (self.CP.minSteerSpeed + 4.):
      self.low_speed_alert = False
    if self.low_speed_alert and self.CS.madsEnabled:
      events.add(car.CarEvent.EventName.belowSteerSpeed)

    # 使用原厂巡航时，构造一个用于自定义巡航速度调节的 CustomStockLong 消息
    # 这里的self.CC也是在interfaces.py中定义，self.CC: CarControllerBase = CarController(dbc_name, CP, self.VM)
    ret.customStockLong = self.CS.update_custom_stock_long(self.CC.cruise_button, self.CC.final_speed_kph,
                                                           self.CC.target_speed, self.CC.v_set_dis,
                                                           self.CC.speed_diff, self.CC.button_type)

    ret.events = events.to_msg()

    return ret
