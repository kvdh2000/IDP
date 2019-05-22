import cv2
from imutils.video import VideoStream
from pyzbar import pyzbar
import argparse
import datetime
import imutils
import time

class QRScanner:
	def __init__(self):
		pass
		
	# check if the names are the same
	def checkName(self, name, namecheck):
		if(name == namecheck):
			print(name)
			return True
		else:
			print("nothing found")
			return False

	# start searching for QR codes		
	def SearchQR(self, name, frame1):
		
		# grab the frame from the threaded video stream and resize it to
		# have a maximum width of 400 pixels
		frame = frame1.array
		frame = imutils.resize(frame, width=400)
		
		# find the barcodes in the frame and decode each of the barcodes
		barcodes = pyzbar.decode(frame)

		# loop over the detected barcodes
		for barcode in barcodes:
			# extract the bounding box location of the barcode and draw
			# the bounding box surrounding the barcode on the image
			(x, y, w, h) = barcode.rect
			cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)
		
			# the barcode data is a bytes object so if we want to draw it
			# on our output image we need to convert it to a string first
			barcodeData = barcode.data.decode("utf-8")
			barcodeType = barcode.type
		
			# draw the barcode data and barcode type on the image
			text = "{} ({})".format(barcodeData, barcodeType)
			#print(barcodeData)
			
			# check if the QR code is the same as the QR code that's 
			# assigned to us
			if (self.checkName(barcodeData, name)):
				return True
			
			cv2.putText(frame, text, (x, y - 10),
				cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2)
		 
			# show the output frame
			cv2.imshow("cam", frame)
			key = cv2.waitKey(1) & 0xFF
		 
			# if the `q` key was pressed, break from the loop
			if key == ord("q"):
				break
		 