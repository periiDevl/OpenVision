
#pragma once
#include<iostream>
#include"Console.h"
#include"Object.h"
#include "OV.h"
class TestingScript {
public:
    void Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects);
    void Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects);
};
    