def PosX(objloc, PosX):
	f = open("ov.rtsm", "a")
	f.write("stox :" + str(objloc) + ":" + str(PosX) + "\n")
	f.close()
def PosY(objloc, PosX):
	f = open("ov.rtsm", "a")
	f.write("stoy :" + str(objloc) + ":" + str(PosX) + "\n")
	f.close()