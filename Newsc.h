
#pragma once
#include<iostream>
#include"Console.h"
#include"Object.h"
class Newsc {
public:
    void Start(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects);
    void Update(Console& ovcon, GLFWwindow* window, PhysicsWorld& world, std::vector<Object>& sceneObjects);
};
    