
from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import numpy as np
import cv2
import time
import math

camera = PiCamera()

time.sleep(1)

while True:
        rawCapture = PiRGBArray(camera)
        camera.capture(rawCapture, format="bgr")
        frame = rawCapture.array
    
        gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
        gray = cv2.GaussianBlur(gray, (5, 5), 0)
        ret, th = cv2.threshold(gray, 183, 255, cv2.THRESH_BINARY)
        image, contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        cnt = contours[0]
        for cnt in contours:
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            if area > 10:#> 10000 and area < 60000:
                factor = 4 * math.pi * area / perimeter**2
                if factor < 0.6:# and factor > 0.4:
                    img = cv2.drawContours(frame, [cnt], -1, (0, 255, 255), 3)
                    print("egg")
        cv2.imshow("Frame", frame)

key = cv2.waitKey(0)
cv2.destroyAllWindows()
