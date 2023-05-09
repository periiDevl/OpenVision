#pragma once
#include"Console.h"
#include"Object.h"
class Script {
public:
	float speed = 10;
	void Start (Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects);
	void Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects);
};