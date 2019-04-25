import numpy as np
import cv2
import math

img = cv2.imread('D:/GitHub/IDP.git/trunk/PyScripts Vision/egg.jpg')

gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
gray = cv2.GaussianBlur(gray, (5, 5), 0)
ret, th = cv2.threshold(gray, 183, 255, cv2.THRESH_BINARY_INV)
#th3 = cv.adaptiveThreshold(img,255,cv.ADAPTIVE_THRESH_GAUSSIAN_C,cv.THRESH_BINARY,11,2)
contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

cnt = contours[0]
for cnt in contours:
    area = cv2.contourArea(cnt)
    perimeter = cv2.arcLength(cnt, True)
    if area > 100:#> 10000 and area < 60000:
        factor = 4 * math.pi * area / perimeter**2
        if factor < 0.4:# and factor > 0.4:
            img = cv2.drawContours(img, [cnt], -1, (0, 255, 255), 3)
            print("egg")

cv2.imshow('img', img)
cv2.waitKey(0)
cv2.destroyAllWindows()
