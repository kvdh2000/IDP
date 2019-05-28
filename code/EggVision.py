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
        pass

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
                print("Go right")
            elif cx > ((len(frame[1]) / 2) + (len(frame[1]) * maxoffcenter * 0.005)):
                print("Go left")
            else:
                print("Good enough")
                print('area: ' + str(area))
                if (area > 20000):
                    return True
        elif self.threshold < 15:
            self.threshold = 255
        else:
            self.threshold -= 10
        cv.imshow('cam', frame)
