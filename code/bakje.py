import cv2
from picamera.array import PiRGBArray
from picamera import PiCamera
from shapedetector import ShapeDetector
import imutils
import numpy as np
import math
import time

# find shapes and see if it resembles the container
def FindBakje():
    # setup some variables
    foundBakje = False
    sleep = True
    sd = ShapeDetector()
    
    # setup the picamera
    cam = PiCamera()
    cam.resolution = (640, 480)
    cam.framerate = 32
    rawCapture = PiRGBArray(cam, size=(640, 480))
    time.sleep(.2) # sleep to give the camera a short time to startup
    
    # set color values to check for bright colors
    lowerBound = np.array([0, 100, 100])
    upperBound = np.array([255, 255, 255])

    kernelOpen = np.ones((5, 5))
    kernelClose = np.ones((20, 20))

    # for every frame seen by the cmaera see if there's a shape we recognize
    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):
        
        img = frame.array # set our working screen/ image
        resized = imutils.resize(img, width=300) # resize our screen
        ratio = img.shape[0] / float(resized.shape[0]) # respond to the resize
        
        # setup our "masks"
        gray = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
        blurred = cv2.GaussianBlur(gray, (5, 5), 0)
        thresh = cv2.threshold(blurred, 60, 255, cv2.THRESH_BINARY)[1]
        
        # find the contours
        cnts = cv2.findContours(thresh.copy(), cv2.RETR_EXTERNAL,
        cv2.CHAIN_APPROX_SIMPLE)
        cnts = imutils.grab_contours(cnts)

        # for every contour check if it's a sha[e we know
        for c in cnts:
            # compute the center of the contour, then detect the name of the
            # shape using only the contour
            M = cv2.moments(c)
            # debugging for the 'M' values
            #print("m10: " +str(M["m10"]))
            #print("m00: " +str(M["m00"]))
            #print("m01: " +str(M["m01"]))
            if(M["m10"] > 0.0 and M["m00"] > 0.0 and M["m01"] > 0.0):
                cX = int((M["m10"] / M["m00"]) * ratio)
                cY = int((M["m01"] / M["m00"]) * ratio)
                shape = sd.detect(c)

                # multiply the contour (x, y)-coordinates by the resize ratio,
                # then draw the contours and the name of the shape on the image
                c = c.astype("float") # cast the value as a float value
                c *= ratio
                c = c.astype("int")
                cv2.drawContours(img, [c], -1, (0, 255, 0), 2)
                cv2.putText(img, shape, (cX, cY), cv2.FONT_HERSHEY_SIMPLEX,
                    0.5, (255, 255, 255), 2) # set text on the found shape

        # may or may not be returned to a working state
        #if(foundBakje == True):
            #break
            
        cv2.imshow("cam", img)
        rawCapture.truncate(0) # clear the frame so we're ready to receive the next image
        cv2.waitKey(10)
        
    cv2.destroyAllWindows()
    cam.close()
    return True
