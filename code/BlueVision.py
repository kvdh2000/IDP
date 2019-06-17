import cv2 as cv
import numpy as np
import time
import math
import imutils

class BlueVision:
    def __init__(self):
        pass
        
    def FindCar(self, f):

        frame = f.array
        
        image, contours, hierarchy = cv.findContours(cv.inRange(cv.cvtColor(frame, cv.COLOR_RGB2HSV), np.array([10,115,100]), np.array([40,255,255])),cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
        for cnr in range(len(contours)):
                cnt = contours[cnr]
                center = cv.moments(cnt)
                if center['m00'] > 0 and cv.contourArea(cnt) > 500:
                        frame = cv.drawContours(frame, [cnt], -1, (0,255,0), 3)
                        cx = int(center['m10']/center['m00'])
                        cy = int(center['m01']/center['m00'])
                        print(cx, " ", cy)
                        if cx < ((len(frame[1]) / 2) - (len(frame[1]) * maxoffcenter * 0.005)):
                            print("Go left")
                        elif cx > ((len(frame[1]) / 2) + (len(frame[1]) * maxoffcenter * 0.005)):
                            print("Go right")
                        else:
                            print("Good enough")
                            print('area: ' + str(area))
                            if (area > 20000):
                                centimeter = self.dist.getDistance(frame, currentcontour, 2045.45, 4.2)
                                if(centimeter < 25):
                                    print("found stuff")
                        frame = cv.circle(frame,(cx,cy),4,(255,0,0),-1)
        cv.imshow('cam',frame)
