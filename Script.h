#pragma once
#include"Console.h"
#include"Object.h"
class Script {
public:
	void Start(Console& ovcon, std::vector<Object>& sceneObjects);
	void Update(Console& ovcon, std::vector<Object>& sceneObjects);
};