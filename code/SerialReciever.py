import serial


class SerialReciever:


    def __init__(self, comPort, baudrate = 9600):
        # public data vars
        self.armMoved = False
        self.boolLook = False
        self.setupComplete = False

        # private vars
        self._buffer = ""
        self._serial_port = serial.Serial(comPort, baudrate, timeout=5)
        self._cmd_sep = "|"

    # Method for sending commands to the arduino
    # Command can be max 5 characters long
    def send_command(self, command):
        if (command != None):
            command += "|"
            print("Python value sent: ")
            print(command)
            self._serial_port.write(command.encode())

    def recieve(self):
        # dumps this into buffer
        self._buffer += serial.read(serial.inWaiting()).decode("UTF-8")
        # looks for cmd sep
        cmd_sep_idx = self._buffer.find(self._cmd_sep)
        if cmd_sep_idx > -1:
            # if cmd sep found
            cmd = self._buffer[:cmd_sep_idx]# cuts all before sep
            self._buffer = self._buffer[cmd_sep_idx+1:]# cuts all after sep

            # parses cmd
            if cmd.find("Arm mov") > -1:# check if the arm stopped moving
                self.armMoved = True
                self.boolLook = False
            elif cmd.find("MEGA start") > -1:# check if the arduino booted
                self.setupComplete = True


