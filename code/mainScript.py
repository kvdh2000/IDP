# import our own scripts
from ConVision import FindCon
from QRVision import QRScanner
from BlueVision import BlueVision
from EggVision import EggVision

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
found =[False, False, False]
armMoved = False

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
        if(re.search('.(Arm mov).', str(msg))):
            print("found")
            global armMoved
            armMoved = True
    
    
# Method for sending commands to the arduino
# Command can be max 5 characters long
def SendMessage(command):
    if(command != None):
        print("Python value sent: ")
        print(command)
        ard.write(command.encode())

def main():
    loop = asyncio.get_event_loop()
    # drive around and use the camera to search for objects
    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
        
        loop.run_until_complete(GetArduino())
        
        # follow these steps in order
        if(not found[0]):
            found[0] = egg.FindEgg(frame) # find the egg
            if found[0]:
                SendMessage('marm|') # send a command to the arduino over Serial
        if armMoved:
            if(not found[1]):
                found[1] = find.FindContainer(frame) # scans for the QRCode
            elif(not found[2]):
                found[2] = scan.SearchQR(argName.a, frame) # find the container
                if found[2]:
                    SendMessage("marm|")
            else:
                print("got all")
        
        rawCapture.truncate(0) # ready the camera for a new frame to be analysed
        cv2.waitKey(10)

    cv2.destroyAllWindows()
    cam.close()


if __name__== "__main__":
    main()
