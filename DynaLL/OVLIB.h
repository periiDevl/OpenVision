#pragma once
#include <iostream>
#include<vector>
#ifdef OVLIB_EXPORTS
#define OVLIB_API __declspec(dllexport)
#else
#define OVLIB_API __declspec(dllimport)
#endif

struct Ov_Object
{
	float x, y, scale_x, scale_y;
};
extern "C" OVLIB_API void ScriptStart();
extern "C" OVLIB_API void ScriptUpdate();
extern "C" OVLIB_API void ScriptExit();
extern "C" OVLIB_API std::vector<Ov_Object> OVObjects;
extern int sharedVar;

extern std::vector<int> sharedArray;

extern std::string sharedString;



extern "C" OVLIB_API int GetSharedVar();
extern "C" OVLIB_API std::string GetSharedString();

extern "C" OVLIB_API Ov_Object GetSharedVarX(int i);
extern "C" OVLIB_API int ObjectsSize();

//extern "C" OVLIB_API std::vector<int> GetShared();
/*
extern "C" OVLIB_API void GetSharedObject(std::vector<Ov_Object>&OVObjectss);
extern "C" OVLIB_API void SetSharedObject(std::vector<Ov_Object> OVObjectss);
*/


