# import our own scripts
from bakje_new import FindCon
from barcode_video import QRScanner
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

# initialize variable for serial communication
port = '/dev/ttyACM0' # Raspberry port which connects to the arduino
baud = 9600 # set arduino baudrate
ard = serial.Serial(port,baud,timeout=5)
time.sleep(.5) # wait for Arduino and camera to start up

# Run this method async so we can grab the replies from the
# arduino
async def GetArduino():
    msg = (ard.read(ard.inWaiting()))
    if(msg != None):
        print(str(msg.decode('utf-8', errors="ignore")))
    print(msg)

# Method for sending commands to the arduino
# Command can be max 5 characters long
def SendMessage(command):
    loop = asyncio.get_event_loop()
    if(command != None):
        print("Python value sent: ")
        print(command)
        ard.write(command.encode())
    loop.run_until_complete(GetArduino())

# drive around and use the camera to search for objects
for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
	
	# follow these steps in order
	if(not found[0]):
		found[0] = egg.findEgg(frame) # find the egg
	elif(not found[1]):
		found[1] =find.FindBakje(frame) # find the container
	elif(not found[2]):
		found[2] = scan.SearchQR(argName.a, frame) # scans for the QRCode
	else:
		print("got all")
		SendMessage('marm') # send a command to the arduino over Serial
		
	rawCapture.truncate(0) # ready the camera for a new frame to be analysed
	cv2.waitKey(10)

cv2.destroyAllWindows()
cam.close()