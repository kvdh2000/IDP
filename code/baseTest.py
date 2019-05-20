from bakje import FindBakje # import our own script
from barcode_video import SearchQR # import our own script
import argparse
import time

ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()

#drive around
func = FindBakje()
print(func)
# if Bakje found it switch to QR scode scanning
if(func == True):
	SearchQR(argName.a)

#do a scan
#SearchQR(argName.a)
