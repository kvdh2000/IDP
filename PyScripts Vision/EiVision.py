from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import cv2 as cv
import numpy as np
import time
import math
import timeit
import imutils

res = (480, 360)
fr = 2

camera = PiCamera()
camera.resolution = res
camera.framerate = fr

time.sleep(1)
threshold = 255

rawCapture = PiRGBArray(camera, size = res)
stream = camera.capture_continuous(rawCapture, format="bgr", use_vedio_port=True)
frame = None
stopped = False

    
for f in stream:
    count = 0

    frame = f.array
    rawCapture.truncate(0)

    end = time.time()
    print (end - start)
    
    processing = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
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

    print (threshold)

    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
