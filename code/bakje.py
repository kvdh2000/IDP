import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera
from imutils.video import VideoStream
import imutils
from pyzbar import pyzbar
import numpy as np
import math
import time

#cam = VideoStream(usePiCamera=True).start()

def FindBakje():
    cam = PiCamera()
    cam.resolution = (640, 480)
    cam.framerate = 32
    rawCapture = PiRGBArray(cam, size=(640, 480))
    time.sleep(.2)

    lowerBound = np.array([0, 100, 100])
    upperBound = np.array([255, 255, 255])

    kernelOpen = np.ones((5, 5))
    kernelClose = np.ones((20, 20))

    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
        img =frame.array
        img = cv2.resize(img, (340, 220))

        # convert BGR to HSV
        imgHSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        # create the Mask
        mask = cv2.inRange(imgHSV, lowerBound, upperBound)
        # morphology
        maskOpen = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernelOpen)
        maskClose = cv2.morphologyEx(maskOpen, cv2.MORPH_CLOSE, kernelClose)

        maskFinal = maskClose
        __,conts ,h= cv2.findContours(
            maskFinal.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        cv2.drawContours(img, conts, -1, (255, 0, 0), 3)
        for i in range(len(conts)):
            x, y, w, h = cv2.boundingRect(conts[i])
            cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)

        cv2.imshow("cam", img)
        rawCapture.truncate(0)
        #if(len(conts) > 0):
            #print("STAAAWP")
            #return true
            
    cv2.destroyAllWindows()
    cam.stop()
