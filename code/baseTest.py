from bakje import FindBakje
from barcode_video import SearchQR
import argparse

ap = argparse.ArgumentParser()
ap.add_argument("a")
argName = ap.parse_args()


#drive around
FindBakje()

#if(FindBakje == true):
	#SearchQR(argName.a)

#do a scan
#SearchQR(argName.a)
