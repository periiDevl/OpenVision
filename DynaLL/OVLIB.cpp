#include "pch.h"
#include <utility>
#include <limits.h>
#include "OVLIB.h"
#include"Script.h"
#include"TestScript.h"
#include <fstream>
#include <sstream>
int sharedVar = 17;
std::vector<int> sharedArray;
std::string sharedString = "";

std::vector<Ov_Object> OVObjects = { };
Script script;
TestScript TestScriptscr;

void ScriptStart()
{
    OVObjects.clear();
    std::ifstream inputFile("runtimeconfig.ov");
    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
    }

    std::string line;
    int lineIndex = 0;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::vector<float> values;

        float val;
        while (iss >> val) {
            values.push_back(val);
        }

        if (values.size() == 4) {
            Ov_Object object;
            object.x = values[0];
            object.y = values[1];
            object.scale_x = values[2];
            object.scale_y = values[3];

            OVObjects.push_back(object);
        }
        else {
            std::cerr << "Error: Invalid data on line " << lineIndex + 1 << std::endl;
        }

        lineIndex++;
    }

    inputFile.close();

    script.Start();
    TestScriptscr.Start();
    std::cout << "Hello";

}
void ScriptUpdate()
{
    
    script.Update();
    TestScriptscr.Update();

}
void ScriptExit()
{
    script.Exit();
    TestScriptscr.Exit();

}


int GetSharedVar()
{
    return sharedVar;
}
int ObjectsSize()
{
    return OVObjects.size();
}
std::string GetSharedString()
{
    return sharedString;
}

Ov_Object GetOvObjectList(int i)
{
    return OVObjects[i];
}
Ov_Object GetSharedVarX(int i)
{
    return OVObjects[i];
}
/*
std::vector<int> GetShared()
{
    std::vector<int> hello = { 1 };
    return hello;
}
*/
/*
void GetSharedObject(std::vector<Ov_Object>& OVObjectss)
{
    OVObjectss = OVObjects;
}

void SetSharedObject(std::vector<Ov_Object> OVObjectss)
{
    OVObjects = OVObjectss;
}
*/
