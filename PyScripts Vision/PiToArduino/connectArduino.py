import serial
import time
from tkinter import *
import asyncio

#port = '/dev/ttyACM0'
port = 'COM5'
command = None

ard = serial.Serial(port,9600,timeout=5)
time.sleep(2) # wait for Arduino

#sendCommand = True
#command can be max 5 characters long
#command = 'back'
top = Tk()
var = StringVar(top)
var.set("Placeholder")


async def GetArduino():
    msg = (ard.read(ard.inWaiting()))
    if(msg != None):
        print (str(msg.decode('utf-8')))
    var.set(msg)
    await asyncio.sleep(1)
    

TextBox = Entry(top)

def SendMessage():
    loop = asyncio.get_event_loop()
    command = TextBox.get()
    if(command != None):
        print ("Python value sent: ")
        print (command)
        ard.write(command.encode())
        time.sleep(3)
    loop.run_until_complete(GetArduino())


EnterButton = Button(top, text='Enter', command=SendMessage)
Label = Label(textvariable=var)

TextBox.pack()
EnterButton.pack(side = 'right')
Label.pack()

top.mainloop()


