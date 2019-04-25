from picamera import PiCamera
from io import BytesIO
import cv2 as cv
import numpy as np

stream = BytesIO()
camera = PiCamera()
camera.resolution = (640, 480)
camera.start_recording(stream, format='h264', quality=23)

while True:
    frame = camera.read()
    
    cv.imshow("Frame", frame)
    
    key = cv.waitKey(1) & 0xFF
    
    if key == ord("q"):
        break

camera.wait_recording(15)
camera.stop_recording()