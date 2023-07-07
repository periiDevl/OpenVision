
#pragma once
#include<iostream>
#include"Console.h"
#include"Object.h"
#include"InputSystem.h"
#include"OV.h"
class Anewscript {
public:
	void Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects,Camera& camera);
	void Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects,Camera& camera);

};
    