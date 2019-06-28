__author__ = "Daan Eekhof, Elon Gielink"
__version__ = "1.0.0"
__maintainer__ = "Daan Eekhof, Elon Gielink"
__status__ = "Development"

import cv2 as cv
import numpy as np
import imutils
import math

class CalcDistance:
    def __init__(self):
        self.distanceFound = 0
        pass

    # requires, image/ frame form camera
    # contour, focalRange (calculated from the amount of pixels in the photo)
    # known width of the object in real life in centimeters
    def getDistance(self, image, c, focalRange, KNOWN_WIDTH):
        # initialize the known object width, which in this case, the piece of
        # paper is 12 inches wide

        marker = cv.minAreaRect(c)
        focalLength = focalRange * 4.05 # (marker[1][0] * KNOWN_DISTANCE) / KNOWN_WIDTH

        centimeters = (KNOWN_WIDTH * focalLength)/ marker[1][0]
        self.distanceFound = centimeters/10
        # draw a bounding box around the image and display it
        box = cv.cv.BoxPoints(marker) if imutils.is_cv2() else cv.boxPoints(marker)
        box = np.int0(box)
        cv.drawContours(image, [box], -1, (0, 255, 0), 2)
        cv.putText(image, "%.2fcm" % (centimeters / 10),
            (image.shape[1] - 200, image.shape[0] - 20), cv.FONT_HERSHEY_SIMPLEX,
            2.0, (0, 255, 0), 3)
        return self.distanceFound
        