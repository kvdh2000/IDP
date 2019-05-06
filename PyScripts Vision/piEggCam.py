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
    rawCapture = PiRGBArray(camera)
    camera.capture(rawCapture, format="bgr")
    frame = rawCapture.array

    gray = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    gray = cv.GaussianBlur(gray,(5,5),0)
    ret,th = cv.threshold(gray,183,255,cv.THRESH_BINARY)
    image, contours,hierarchy = cv.findContours(th,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)

    cnt = contours[0]
    for cnt in contours:
        area = cv.contourArea(cnt)
        perimeter = cv.arcLength(cnt, True)
        if area > 10:#> 10000 and area < 60000:
            factor = 4 * math.pi * area / perimeter**2
            if factor < 0.6:# and factor > 0.4:
                img = cv.drawContours(frame, [cnt], -1, (0, 255, 255), 3)
                print("egg")
        cv.imshow("Frame", frame)
key = cv.waitKey(0)
cv.destroyAllWindows()
