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

    std::tuple<int, float, bool> SetFloat(const std::string& filename, std::string keyword);



    void Load(std::string filename, std::vector<Object>& sceneObjects);


    void Relase(std::string filename)
    {
        std::ofstream ofs(filename, std::ios::out);
    }
private:
    void MicroLoadF(std::vector<Object>& sceneObjects, std::string filename, std::string keyword, float& value);
    int first;
    float second;
    bool exists;

};