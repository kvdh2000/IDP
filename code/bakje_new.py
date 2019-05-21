import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera
import imutils
import numpy as np
import math
import time

class FindCon:
    def __init__(self):
        pass
    
    # find shapes and see if it resembles the container
    def FindBakje(self):
		
        foundBakje = False
        # setup the picamera
        cam = PiCamera()
        cam.resolution = (640, 480)
        cam.framerate = 32
        rawCapture = PiRGBArray(cam, size=(640, 480))
        time.sleep(.25) # sleep to give the camera a short time to startup
        
        # set color values to check for bright colors
        lowerBound = np.array([0, 100, 100])
        upperBound = np.array([255, 255, 255])

        kernelOpen = np.ones((5, 5))
        kernelClose = np.ones((20, 20))

        # for every frame seen by the cmaera see if there's a shape we recognize
        for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
            
            img = frame.array # set our working screen/ image
            img = cv2.resize(img, (340, 220))

            # convert BGR to HSV
            imgHSV = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
            # create the Mask
            mask = cv2.inRange(imgHSV, lowerBound, upperBound)
            # morphology
            maskOpen = cv2.morphologyEx(mask, cv2.MORPH_OPEN, kernelOpen)
            maskClose = cv2.morphologyEx(maskOpen, cv2.MORPH_CLOSE, kernelClose)

            maskFinal = maskClose
            __,conts, h = cv2.findContours(
                maskFinal.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

            cv2.drawContours(img, conts, 0, (255, 0, 0), 3)
            for i in range(len(conts)):
                area = cv2.contourArea(conts[i])
                print(area)
                if(area > 6500):
                    x, y, w, h = cv2.boundingRect(conts[i])
                    cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
                    foundBakje = True
                    break

            cv2.imshow("maskClose", maskClose)
            cv2.imshow("maskOpen", maskOpen)
            cv2.imshow("mask", mask)
            cv2.imshow("cam", img)

            if(foundBakje == True):
                break
            rawCapture.truncate(0)
            cv2.waitKey(10)

        cv2.destroyAllWindows()
        cam.close()
        return True
