import cv2 as cv
import numpy as np
import time
import math
import imutils
 
res = (640, 480)
fr = 24
maxoffcenter = 10
threshold = 255
tyfus = False
 
cap = cv.VideoCapture(0)
 
while(1):
    count = 0
 
    _, frame = cap.read()
     
    processing = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    processing = cv.GaussianBlur(processing,(25,25),0)
    ret,th = cv.threshold(processing,threshold,255,cv.THRESH_BINARY)
    contours,hierarchy = cv.findContours(th,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
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
 
        center = cv.moments(currentcontour)
        cx = int(center['m10']/center['m00'])
        cy = int(center['m01']/center['m00'])
        if cx < ((len(frame[1]) / 2) - (len(frame[1]) * maxoffcenter * 0.005)):
            print ("Go right")
        elif cx > ((len(frame[1]) / 2) + (len(frame[1]) * maxoffcenter * 0.005)):
            print ("Go left")
        else:
            print ("Good enough")
 
        tyfus = False
        #print (cx, " ", cy)
    elif threshold < 15:
        threshold = 255
        if tyfus == False:
            tyfus = True
            print ("tyfus")
    else:
        threshold -= 10
        if tyfus == False:
            tyfus = True
            print ("tyfus")
    cv.imshow('test',frame)
    #print (threshold)
 
    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
