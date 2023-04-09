import os

f = open("ov.rtsm", "a")
def PosX(objloc, PosX):
	f.write("px :" + str(objloc) + ":" + str(PosX) + "\n")
	f.flush()
def PosY(objloc, PosY):
	f.write("py :" + str(objloc) + ":" + str(-PosY) + "\n")
	f.flush()

def ScaleX(objloc, ScaleX):
	f.write("scax :" + str(objloc) + ":" + str(ScaleX) + "\n")
	f.flush()

def ScaleY(objloc, ScaleY):
	f.write("scay :" + str(objloc) + ":" + str(ScaleY) + "\n")
	f.flush()

def Angle(objloc, ScaleY):
	f.write("ang :" + str(objloc) + ":" + str(ScaleY) + "\n")
	f.flush()
def OutlineWidth(objloc, Width):
	f.write("oulw :" + str(objloc) + ":" + str(Width) + "\n")
	f.flush()

def OutlineColor(objloc, r, g, b):
	f.write("oulR :" + str(objloc) + ":" + str(r) + "\n")
	f.write("oulG :" + str(objloc) + ":" + str(g) + "\n")
	f.write("oulB :" + str(objloc) + ":" + str(b) + "\n")
	f.flush()




def done():
	os.remove("ov.rtsm")
