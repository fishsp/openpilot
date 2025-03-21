import os
import logging
import time
from openpilot.common.params import Params

class Logger:
    def __init__(self, log_dir="/data/log", debug_mode=False):
        self.log_dir = log_dir  # OpenPilot 设备可写目录
        self.debug_mode = debug_mode  # 是否打印到终端
        self.logger = None  # 延迟创建日志记录器
        self.log_file = None  # 延迟设置日志文件
        self.current_date = None  # 记录日志文件创建时的日期

        self.param_s = Params()
        self.lkas_toggle = self.param_s.get_bool("LkasToggle")
        self._initialize_logger()  # 立即初始化日志系统
        if self.lkas_toggle:
            self.logger.debug("lkas toggle.")
        else:
            self.logger.debug("lkas not toggle.")

    def _initialize_logger(self):
        """创建日志文件或在日期变更时重新创建"""
        os.makedirs(self.log_dir, exist_ok=True)
        date_str = time.strftime("%Y-%m-%d")
        new_log_file = os.path.join(self.log_dir, f"debug_{date_str}.log")

        # 如果是首次创建，或者日期变化，或者日志文件被删除，则重新初始化
        if self.logger is None or self.current_date != date_str or not os.path.exists(new_log_file):
            self.current_date = date_str
            self.log_file = new_log_file

            # 清理旧的 Logger
            if self.logger:
                for handler in self.logger.handlers[:]:
                    self.logger.removeHandler(handler)
                    handler.close()

            # 创建新的 Logger
            self.logger = logging.getLogger("openpilot_logger")
            self.logger.setLevel(logging.DEBUG)

            file_handler = logging.FileHandler(self.log_file, encoding="utf-8")
            formatter = logging.Formatter("%(asctime)s - %(message)s")
            file_handler.setFormatter(formatter)
            self.logger.addHandler(file_handler)

            self.logger.debug("===== OpenPilot Logger Initialized =====")

    def log(self, msg, **kwargs):
        """记录日志，每次检查日期是否变化，必要时重新创建日志文件"""
        self._initialize_logger()  # 每次调用 log() 时检查日期

        # 生成日志内容
        if kwargs:
            formatted_values = ", ".join(
                f"{k}={float(v):.2f}" if isinstance(v, (int, float)) else f"{k}={v}"
                for k, v in kwargs.items()
            )
            log_msg = f"{msg}: {formatted_values}"
        else:
            log_msg = msg

        if self.debug_mode:
            print(log_msg)  # 仅在 debug_mode=True 时打印到终端

        self.logger.debug(log_msg)  # 记录到日志文件

# **全局 Logger 实例**
logger = Logger(debug_mode=True)  # 在开发模式下启用终端输出
