import serial
import time

port = 'COM3'
command = None

ard = serial.Serial(port,9600,timeout=5)
time.sleep(2) # wait for Arduino

sendCommand = True
#command can be max 5 characters long
command = 'back'

while sendCommand:
    if(command != None):
        print ("Python value sent: ")
        print (command)
        ard.write(command.encode())
        time.sleep(3)

    msg = ard.read(ard.inWaiting())
    if(msg != None):
        print (msg.decode('utf-8'))

        
exit()
