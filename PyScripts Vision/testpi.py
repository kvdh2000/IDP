from picamera.array import PiRGBArray
from picamera import PiCamera
from time import sleep
import time
import cv2

while True:
    camera = PiCamera()
    rawCapture = PiRGBArray(camera)
    time.sleep(1)
    camera.capture(rawCapture, format="bgr")
    image = rawCapture.array
    cv2.imshow("image", image)
cv2.waitKey()
cv2.destroyAllWindows()
