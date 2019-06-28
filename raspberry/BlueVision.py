__author__ = "Daan Eekhof, Elon Gielink"
__version__ = "0.8.0"
__maintainer__ = "Daan Eekhof"
__status__ = "Development"

import cv2
import numpy as np
import time
import math
import imutils


class BlueVision():
    """
    Enables camera and uses opencv to analyses the video for blue objects

    :param exit_event: (threading.Event object)
    :param servo_pos: (int list)
    """

    def __init__(self):
        print("shits fucked")
        pass

    def run(self, frame):
        # set how many frames the camera should wait before moving back to the
        # starting position if it does not spot any objects
        detection_timeout = 30
 
        # grab the raw NumPy array representing the image, then change colorspace, 
        # add blur, resize and flip for better recognition
        img = frame.array
        img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        img = cv2.GaussianBlur(img, (5,5), 0)
        img = cv2.resize(img, (512, 240))
        img = cv2.flip(img, -1)

        (h, w) = img.shape[:2]
        w2 = w / 2

        # create a mask for object detection and apply it to the image
        low_blue = np.array([98, 80, 80])
        high_blue = np.array([115, 255, 255])
        blue_mask = cv2.inRange(img, low_blue, high_blue)
#            blue = cv2.bitwise_and(img, img, mask=blue_mask)

        _, contours, _ = cv2.findContours(blue_mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        # loop through all contours that were found to match the mask
        for cnt in contours:
	    area = cv2.contourArea(cnt)
	    M = cv2.moments(cnt)
	    if 400 > area > 60:
	        # draw a box around the found object and calculate the ratio of length and width
	        rect = cv2.minAreaRect(cnt)
	        box = cv2.boxPoints(rect)
	        box = np.int0(box)
	        x_width_offset = abs(box[0][0] - box[1][0])
	        y_width_offset = abs(box[0][1] - box[1][1])
	        x_length_offset = abs(box[1][0] - box[2][0])
	        y_length_offset = abs(box[1][1] - box[2][1])
	        x_length = math.sqrt(x_width_offset**2 + y_width_offset**2)
	        y_length = math.sqrt(x_length_offset**2 + y_length_offset**2)

	        x_relative = x_length / y_length
	        y_relative = y_length / x_length

#                    img = cv2.drawContours(img, [cnt], -1, (0, 255, 255), 3)
#                    img = cv2.drawContours(img, [box], 0, (0, 0, 255), 2)
	        cx = int(M['m10'] / M['m00'])
	        cy = int(M['m01'] / M['m00'])

	    # check if the calculated ratio of length and width matches the object we're looking for
	    # if this is true return the position where the blue bar is
	        if ((4.0 > x_relative > 2.2) or (4.0 > y_relative > 2.2)):          
#                    img = cv2.circle(img, (cx, cy), 5, (0, 0, 255), -1)
		    self.ObjectPosition(w2, cx, self.servo_pos)
		    detection_timeout = 30
	    
    print("Bluedetection Thread Exited")

    @classmethod
    def ObjectPosition(self, w2, cx, pos):
        """
        Calculates necessary servo movements and updates servo_pos accordingly
        """
        min_bound = w2 - 30
        max_bound = w2 + 30

        if min_bound < cx < max_bound:  # if center of object is within bounds
            pass

        elif cx > min_bound:
            move = (abs(cx - w2) / 8)
            if not pos[0] < 255:
                pos[0] -= move
                return 'left'
                # maibi

        elif cx < max_bound:
            move = (abs(cx - w2) / 8)
            if not pos[0] > 0:
                pos[0] += move
                return 'right'
                # should be right
