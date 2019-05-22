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
tyfus = False

for f in stream:
    count = 0

    frame = f.array
    rawCapture.truncate(0)
    
    hsv = cv.cvtColor(frame, cv.COLOR_RGB2HSV)
    frame = cv.circle(frame,(320,240),4,(255,0,0),-1)
    print(hsv[240][320])

    cv.imshow('test',frame)

    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
