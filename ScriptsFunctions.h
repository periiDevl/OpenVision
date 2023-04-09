#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include "Object.h"


class Scripting
{
public:
    Scripting()
    {

    }
    std::tuple<int, float, bool> SetPosX(const std::string& filename);

    std::tuple<int, float, bool> SetPosY(const std::string& filename);

    void LoadTranslation(std::string filename, std::vector<Object> sceneObjects);
};