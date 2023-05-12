#pragma once
#include"Console.h"
#include"Object.h"
#include "SaveSystem.h"
#include"InputSystem.h"
#include "OV.h"
class Script {
public:

	void Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);
	void Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);


};