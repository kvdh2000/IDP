__author__ = "Merijn, Daan Eekhof"
__version__ = "0.8.0"
__maintainer__ = "Merijn"
__status__ = "Development"

import cv2
from random import randint
import numpy as np
import math
import imutils

class TrackerTest:
    def __init__(self):
        pass

    bboxes = []
    colors = [] 

    # Create MultiTracker object
    # and initialize our Tracking algorithm
    multiTracker = cv2.MultiTracker_create()
    csrtTracker = cv2.TrackerCSRT_create()

    def TrackEgg(self, f):
        frame = f.array
        if not self.bboxes:
            bbox = cv2.selectROI('MultiTracker', frame)
            self.bboxes.append(bbox)

            # Initialize MultiTracker 
            for bbox in self.bboxes:
                self.multiTracker.add(self.csrtTracker, frame, bbox)
        
        # get updated location of objects in subsequent frames
        boxes = self.multiTracker.update(frame) # makes shit slow

        # draw tracked objects
        for i, newbox in enumerate(boxes):
            if i is 1:
                p1 = (int(newbox[0, 0]), int(newbox[0, 1]))
                p2 = (int(newbox[0, 0] + newbox[0, 2]), int(newbox[0, 1] + newbox[0, 3]))
                cv2.rectangle(frame, p1, p2, (146,186,108), 2, 1)

        # show frame
        cv2.imshow('MultiTracker', frame)
