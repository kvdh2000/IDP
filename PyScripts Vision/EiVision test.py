from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import cv2 as cv
import numpy as np
import time
import math
import imutils

res = (640, 480)
fr = 24
maxoffcenter = 10

camera = PiCamera()
camera.resolution = res
camera.framerate = fr

time.sleep(1)
threshold = 255

rawCapture = PiRGBArray(camera, size = res)
stream = camera.capture_continuous(rawCapture, format="bgr", use_video_port=True)
frame = None
for f in stream:
    count = 0

    frame = f.array
    rawCapture.truncate(0)
    
    processing = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    processing = cv.GaussianBlur(processing,(25,25),0)
    ret,th = cv.threshold(processing,threshold,255,cv.THRESH_BINARY)
    image, contours,hierarchy = cv.findContours(th,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
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
        img = cv.drawContours(frame, [currentcontour], -1, (255,0,0), 3)

        center = cv.moments(cnt)
        cx = int(center['m10']/center['m00'])
        cy = int(center['m01']/center['m00'])
        #if len(frame[1])
        print len(frame[1])
        #print (cx, " ", cy)
    elif threshold < 15:
        threshold = 255
        print ("tyfus")
    else:
        threshold -= 10
        print ("tyfus")
    cv.imshow('test',frame)

    #print (threshold)

    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
