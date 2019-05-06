from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import cv2 as cv
import numpy as np
import time
import math

camera = PiCamera()

time.sleep(1)

while(1):
    count = 0
    rawCapture = PiRGBArray(camera)
    camera.capture(rawCapture, format="bgr")
    frame = rawCapture.array

    processing = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    processing = cv.GaussianBlur(processing,(5,5),0)
    ret,th = cv.threshold(processing,183,255,cv.THRESH_BINARY)
    image, contours,hierarchy = cv.findContours(th,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
    for cnr in range(len(contours)):
        cnt = contours[cnr]
        area = cv.contourArea(cnt)
        perimeter = cv.arcLength(cnt, True)
        if perimeter > 1000:
            factor = 4 * math.pi * area / perimeter**2
            if factor > 0.77:
                count += 1
                currentcontour = cnt
    
    img = cv.drawContours(frame, [currentcontour], -1, (255,0,0), 3)
    
    cv.imshow('test',frame)
    
    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
