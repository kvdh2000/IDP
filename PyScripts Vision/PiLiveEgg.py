
from picamera import PiCamera
from io import BytesIO
from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time
import math

stream = BytesIO()
camera = PiCamera()
camera.resolution = (640, 480)
camera.start_recording(stream, format='h264', quality=23)
camera.wait_recording(3)

time.sleep(2.0)

while True:
        frame = camera

        frame = frame[1]
        frame = imutils.resize(frame, width=600)

        gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
        gray = cv2.GaussianBlur(gray, (5, 5), 0)
        ret, th = cv2.threshold(gray, 183, 255, cv2.THRESH_BINARY)
        contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

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
camera.stop_recording()
cv2.destroyAllWindows()
