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
port = '/dev/ttyACM0' # Raspberry port which connects to the arduino
baud = 9600 # set arduino baudrate
ard = serial.Serial(port,baud,timeout=5)
time.sleep(.5) # wait for Arduino and camera

# Run this method async so we can grab the replies from the 
# arduino
async def GetArduino():
    msg = (ard.read(ard.inWaiting()))
    if(msg != None):
        print(str(msg.decode('utf-8')))
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
		SendMessage('marm')
		
	rawCapture.truncate(0)
	#loop.run_until_complete(GetArduino())
	cv2.waitKey(10)

cv2.destroyAllWindows()
cam.close()