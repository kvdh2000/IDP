import abc
from abc import abstractmethod
import threading
from idp_deamon.logging import Logging
from idp_deamon.helpers import Pi_Serial_Msg


class Abs_Module(threading.Thread):
    __metaclass__ = abc.ABCMeta

    def __init__(self, serial_recieve_queue, serial_send_queue, module_name):
        super().__init__()
        self.serial_recieve_queue = serial_recieve_queue
        self.serial_send_queue = serial_send_queue
        self.module_name = module_name
        self.logging = Logging(module_name)

    def serial_get_nxt(self):
        if not self.serial_recieve_queue.empty():
            return self.serial_recieve_queue.get_nowait()
        else:
            return None

    def send_serial(self, msg):
        self.serial_send_queue.put(Pi_Serial_Msg(self.module_name, msg))

    @abstractmethod
    def run(self):
        pass
