import os
import logging
import time

class Logger:
    def __init__(self, log_filename="debug.log"):
        log_dir = "/data/log"  # OpenPilot 设备通常可写
        os.makedirs(log_dir, exist_ok=True)  # 确保日志目录存在
        self.log_file = os.path.join(log_dir, log_filename)

        # 获取 OpenPilot 兼容的 Logger
        self.logger = logging.getLogger("openpilot_logger")
        self.logger.setLevel(logging.DEBUG)

        # 避免重复添加 Handler
        if not self.logger.handlers:
            file_handler = logging.FileHandler(self.log_file, encoding="utf-8")
            formatter = logging.Formatter("%(asctime)s - %(message)s")
            file_handler.setFormatter(formatter)
            self.logger.addHandler(file_handler)

    def log(self, msg, **kwargs):
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")

        if kwargs:  # 格式化变量
            formatted_values = ", ".join(
                f"{k}={float(v):.2f}" if isinstance(v, (int, float)) else f"{k}={v}"
                for k, v in kwargs.items()
            )
            log_msg = f"[{timestamp}] {msg}: {formatted_values}"
        else:
            log_msg = f"[{timestamp}] {msg}"

        print(log_msg)  # 终端输出（适用于 SSH 调试）
        self.logger.debug(log_msg)  # 记录到日志文件

# **全局 Logger 实例**
logger = Logger()
