from collections import deque
from imutils.video import VideoStream
import numpy as np
import cv2
import argparse
import imutils
import time
import math

cap = cv2.VideoCapture(0)

while True:
    __,frame = cap.read()
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    gray = cv2.GaussianBlur(gray, (5, 5), 0)
    
    contours,__ = cv2.findContours(gray, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    cnt = contours[0]
    for cnts in contours:
        frame = cv2.drawContours(frame, cnts, -1, (0,255,255), 2, 3)
    
    cv2.imshow("Frame", frame)
    key = cv2.waitKey(1) & 0xFF
    
    if key == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()