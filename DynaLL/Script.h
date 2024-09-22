



#include <iostream>
#include"pch.h"
#include"OVLIB.h"

class Script {

public:
	Ov_Object nig1;
	Ov_Object nig2;
	bool start = false;
	void Start() {
		
		std::cout << "Hello!" << std::endl;
		nig1.x = 1;
		nig1.y = 0;
		nig1.scale_x = 5;
		nig1.scale_y = 5;

		nig2.x = 8;
		nig2.y = 0;
		nig2.scale_x = 5;
		nig2.scale_y = 5;
		AddToPool(nig1);
		AddToPool(nig2);

	}
	void Update() {

		// Main simulation loop
				// Create the collision configuration
		OVObjects[nig1.index].x += 0.1;
		OVObjects[nig1.index].y = 0;
		OVObjects[nig1.index].scale_x = 10;
		OVObjects[nig1.index].scale_y = 10;
		
		OVObjects[nig2.index].x = 8;
		OVObjects[nig2.index].y = 0;
		OVObjects[nig2.index].scale_x = 0;
		OVObjects[nig2.index].scale_y = 0;
		
		
	}
	void Exit() {

	}


};

