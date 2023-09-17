
#pragma once
#include<iostream>
#include"OVLIB.h"

/*
#include"OV.h"
#include"Console.h"
#include"Object.h"
#include"InputSystem.h"
*/

#include"pch.h"

class PW {
public:
	void Start() {

		/*
		for (size_t i = 0; i < OVObjects.size(); i++)
		{
			std::cout << OVObjects[i].x;
		}
		/*
		OVObjects[0].scale_x = 100;
		*/
		int sharedVar = GetSharedVar();
		std::cout << sharedVar;
		//sharedVar = 777;
		//SetSharedVar(sharedVar);
		//std::cout << sharedVar;
	}
	void Update() {
		//std::cout << OVObjects[1].scale_x;
		OVObjects[1] = Ov_Object{0, 0, 0, 0};
	}
	void Exit() {

	}
};
    