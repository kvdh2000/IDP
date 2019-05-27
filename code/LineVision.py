from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import cv2 as cv
import numpy as np
import time
import math
import imutils

class LineVision:
    def __init__(self):
        pass
        
    def FindLine(self, f):
        maxoffcenter = 10
        threshold = 255
        frame = None
        tyfus = False

        count = 0

        frame = f.array
        
        image, contours, hierarchy = cv.findContours(cv.inRange(cv.cvtColor(frame, cv.COLOR_RGB2HSV), np.array([0, 0, 0]), np.array([0, 0, 0])),cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
        for cnr in range(len(contours)):
                cnt = contours[cnr]
                center = cv.moments(cnt)
                if center['m00'] > 0 and cv.contourArea(cnt) > 500:
                        frame = cv.drawContours(frame, [cnt], -1, (0,255,0), 3)
                        cx = int(center['m10']/center['m00'])
                        cy = int(center['m01']/center['m00'])
                        print(cx, " ", cy)
                        frame = cv.circle(frame,(cx,cy),4,(255,0,0),-1)
        cv.imshow('cam',frame)
