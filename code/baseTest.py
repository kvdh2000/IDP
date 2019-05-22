from bakje_new import FindCon # import our own script
from barcode_video import QRScanner # import our own script
from BlueVision import BlueVision
from EggVision import EggVision
from picamera.array import PiRGBArray
from picamera import PiCamera
import serial
import asyncio
import cv2
import argparse
import time
import imutils

ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()

# setup the picamera, this way we don't have to 
# close it between switching scripts
cam = PiCamera()
cam.resolution = (640, 480)
cam.framerate = 32
rawCapture = PiRGBArray(cam, size=(640, 480))
#time.sleep(.25) # sleep to give the camera a short time to startup

bakje = False

# initialize variable to request 
# the method from the other script file
scan = QRScanner()
find = FindCon()
egg = EggVision()
blue = BlueVision()
found =[False, False, False]
# initialize variable for serial communication
port = '/dev/ttyAMA0' # Raspberry port which connects to the arduino
ard = serial.Serial(port,9600,timeout=5)
time.sleep(.5) # wait for Arduino and camera

# drive around
for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
	#if(bakje == False):
		#func = find.FindBakje(frame)
	
	if(not found[0]):
		found[0] = egg.findEgg(frame)
	elif(not found[1]):
		found[1] =find.FindBakje(frame)
	elif(not found[2]):
		found[2] = scan.SearchQR(argName.a, frame) # scans for the QRCode
	else:
		print("got all")
		#SendMessage('marm')
		
	rawCapture.truncate(0)
	#loop.run_until_complete(GetArduino())
	cv2.waitKey(10)

cv2.destroyAllWindows()
cam.close()