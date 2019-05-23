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
    def FindContainer(self, frame):
		
        maxoffcenter = 10
        
        # set color values to check for bright colors
        lowerBound = np.array([0, 100, 100])
        upperBound = np.array([255, 255, 255])

        kernelOpen = np.ones((5, 5))
        kernelClose = np.ones((20, 20))
        
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
                currentContour = conts[i]
                center = cv2.moments(currentContour)
                cx = int(center['m10']/center['m00'])
                cy = int(center['m01']/center['m00'])
                if cx < ((len(img[1]) / 2) - (len(img[1]) * maxoffcenter * 0.005)):
                    print ("Go left")
                elif cx > ((len(img[1]) / 2) + (len(img[1]) * maxoffcenter * 0.005)):
                    print ("Go right")
                else:
                    print ("Good enough")
                    return True

                x, y, w, h = cv2.boundingRect(conts[i])
                cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
                break
                
        cv2.imshow("cam", img)
