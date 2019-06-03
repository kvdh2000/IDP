import cv2
import numpy
import imutils
import math

class CalcDistance:
    def __init__(self):
        pass

    def getDistance(self):
        focal = 3.6
        objHeight = 55
        camfHeight = 640  # 2592
        imgHeight = 640
        sensHeight = 3.42

        calc = (focal * objHeight * camfHeight)/(imgHeight * sensHeight)
        distance = calc / 10
        print(distance)
        #return distance
        