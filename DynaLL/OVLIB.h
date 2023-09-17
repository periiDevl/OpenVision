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

extern "C" OVLIB_API void SetSharedVar(int value);

extern "C" OVLIB_API int GetSharedVar();


