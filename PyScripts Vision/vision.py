import numpy as np
import cv2
import math
import imutils

cap = cv2.VideoCapture(0)

lower = np.array([0,0,85])
upper = np.array([0,0,100])

while(True):
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    shapeMask = cv2.inRange(cap, lower, upper)
    cnts = cv2.findContours(shapeMask.copy(), cv2.RETR_EXTERNAL,
	    cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    print("I found {} black shapes".format(len(cnts)))
    #cv2.imshow("Mask", shapeMask)
    for c in cnts:
        # draw the contour and show it
        cv2.drawContours(cap, [c], -1, (0, 255, 0), 2)
        cv2.imshow("Image", cap)
        cv2.waitKey(0)
    #cv2.imshow('frame', gray)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()