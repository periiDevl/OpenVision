#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <utility>
#include "Object.h"
#include "Tex.h"


class Scripting
{
public:
    Scripting()
    {

    }


    std::tuple<int, float, bool> SetFloat(const std::string& filename, std::string keyword);
    std::tuple<int, std::string, bool> SetString(const std::string& filename, std::string keyword);


    void Load(std::string filename, std::vector<Object>& sceneObjects, std::vector<Texture>& textures);


    void Relase(std::string filename)
    {
        std::ofstream ofs(filename, std::ios::out);
    }
private:
    void MicroLoadF(std::vector<Object>& sceneObjects, std::string filename, std::string keyword, float& value);
    void MicroLoadLocationTex(std::vector<Object>& sceneObjects, std::string filename, std::string keyword, std::vector<Texture>& textures);
    int first;
    float second;
    bool exists;

};