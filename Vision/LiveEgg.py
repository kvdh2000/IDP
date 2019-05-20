
from collections import deque
from imutils.video import VideoStream
import numpy as np
import argparse
import cv2
import imutils
import time
import math

ap = argparse.ArgumentParser()
ap.add_argument("-v", "--video",
        help="path to the (optional) video file")
ap.add_argument("-b", "--buffer", type=int, default=64,
        help="max buffer size")
args = vars(ap.parse_args())

greenLower = (0, 0,50)
greenUpper = (0, 0, 255)
pts = deque(maxlen=args["buffer"])

if not args.get("video", False):
        vs = VideoStream(src=0).start()
else:
        vs = cv2.VideoCapture(args["video"])

time.sleep(2.0)

while True:
        frame = vs.read()

        frame = frame[1] if args.get("video", False) else frame
        if frame is None:
                break
            
        frame = imutils.resize(frame, width=600)

        gray = cv2.cvtColor(frame, cv2.COLOR_RGB2HSV)
        lower_red = np.array([0, 0, 50])
        upper_red = np.array([0, 13, 255])
        gray = cv2.GaussianBlur(gray, (5, 5), 0)
        mask2 = cv2.inRange(gray, lower_red, upper_red)
        ret, th = cv2.threshold(mask2, 60, 255, cv2.THRESH_BINARY)
        #th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,11,2)
        contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        cnt = contours[0]
        for cnt in contours:
            area = cv2.contourArea(cnt)
            perimeter = cv2.arcLength(cnt, True)
            if area > 0:#> 10000 and area < 60000:
                factor = 4 * math.pi * area / perimeter**2
                if factor < .2:# and factor > 0.4:
                    img = cv2.drawContours(frame, [cnt], -1, (0, 255, 255), 3)
                    print("egg")
        
        cv2.imshow("Frame", frame)
        key = cv2.waitKey(1) & 0xFF

        if key == ord("q"):
                break

if not args.get("video", False):
        vs.stop()
else:
        vs.release()

cv2.destroyAllWindows()