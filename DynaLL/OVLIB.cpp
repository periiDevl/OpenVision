#include "pch.h"
#include <utility>
#include <limits.h>
#include "OVLIB.h"
#include"Script.h"
#include"PW.h"
int sharedVar = 17;

std::vector<Ov_Object> OVObjects;
Script script;
PW PWscr;

void ScriptStart()
{
    OVObjects[1].x = 1000;
    script.Start();
    PWscr.Start();
    std::cout << "Hello";

}
void ScriptUpdate()
{
    
    script.Update();
    PWscr.Update();

}
void ScriptExit()
{
    script.Exit();
    PWscr.Exit();

}
void SetSharedVar(int value)
{
    sharedVar = value;
}

int GetSharedVar()
{
    return sharedVar;
}
