from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import cv2 as cv
import numpy as np
import time
import math
import timeit

camera = PiCamera()

time.sleep(1)
threshold = 255
rawCapture = PiRGBArray(camera)
while(1):
    count = 0
    print ("eerste")
    start = time.time()
    
    rawCapture = PiRGBArray(camera)
    camera.capture(rawCapture, format="bgr")
    frame = rawCapture.array

    end = time.time()
    print (end - start)
    
    processing = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    processing = cv.GaussianBlur(processing,(25,25),0)
    ret,th = cv.threshold(processing,threshold,255,cv.THRESH_BINARY)
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
    if count == 1:
        img = cv.drawContours(frame, [currentcontour], -1, (255,0,0), 3)
    elif threshold < 15:
        threshold = 255
    else:
        threshold -= 10
    cv.imshow('test',frame)
    #print (threshold)

    end = time.time()
    print (end - start)
    
    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
