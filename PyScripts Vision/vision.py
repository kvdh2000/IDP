from collections import deque
from imutils.video import VideoStream
import numpy as np
import cv2
import imutils
import time
import math

cap = cv2.VideoCapture(0)

while True:
    __,frame = cap.read()
    
    #gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    #blurred = cv2.GaussianBlur(gray, (5,5), 0)
    
    cv2.imshow("frame", frame)
    
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()