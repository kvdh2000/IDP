__author__ = "Daan Eekhof, Keanu Attema, and Elon Gielink"
__version__ = "0.8.0"
__maintainer__ = "Daan Eekhof"
__status__ = "Development"

# import our own scripts
from ConVision import FindCon # vind het bakje 80% van de keren
from QRVision import QRScanner # werkt
from BlueVision import BlueVision # werkt misschien
from EggVision import EggVision # ongeveer 80% nauwkeurig, distance is goed mits het ei goed gevonden word
from LineVision import LineVision # werkt niet
from TrackerTest import TrackerTest # werkt maar nog niet autonoom
from ChickVision import ChickVision # werkt niet

import argparse
import asyncio
import cv2
import imutils
from picamera import PiCamera
from picamera.array import PiRGBArray
import serial
import time
import re

# used for grabbing an argument in this case 
# we use it for grabbing which location we need 
# to deliver the egg to
# will be parsed from the controller
ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()

# setup the picamera, this way we don't have to 
# close it between switching scripts
cam = PiCamera()
cam.resolution = (640, 480)
cam.framerate = 90
rawCapture = PiRGBArray(cam, size=(640, 480))

# initialize variable to request 
# the method from the other script file
scan = QRScanner()
find = FindCon()
egg = EggVision()
blue = BlueVision()
line = LineVision()
track = TrackerTest()
chicken = ChickVision()
found =[False, False, False]

# use this to check if the arm stopped moving
# so we can continue with the vision stuff
armMoved = False
boolLook = False 
setupComplete = True
locationCon = 'Duckstad' # default is None

# initialize variable for serial communication
port = '/dev/ttyACM0' # Raspberry port which connects to the arduino
baud = 9600 # set arduino baudrate
ard = serial.Serial(port,baud,timeout=5)
msg = ''
time.sleep(.5) # wait for Arduino and camera to start up

# Run this method async so we can grab the replies from the
# arduino
async def GetArduino():
    # add 'global' so it doesn't create a local variable named armMoved
    global locationCon
    global boolLook
    global armMoved 
    global setupComplete

    msg = (ard.read(ard.inWaiting()))
    if msg != None:
        print(msg)
        if msg == "b''":
            print(msg)
        if re.search('.(Arm mov).', str(msg)): # check if the arm stopped moving
            print("found")
            armMoved = True
            boolLook = False
        elif re.search('.(MEGA start).', str(msg)): # check if the arduino booted
            setupComplete = True
        elif re.search('.(Loc: Duckst).', str(msg)):
            locationCon = 'Duckstad'
        elif re.search(".(Loc: 'Eiber).", str(msg)):
            locationCon = 'Eibergen'
        elif re.search('.(Loc: Barnev).', str(msg)):
            locationCon = 'Barneveld'
        elif re.search('.(Loc: Eindho).', str(msg)):
            locationCon = 'Eindhoven'
    
# Method for sending commands to the arduino
# Command can be max 5 characters long
def SendMessage(command):
    if(command != None):
        print("Python value sent: ")
        print(command)
        ard.write(command.encode())

def main():
    loop = asyncio.get_event_loop()
    global boolLook
    # drive around and use the camera to search for objects
    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):

        loop.run_until_complete(GetArduino())
        if not boolLook:
            SendMessage('look|')
            boolLook = True

        # follow these steps in order
        # wait till the arduino is ready to receive commands
        if setupComplete: 
            if(not found[0]):
                found[0], location = egg.FindEgg(frame) # find the egg
                if found[0] and location is not None:
                    SendMessage('marm-z'+str(round(location, 1))+'|') # send a command to the arduino over Serial
        if armMoved:        
            if(not found[1]):
                found[1] = scan.SearchQR(locationCon, frame) # scans for the QRCode
            elif(not found[2]):
                found[2], location = find.FindContainer(frame) # find the container
                if found[2]:
                    SendMessage('marm-z'+str(round(location, 1))+'|')
            else:
                print("got all")
        
        # Direct functies aanroepen om te debuggen 
        # niet afhankelijk maken van de Arduino 
        # om te testen
        #line.FindLine(frame) # run continuesly to check if we're still in the playing field
        #track.TrackEgg(frame) # niet autonoom maar werkt wel
        #egg.FindEgg(frame) # niet 100% nauwkeurig maar werkt, distance werkt wanneer het ei goed gevonden word
        #chicken.FindChicken(frame) # werkt niet

        rawCapture.truncate(0) # ready the camera for a new frame to be analysed
        cv2.waitKey(10)

    cv2.destroyAllWindows()
    cam.close()


if __name__== "__main__":
    main()
