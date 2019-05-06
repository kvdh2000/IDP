import serial #used for the arduino serial port stuff
import sys #used for arguments over cmd

#ser = serial.Serial('dev/ttyACM0', '9600')
ser = serial.Serial()
ser.port = 'COM4'
ser.baudrate = 9600
ser.open()

command = 'TEST'

ser.write(command)