#pragma once
#include"Console.h"
#include"Object.h"
#include "SaveSystem.h"
#include "InputSystem.h"
class Script {
public:
	float speed = 10;
	Object* player = nullptr;
	Object* ground = nullptr;
	Object* ground2 = nullptr;
	Object* ground_detection = nullptr;
	void Start (Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);
	void Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);

private:
	SaveSystem SavingSystem;
};