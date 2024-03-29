__author__ = "Daan Eekhof, Keanu Attema, and Elon Gielink"
__version__ = "0.8.0"
__status__ = "Development"

# import our own scripts
from ConVision import FindCon
from QRVision import QRScanner 
from BlueVision import BlueVision 
from EggVision import EggVision 
from LineVision import LineVision 
from TrackerTest import TrackerTest 
from ChickVision import ChickVision 

import asyncio
import cv2
import imutils
from picamera import PiCamera
from picamera.array import PiRGBArray
import serial
import time
import re
import os

# setup the picamera, this way we don't have to 
# close it between switching scripts
cam = PiCamera()
cam.resolution = (640, 480)
cam.framerate = 60

# not sure what it does we can test it out
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
arduinoDisc = False
locationCon = None # default is None
locations = ("Duckstad", "Eibergen", "Eindhoven", "Barneveld") # initialize locations
tracking = False
moveDir = "Nope"
vuValue = None

# initialize variable for serial communication
port = '/dev/ttyACM0' # Raspberry port which connects to the arduino
baud = 38400 # set arduino baudrate
ard = None
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
    global tracking
    global vuValue

    msg = (ard.read(ard.inWaiting()))
    if msg != None:
        # used for grabbing an argument in this case 
        # we use it for grabbing which location we need 
        # to deliver the egg to
        loc = re.search('.?(Loc:)(\d).?', str(msg))
        vuValue = re.search('.?(VU:)(\d{0,2}.\d{0,2}).?', str(msg))

        if msg == "b''":
            print()
        if re.search('.(Arm mov).', str(msg)): # check if the arm stopped moving
            armMoved = True
            boolLook = False
        elif re.search('.(MEGA start).', str(msg)): # check if the arduino booted
            setupComplete = True        
        elif re.search('.(Start Tracking).', str(msg)):
            tracking = True
            print("tracking python: "+ str(tracking))      
        elif re.search('.(Stop Tracking).', str(msg)):
            tracking = False
            print("tracking python: "+ str(tracking)) 
        elif vuValue is not None and float(vuValue.group(2)) < 10.25:
            os.system('sudo shutdown now')
        elif loc is not None:
            locationCon = locations[int(loc.group(2))]
            print("Location python: "+locationCon)        


    
# Method for sending commands to the arduino
# Command can be max 5 characters long
def SendMessage(command):
    global arduinoDisc
    if command is not None and not arduinoDisc:
        print("Python value sent: ")
        print(command)
        ard.write(command.encode())
def Move(dir):
    global moveDir

    if(moveDir is not dir):
        SendMessage("move-x"+str(dir) + "|")
        moveDir = dir

def main():
    loop = asyncio.get_event_loop()
    global boolLook
    global tracking
    global ard
    global arduinoDisc

    try:
        ard = serial.Serial(port,baud,timeout=5)
    except serial.serialutil.SerialException:
        arduinoDisc = True
        print('Arduino is not connected')

    # drive around and use the camera to search for objects
    for frame in cam.capture_continuous(rawCapture, format='bgr', use_video_port=True):

        if not arduinoDisc:
            loop.run_until_complete(GetArduino())
        if not boolLook:
            SendMessage('look|')
            boolLook = True
        
        # follow these steps in order
        # wait till the arduino is ready to receive commands
        if setupComplete: 
            if tracking:
                direction = blue.FindCar(frame)                 
                if direction is not None:
                    Move(direction)              

            elif not found[0]:
                found[0], location, direction = egg.FindEgg(frame) # find the egg
                if found[0] and location is not None and direction is None:
                    SendMessage('marm-z'+str(round(location, 1))+'|') # send a command to the arduino over Serial
                # elif not found[0] and location is None and direction is not None:
                #     Move(direction)
        if armMoved:        
            if not found[1] and locationCon is not None:
                found[1] = scan.SearchQR(locationCon, frame) # scans for the QRCode
            elif not found[2]:
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
        # locTest = blue.FindCar(frame)
        # if locTest is not None:
        #     SendMessage('blueLoc-'+locTest)
        #     print('location blue: '+locTest)
        blue.run(frame)

        rawCapture.truncate(0) # ready the camera for a new frame to be analysed
        cv2.waitKey(10)

    cv2.destroyAllWindows()
    cam.close()


if __name__== "__main__":
    main()
