from systemd import journal


class Logging:
    def __init__(self, module_name):
        self.module_name = "idp_" + module_name

    def log_msg(self, info):
        self.__log(info, "info")

    def log_error(self, err):
        self.__log(err, "err")

    def log_warning(self, warn):
        self.__log(warn, "warn")

    def __log(self, msg, _type):
        # todo mongodb logging
        journal.send(_type + "=" + msg, CODE_FILE=self.module_name)
        print("module {} {}: {}".format(self.module_name, _type, msg))
