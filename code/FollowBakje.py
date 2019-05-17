import cv2
import numpy as np
from imutils.video import VideoStream
import time
from picamera.array import PiRGBArray
from picamera import PiCamera

lowerBound=np.array([0,15,92])
upperBound=np.array([67,255,255])

camera = PiCamera()
camera.resolution = (640, 480)
camera.framerate = 32
rawCapture = PiRGBArray(camera, size=(640, 480))

#cam = VideoStream(usePiCamera=True).start()
time.sleep(.2)
kernelOpen=np.ones((5,5))
kernelClose=np.ones((20,20))

for frame in camera.capture_continuous(rawCapture, format='bgr', use_video_port=True):
    img=frame.array
    img=cv2.resize(img,(340,220))

    #convert BGR to HSV
    imgHSV= cv2.cvtColor(img,cv2.COLOR_BGR2HSV)
    # create the Mask
    mask=cv2.inRange(imgHSV,lowerBound,upperBound)
    #morphology
    maskOpen=cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
    maskClose=cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)

    maskFinal=maskClose
    __,conts, h =cv2.findContours(maskFinal.copy(),cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)

    img =cv2.drawContours(img,conts,-1,(0,255,0),4)

    for i in range(len(conts)):         
        cnt = conts[i]
        if cnt is None:
            continue
        x,y,w,h = cv2.boundingRect(cnt)
        img = cv2.rectangle(img,(x,y),(x+w,y+h),(0,0,255), 2)
    cv2.imshow("maskClose",maskClose)
    cv2.imshow("maskOpen",maskOpen)
    cv2.imshow("mask",mask)
    cv2.imshow("cam",img)
    rawCapture.truncate(0)
    k = cv2.waitKey(60) & 0xff
    if k == 27:
        break

cv2.destroyAllWindows()
cam.stop()
