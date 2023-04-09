import os

f = open("ov.rtsm", "a")
def PosX(objloc, PosX):
	f.write("stox :" + str(objloc) + ":" + str(PosX) + "\n")
	f.flush()
def PosY(objloc, PosX):
	f.write("stoy :" + str(objloc) + ":" + str(PosX) + "\n")
	f.flush()

def done():
	os.remove("ov.rtsm")
