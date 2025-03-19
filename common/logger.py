import os
import logging
import time

class Logger:
    def __init__(self, log_dir="/data/log", debug_mode=False):
        self.log_dir = log_dir  # OpenPilot 设备可写目录
        self.debug_mode = debug_mode  # 是否打印到终端
        self.logger = None  # 延迟创建日志记录器
        self.log_file = None  # 延迟设置日志文件

    def _initialize_logger(self):
        """创建日志文件（仅在 log() 第一次调用时执行）"""
        if self.logger is None:
            os.makedirs(self.log_dir, exist_ok=True)  # 确保日志目录存在
            date_str = time.strftime("%Y-%m-%d")
            self.log_file = os.path.join(self.log_dir, f"debug_{date_str}.log")

            # 设置 Logger
            self.logger = logging.getLogger("openpilot_logger")
            self.logger.setLevel(logging.DEBUG)

            # 避免重复添加 Handler
            if not self.logger.handlers:
                file_handler = logging.FileHandler(self.log_file, encoding="utf-8")
                formatter = logging.Formatter("%(asctime)s - %(message)s")
                file_handler.setFormatter(formatter)
                self.logger.addHandler(file_handler)

            self.logger.debug("===== OpenPilot Logger Initialized =====")

    def log(self, msg, **kwargs):
        """记录日志，首次调用时才创建日志文件"""
        if self.logger is None:
            self._initialize_logger()  # 第一次调用 log() 时创建日志文件

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
