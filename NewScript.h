
#pragma once
#include<iostream>
#include"Console.h"
#include"Object.h"
class NewScript {
public:
    void Start(Console& ovcon, std::vector<Object>& sceneObjects);
    void Update(Console& ovcon, std::vector<Object>& sceneObjects);
};
    