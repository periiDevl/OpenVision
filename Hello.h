
#pragma once
#include<iostream>
#include"Console.h"
#include"Object.h"
#include"InputSystem.h"
#include"OV.h"
class Hello {
public:
    void Start(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);
    void Update(Console& ovcon, InputSystem Input, PhysicsWorld& world, std::vector<Object>& sceneObjects);
};
    