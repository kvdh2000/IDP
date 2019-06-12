# import our own scripts
from SerialReciever import SerialReciever

import argparse
import asyncio
import cv2

#from picamera import PiCamera
#from picamera.array import PiRGBArray
import serial
import time
import re

# used for grabbing an argument in this case 
# we use it for grabbing which location we need 
# to deliver the egg to
ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()

# setup the picamera, this way we don't have to 
# close it between switching scripts

#cam = PiCamera()
#cam.resolution = (640, 480)
#cam.framerate = 90
#rawCapture = PiRGBArray(cam, size=(640, 480))

# initialize variable to request 
# the method from the other script file
#scan = QRScanner()
#find = FindCon()
#egg = EggVision()
#blue = BlueVision()
#line = LineVision()
#track = TrackerTest()
#chicken = ChickVision()
found =[False, False, False]

# use this to check if the arm stopped moving
# so we can continue with the vision stuff

# initialize variable for serial communication
# this object also contains the armmoved boollook ect. vars
# Command can be max 24 characters long
# baudrate is 9600 by default
arduino = SerialReciever('/dev/ttyACM0')



time.sleep(.5) # wait for Arduino and camera to start up




def main():
    loop = asyncio.get_event_loop()
    global boolLook
    # drive around and use the camera to search for objects
    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):

        loop.run_until_complete(arduino.recieve())
        if not arduino.boolLook:
            arduino.send_command('look')
            arduino.boolLook = True

        # follow these steps in order
        # wait till the arduino is ready to receive commands
        if arduino.setupComplete:
            if(not found[0]):
                found[0], location = egg.FindEgg(frame) # find the egg
                if found[0] and location is not None:
                    arduino.send_command('marm') # send a command to the arduino over Serial
        if arduino.armMoved:
            if(not found[1]):
                pass
                #found[1] = scan.SearchQR(argName.a, frame) # scans for the QRCode
            elif(not found[2]):
                pass
                #found[2] = find.FindContainer(frame) # find the container
                #if found[2]:
                #    arduino.send_command("marm")
            else:
                print("got all")
        
        #line.FindLine(frame) # run continuesly to check if we're still in the playing field
        
        #track.TrackEgg(frame)
        #egg.FindEgg(frame)
        #chicken.FindChicken(frame)

        rawCapture.truncate(0) # ready the camera for a new frame to be analysed
        cv2.waitKey(10)

    cv2.destroyAllWindows()
    cam.close()
    


if __name__== "__main__":
    main()
