from bakje_new import FindCon # import our own script
from barcode_video import QRScanner # import our own script
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

# initialize variable for serial communication
port = '/dev/ttyAMA0' # Raspberry port which connects to the arduino
ard = serial.Serial(port,9600,timeout=5)
time.sleep(.5) # wait for Arduino and camera
loop = asyncio.get_event_loop()

async def GetArduino():
	msg= (ard.read(ard.inWaiting()))
	if(msg != None):
		print(str(msg.decode('utf-8')))

def SendMessage(command):
    
    if(len(command) > 0):
        print("Python value sent: ")
        print(command)
        ard.write(command.encode())
        #time.sleep(3)


# drive around
for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
	#if(bakje == False):
		#func = find.FindBakje(frame)
	
	func = find.FindBakje(frame)
	func1 = scan.SearchQR(argName.a, frame)

	if(func and func1):
		#bakje = True
		#scan.SearchQR(argName.a, frame) # scans for the QRCode
		SendMessage('marm')

	rawCapture.truncate(0)
	loop.run_until_complete(GetArduino())
	cv2.waitKey(10)

cv2.destroyAllWindows()
cam.close()