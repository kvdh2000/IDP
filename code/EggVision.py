from CalcDistance import CalcDistance
from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import cv2 as cv
import numpy as np
import time
import math
import imutils


class EggVision:
    
    threshold = 255
    def __init__(self):
        self.distanceFound = 0
        pass

    def icanShowYouTheWorld(self, image, c):
        # initialize the known distance from the camera to the object, which
        # in this case is 24 inches
        KNOWN_DISTANCE = 50.0

        # initialize the known object width, which in this case, the piece of
        # paper is 12 inches wide
        KNOWN_WIDTH = 4.2

        # load the furst image that contains an object that is KNOWN TO BE 2 feet
        # from our camera, then find the paper marker in the image, and initialize
        # the focal length
        marker = cv.minAreaRect(c)
        focalLength = 2045.45 * 4.05 # (marker[1][0] * KNOWN_DISTANCE) / KNOWN_WIDTH

        centimeters = (KNOWN_WIDTH * focalLength)/ marker[1][0]
        print("Afstand: " + str(centimeters/10) + " CMs")
        self.distanceFound = centimeters/10
        # draw a bounding box around the image and display it
        box = cv.cv.BoxPoints(marker) if imutils.is_cv2() else cv.boxPoints(marker)
        box = np.int0(box)
        cv.drawContours(image, [box], -1, (0, 255, 0), 2)
        cv.putText(image, "%.2fcm" % (centimeters / 10),
            (image.shape[1] - 200, image.shape[0] - 20), cv.FONT_HERSHEY_SIMPLEX,
            2.0, (0, 255, 0), 3)
    
    def FindEgg(self, frame):
        maxoffcenter = 10
        count = 0
        frame = frame.array

        processing = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
        processing = cv.GaussianBlur(processing, (25, 25), 0)
        ret, th = cv.threshold(processing, self.threshold,
                                 255, cv.THRESH_BINARY)
        image, contours, hierarchy = cv.findContours(
               th, cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)
        for cnr in range(len(contours)):
            cnt = contours[cnr]
            area = cv.contourArea(cnt)
            perimeter = cv.arcLength(cnt, True)
            if perimeter > len(frame) / 6:
                factor = 4 * math.pi * area / perimeter**2
                if factor > 0.77:
                    count += 1
                    currentcontour = cnt
        if count == 1:
            img = cv.drawContours(
                frame, [currentcontour], -1, (255, 0, 0), 3)

            center = cv.moments(currentcontour)
            cx = int(center['m10']/center['m00'])
            cy = int(center['m01']/center['m00'])
            if cx < ((len(frame[1]) / 2) - (len(frame[1]) * maxoffcenter * 0.005)):
                print("Go left")
            elif cx > ((len(frame[1]) / 2) + (len(frame[1]) * maxoffcenter * 0.005)):
                print("Go right")
            else:
                print("Good enough")
                print('area: ' + str(area))
                self.icanShowYouTheWorld(frame, currentcontour)
                if (area > 20000):
                    self.icanShowYouTheWorld(frame, currentcontour)
                    if(self.distanceFound < 20):
                        print("found stuff")
                        return True
        elif self.threshold < 15:
            self.threshold = 255
        else:
            self.threshold -= 10
        cv.imshow('cam', frame)
