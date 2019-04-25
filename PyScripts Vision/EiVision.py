import cv2 as cv
import numpy as np
import math

cap = cv.VideoCapture(0)
while(1):
    _, frame = cap.read()
    processing = cv.cvtColor(frame, cv.COLOR_RGB2GRAY)
    processing = cv.GaussianBlur(processing,(5,5),0)
    ret,th = cv.threshold(processing,120,255,cv.THRESH_BINARY_INV)
    contours,hierarchy = cv.findContours(th,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
    for cnr in range(len(contours)):
        cnt = contours[cnr]
        area = cv.contourArea(cnt)
        perimeter = cv.arcLength(cnt, True)
        if perimeter > 100:
            factor = 4 * math.pi * area / perimeter**2
            if factor > 0.70:
                print(factor)
                cv.drawContours(frame, [cnt], -1, (255,0,0), 3)
    cv.imshow('test',frame)
    
    k = cv.waitKey(5) & 0xFF
    if k == ord('p'):
        break
cv.destroyAllWindows()
