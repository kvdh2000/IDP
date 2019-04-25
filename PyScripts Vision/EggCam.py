import numpy as np
import cv2
import math
from imutils.video import VideoStream
import imutils
import time

vs = VideoStream(src=0).start()

time.sleep(2.0)

while True:
    img = vs.read()

    #img = cv2.imread("egg_pics\\egg.jpg")
    if img is None:
        break

    
    img = imutils.resize(img, width=600)
    gray = cv2.GaussianBlur(img, (5, 5), 0)
    gray = cv2.cvtColor(gray, cv2.COLOR_BGR2GRAY)
    ret, th = cv2.threshold(gray, 183, 255, cv2.THRESH_BINARY_INV)
    #th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,11,2)
    contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours = imutils.grab_contours(contours)
    

    cnt = contours[0]
    for cnt in contours:
        area = cv2.contourArea(cnt)
        perimeter = cv2.arcLength(cnt, True)
        if area > 100:#> 10000 and area < 60000:
            factor = 4 * math.pi * area / perimeter**2
            if factor < 0.4:# and factor > 0.4:
                #img = cv2.drawContours(img, [cnt], -1, (0, 255, 255), 3)
                img = cv2.drawContours(img, cnt, -1, (0,255,0), 3, 8)
                #print("egg")

    cv2.imshow('img', img)
    key = cv2.waitKey(1) & 0xFF

	# if the 'q' key is pressed, stop the loop
    if key == ord("q"):
        break

cv2.waitKey(0)
cv2.destroyAllWindows()
