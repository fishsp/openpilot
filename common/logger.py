import os
import logging

class Logger:
    def __init__(self, log_filename="debug.log", debug_mode=False):
        log_dir = "/data/log"  # OpenPilot 设备通常可写
        os.makedirs(log_dir, exist_ok=True)  # 确保日志目录存在
        self.log_file = os.path.join(log_dir, log_filename)
        self.debug_mode = debug_mode  # 控制是否打印到终端

        # 获取 OpenPilot 兼容的 Logger
        self.logger = logging.getLogger("openpilot_logger")
        self.logger.setLevel(logging.DEBUG)

        # 避免重复添加 Handler
        if not self.logger.handlers:
            file_handler = logging.FileHandler(self.log_file, encoding="utf-8")
            formatter = logging.Formatter("%(asctime)s - %(message)s")
            file_handler.setFormatter(formatter)
            self.logger.addHandler(file_handler)

        # **强制创建日志文件，调用 log() 方法**
        self.log("===== OpenPilot Logger Initialized =====")

    def log(self, msg, **kwargs):
        if kwargs:  # 格式化变量
            formatted_values = ", ".join(
                f"{k}={float(v):.2f}" if isinstance(v, (int, float)) else f"{k}={v}"
                for k, v in kwargs.items()
            )
            log_msg = f"{msg}: {formatted_values}"
        else:
            log_msg = msg

        if self.debug_mode:
            print(log_msg)  # 仅在 debug_mode=True 时打印

        self.logger.debug(log_msg)  # 记录到日志文件

# **全局 Logger 实例**
logger = Logger(debug_mode=True)  # 在开发模式下启用终端输出
