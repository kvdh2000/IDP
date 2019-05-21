from bakje_new import FindCon # import our own script
from barcode_video import QRScanner # import our own script
from picamera.array import PiRGBArray
from picamera import PiCamera
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
time.sleep(.25) # sleep to give the camera a short time to startup

bakje = False

# initialize variable to request 
# the method from the other script file
scan = QRScanner()
find = FindCon()

#drive around
for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
	
	if(bakje == False):
		func = find.FindBakje(frame)

	if(func == True):
		bakje = True
		scan.SearchQR(argName.a, frame) # scans for the QRCode
	rawCapture.truncate(0)
	cv2.waitKey(10)

cv2.destroyAllWindows()
cam.close()