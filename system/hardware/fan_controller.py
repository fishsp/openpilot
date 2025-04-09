#!/usr/bin/env python3
from abc import ABC, abstractmethod

from openpilot.common.realtime import DT_HW
from openpilot.common.numpy_fast import interp
from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.controls.lib.pid import PIDController
from openpilot.common.params import Params

class BaseFanController(ABC):
  @abstractmethod
  def update(self, cur_temp: float, ignition: bool) -> int:
    pass


class TiciFanController(BaseFanController):
  def __init__(self) -> None:
    super().__init__()
    cloudlog.info("Setting up TICI fan handler")

    self.last_ignition = False
    self.controller = PIDController(k_p=0, k_i=4e-3, k_f=1, rate=(1 / DT_HW))

    self.param_s = Params()
    self.fan_power = self.param_s.get_bool("StockLongToyota")
    self.frame = 0

  def update(self, cur_temp: float, ignition: bool) -> int:
    self.controller.neg_limit = -(100 if ignition else 30)
    self.controller.pos_limit = -(30 if ignition else 0)

    if ignition != self.last_ignition:
      self.controller.reset()

    if self.frame % 200 == 0:
      self.fan_power = self.param_s.get_bool("StockLongToyota")
    self.frame += 1

    if not self.fan_power:
      error = 70 - cur_temp
      fan_pwr_out = -int(self.controller.update(
        error=error,
        feedforward=interp(cur_temp, [60.0, 100.0], [0, -100])
      ))
    else:
      fan_pwr_out = int(interp(cur_temp, [60.0, 80.0], [0, 80]))
      fan_pwr_out = max(0, min(80, fan_pwr_out))

    self.last_ignition = ignition

    return fan_pwr_out

