import os
import logging
import time

class Logger:
    def __init__(self, log_filename="debug.log"):
        log_dir = "/data/media/0/openpilot_logs"
        os.makedirs(log_dir, exist_ok=True)
        self.log_file = os.path.join(log_dir, log_filename)
        logging.basicConfig(filename=self.log_file, level=logging.DEBUG, format="%(asctime)s - %(message)s")

    def log(self, msg, **kwargs):
        timestamp = time.strftime("%Y-%m-%d %H:%M:%S")

        if kwargs:  # 如果有变量，格式化它们
            formatted_values = ", ".join(
                f"{k}={float(v):.2f}" if isinstance(v, (int, float)) else f"{k}={v}"
                for k, v in kwargs.items()
            )
            log_msg = f"[{timestamp}] {msg}: {formatted_values}"
        else:  # 只传递了字符串
            log_msg = f"[{timestamp}] {msg}"

        print(log_msg)
        logging.debug(log_msg)

# **全局 Logger 实例**
logger = Logger()
