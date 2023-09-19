
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
		std::cout << OVObjects[0].x;
		
		
		/*
		/*
		for (size_t i = 0; i < OVObjects.size(); i++)
		{
			std::cout << OVObjects[i].x;
		}
		/*
		int sharedVar = GetSharedVar();
		std::cout << sharedVar;

		std::vector<Ov_Object> obj;
		GetSharedObject(obj);
		obj.push_back(Ov_Object{ 10,10,10,10, 10 });
		obj.push_back(Ov_Object{ 10,10,10,10, 10 });
		obj.push_back(Ov_Object{ 10,10,10,10, 10 });

		SetSharedObject(obj);
		*/
		//Ov_Object sharedObject = GetSharedObject();
		//SetSharedObject(Ov_Object{ 1, 100, 100, 100, 100 });
		//sharedVar = 777;
		//SetSharedVar(sharedVar);
		//std::cout << sharedVar;
	}
	void Update() {
		//std::cout << OVObjects[1].scale_x;
	}
	void Exit() {

	}
};
    