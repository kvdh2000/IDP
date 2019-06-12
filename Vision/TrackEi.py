
#!/usr/bin/python
#
# Copyright 2018 BIG VISION LLC ALL RIGHTS RESERVED
# 
from __future__ import print_function
import sys
import cv2
from random import randint
import numpy as np
import math
import imutils
from picamera.array import PiRGBArray
from picamera import PiCamera
import time
from time import sleep


trackerTypes = ['BOOSTING', 'MIL', 'KCF','TLD', 'MEDIANFLOW', 'GOTURN', 'MOSSE', 'CSRT']

def createTrackerByName(trackerType):
  # Create a tracker based on tracker name
  if trackerType == trackerTypes[0]:
    tracker = cv2.TrackerBoosting_create()
  elif trackerType == trackerTypes[1]: 
    tracker = cv2.TrackerMIL_create()
  elif trackerType == trackerTypes[2]:
    tracker = cv2.TrackerKCF_create()
  elif trackerType == trackerTypes[3]:
    tracker = cv2.TrackerTLD_create()
  elif trackerType == trackerTypes[4]:
    tracker = cv2.TrackerMedianFlow_create()
  elif trackerType == trackerTypes[5]:
    tracker = cv2.TrackerGOTURN_create()
  elif trackerType == trackerTypes[6]:
    tracker = cv2.TrackerMOSSE_create()
  elif trackerType == trackerTypes[7]:
    tracker = cv2.TrackerCSRT_create()
  else:
    tracker = None
    print('Incorrect tracker name')
    print('Available trackers are:')
    for t in trackerTypes:
      print(t)
    
  return tracker

if __name__ == '__main__':

  print("Default tracking algoritm is CSRT \n"
        "Available tracking algorithms are:\n")
  for t in trackerTypes:
      print(t)      

  trackerType = "CSRT"      

  
  # Create a video capture object to read videos
  cap = cv2.VideoCapture(0)
 
  # Read first frame
  success, frame = cap.read()
  # quit if unable to read the video file
  if not success:
    print('Failed to read video')
    sys.exit(1)

  ## Select boxes
  bboxes = []
  colors = [] 

  # OpenCV's selectROI function doesn't work for selecting multiple objects in Python
  # So we will call this function in a loop till we are done selecting all objects
  while True:
    # draw bounding boxes over objects
    # selectROI's default behaviour is to draw box starting from the center
    # when fromCenter is set to false, you can draw box starting from top left corner
    bbox = cv2.selectROI('MultiTracker', frame)
    bboxes.append(bbox)
    colors.append((randint(64, 255), randint(64, 255), randint(64, 255)))
    print("Press q to quit selecting boxes and start tracking")
    print("Press any other key to select next object")
    k = cv2.waitKey(0) & 0xFF
    if (k == 113):  # q is pressed
      break
  
  print('Selected bounding boxes {}'.format(bboxes))

  ## Initialize MultiTracker
  # There are two ways you can initialize multitracker
  # 1. tracker = cv2.MultiTracker("CSRT")
  # All the trackers added to this multitracker
  # will use CSRT algorithm as default
  # 2. tracker = cv2.MultiTracker()
  # No default algorithm specified

  # Initialize MultiTracker with tracking algo
  # Specify tracker type
  trackerType = "CSRT"
  
  # Create MultiTracker object
  multiTracker = cv2.MultiTracker_create()

  # Initialize MultiTracker 
  for bbox in bboxes:
    multiTracker.add(createTrackerByName(trackerType), frame, bbox)


  # Process video and track objects
  while cap.isOpened():
    count = 0

    frame = f.array
    rawCapture.truncate(0)
    
    processing = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    processing = cv.GaussianBlur(processing,(25,25),0)
    ret,th = cv.threshold(processing,threshold,255,cv.THRESH_BINARY)
    image, contours,hierarchy = cv.findContours(th,cv.RETR_TREE,cv.CHAIN_APPROX_SIMPLE)
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