import serial
import time
from tkinter import *
import asyncio

#port = '/dev/ttyACM0' # Raspberry port which connects to the arduino
port = 'COM6'

ard = serial.Serial(port,9600,timeout=5)
time.sleep(2) # wait for Arduino

top = Tk() # initialize the Window variable
var = StringVar(top) # initialize the Label text variable
var.set("") # Set label text to something forst as it has to be set to something

# Run this method async so we can grab the replies from the 
# arduino
async def GetArduino():
    msg = (ard.read(ard.inWaiting()))
    if(msg != None):
        print (str(msg.decode('utf-8')))
    var.set(msg)
    await asyncio.sleep(1)
    
TextBox = Entry(top)

# Method for sending commands to the arduino
# Command can be max 5 characters long
def SendMessage():
    loop = asyncio.get_event_loop()
    command = None # initialize the command variable
    command = TextBox.get()
    if(command != None):
        print ("Python value sent: ")
        print (command)
        ard.write(command.encode())
        time.sleep(3)
    loop.run_until_complete(GetArduino())

EnterButton = Button(top, text='Enter', command=SendMessage)
Label = Label(textvariable=var)

# Add the elements to the screen
TextBox.pack()
EnterButton.pack(side = 'right')
Label.pack()

# Keep the window open until we close it
top.mainloop()