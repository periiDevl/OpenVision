import os

f = open("ov.rtsm", "a")
def PosX(objloc, PosX):
	f.write("px :" + str(objloc) + ":" + str(PosX) + "\n")
def PosY(objloc, PosY):
	f.write("py :" + str(objloc) + ":" + str(-PosY) + "\n")

def ScaleX(objloc, ScaleX):
	f.write("scax :" + str(objloc) + ":" + str(ScaleX) + "\n")

def ScaleY(objloc, ScaleY):
	f.write("scay :" + str(objloc) + ":" + str(ScaleY) + "\n")

def Angle(objloc, ScaleY):
	f.write("ang :" + str(objloc) + ":" + str(ScaleY) + "\n")
def OutlineWidth(objloc, Width):
	f.write("oulw :" + str(objloc) + ":" + str(Width) + "\n")

def OutlineColor(objloc, r, g, b):
	f.write("oulR :" + str(objloc) + ":" + str(r) + "\n")
	f.write("oulG :" + str(objloc) + ":" + str(g) + "\n")
	f.write("oulB :" + str(objloc) + ":" + str(b) + "\n")


def BindLocation(objloc, Texture):
	f.write("bt_l:" + str(objloc) + ":" + str(Texture) + "\n")




def done():
	os.remove("ov.rtsm")
