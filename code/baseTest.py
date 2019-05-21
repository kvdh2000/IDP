from bakje import FindCon # import our own script
from barcode_video import QRScanner # import our own script
import argparse
import time

ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()

scan = QRScanner()
find = FindCon()

#drive around
func = find.FindBakje()
print(func)
# if Bakje found it switch to QR scode scanning
if(func == True):
	scan.SearchQR(argName.a)

#do a scan
#SearchQR(argName.a)
