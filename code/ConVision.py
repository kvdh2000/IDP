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
    
    def icanShowYouTheWorld(self, image, c):
        # initialize the known object width, which in this case, the piece of
        # paper is 12 inches wide
        KNOWN_WIDTH = 14.5

        marker = cv2.minAreaRect(c)
        focalLength = 1975.86 * 4.05 # (marker[1][0] * KNOWN_DISTANCE) / KNOWN_WIDTH

        centimeters = (KNOWN_WIDTH * focalLength)/ marker[1][0]
        self.distanceFound = centimeters/10
        # draw a bounding box around the image and display it
        box = cv2.cv.BoxPoints(marker) if imutils.is_cv2() else cv2.boxPoints(marker)
        box = np.int0(box)
        cv2.drawContours(image, [box], -1, (0, 255, 0), 2)
        cv2.putText(image, "%.2fcm" % (centimeters / 10),
            (image.shape[1] - 200, image.shape[0] - 20), cv2.FONT_HERSHEY_SIMPLEX,
            2.0, (0, 255, 0), 3)
    
    # find shapes and see if it resembles the container
    def FindContainer(self, frame):
		
        maxoffcenter = 10
        
        # set color values to check for bright colors
        lowerBound = np.array([0, 100, 100])
        upperBound = np.array([255, 255, 255])

        kernelOpen = np.ones((5, 5))
        kernelClose = np.ones((20, 20))
        
        img = frame.array # set our working screen/ image

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
                    self.icanShowYouTheWorld(img, currentContour)
                    if(self.distanceFound < 25):
                        print("found stuff")
                        return True, self.distanceFound

                x, y, w, h = cv2.boundingRect(conts[i])
                cv2.rectangle(img, (x, y), (x+w, y+h), (0, 255, 0), 2)
                break
                
        cv2.imshow("cam", img)
        return False, None
