# import our own scripts
from ConVision import FindCon
from QRVision import QRScanner
from BlueVision import BlueVision
from EggVision import EggVision
from LineVision import LineVision

import argparse
import asyncio
import cv2
import imutils
from picamera import PiCamera
from picamera.array import PiRGBArray
import serial
import time
import re

# used for grabbing an argument in this case 
# we use it for grabbing which location we need 
# to deliver the egg to
ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()

# setup the picamera, this way we don't have to 
# close it between switching scripts
cam = PiCamera()
cam.resolution = (640, 480)
cam.framerate = 32
rawCapture = PiRGBArray(cam, size=(640, 480))

# initialize variable to request 
# the method from the other script file
scan = QRScanner()
find = FindCon()
egg = EggVision()
blue = BlueVision()
line = LineVision()
found =[False, False, False]

# use this to check if the arm stopped moving
# so we can continue with the vision stuff
armMoved = False
boolLook = False 
setupComplete = False

# initialize variable for serial communication
port = '/dev/ttyACM0' # Raspberry port which connects to the arduino
baud = 9600 # set arduino baudrate
ard = serial.Serial(port,baud,timeout=5)
msg = ''
time.sleep(.5) # wait for Arduino and camera to start up

# Run this method async so we can grab the replies from the
# arduino
async def GetArduino():
    msg = (ard.read(ard.inWaiting()))
    if(msg != None):
        print(str(msg.decode('utf-8', errors="ignore")))
        if(msg == "b''"):
            print(msg)
        if(re.search('.(Arm mov).', str(msg))): # check if the arm stopped moving
            print("found")
            global boolLook
            global armMoved # add 'global' so it doesn't create a local variable named armMoved
            armMoved = True
            boolLook = False
        elif(re.search('.(MEGA start).', str(msg))): # check if the arduino booted
            global setupComplete
            setupComplete = True
    
# Method for sending commands to the arduino
# Command can be max 5 characters long
def SendMessage(command):
    if(command != None):
        print("Python value sent: ")
        print(command)
        ard.write(command.encode())

def main():
    loop = asyncio.get_event_loop()
    global boolLook
    # drive around and use the camera to search for objects
    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):

        loop.run_until_complete(GetArduino())
        if not boolLook:
            SendMessage('look|')
            boolLook = True

        # follow these steps in order
        # wait till the arduino is ready to receive commands
        if setupComplete: 
            if(not found[0]):
                found[0] = egg.FindEgg(frame) # find the egg
                if found[0]:
                    SendMessage('marm|') # send a command to the arduino over Serial
        if armMoved:        
            if(not found[1]):
                found[1] = scan.SearchQR(argName.a, frame) # scans for the QRCode
            elif(not found[2]):
                found[2] = find.FindContainer(frame) # find the container
                if found[2]:
                    SendMessage("marm|")
            else:
                print("got all")
        
        #line.FindLine(frame) # run continuesly to check if we're still in the playing field
        
        egg.FindEgg(frame) # try finding the distance

        rawCapture.truncate(0) # ready the camera for a new frame to be analysed
        cv2.waitKey(10)

    cv2.destroyAllWindows()
    cam.close()


if __name__== "__main__":
    main()
