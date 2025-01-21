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
	std::string name;
	float x, y, scale_x, scale_y, index;
	bool acc = false;
};
//Testing String ret
extern "C" {
	_declspec(dllexport) const char* getStringFromDLL() {
		static std::string str = "Hello";
		return str.c_str();
	}
	__declspec(dllexport) void printStringFromMain(const char* str) {
		std::cout << "Received string in DLL: " << str << std::endl;
	}
}


extern "C" OVLIB_API void ScriptStart();
extern "C" OVLIB_API void ScriptUpdate();
extern "C" OVLIB_API void ScriptExit();
extern "C" OVLIB_API std::vector<Ov_Object> OVObjects;

__declspec(dllimport) int global_var;

extern std::vector<int> sharedArray;

extern std::string sharedString;



extern "C" OVLIB_API int GetSharedVar();
extern "C" OVLIB_API std::string GetSharedString();

extern "C" OVLIB_API Ov_Object GetSharedVarX(int i);
extern "C" OVLIB_API int ObjectsSize();
Ov_Object GetOV(std::string Oname)
{
	for (size_t i = 0; i < ObjectsSize(); i++)
	{
		if (OVObjects[i].name == Oname) {
			return OVObjects[i];
		}
	}
}
Ov_Object GetOV(int index)
{
	return OVObjects[index];
}
// Adds the object to the pool and assigns its index
void AddToPool(Ov_Object& object) {
	OVObjects.push_back(object);          // Add object to the vector
	object.index = OVObjects.size() - 1;  // Set index to the current size (new index)
}


// Modifies the object from the pool
void UseFromPool(Ov_Object& object) {
	OVObjects[object.index] = object;
	object.acc = true;

}

// Modifies the pointer to refer to the object in the pool
void CreateObject(Ov_Object& object) {
	AddToPool(object);  // Add the object by reference
	 // Update the object from the pool
}

//extern "C" OVLIB_API std::vector<int> GetShared();
/*
extern "C" OVLIB_API void GetSharedObject(std::vector<Ov_Object>&OVObjectss);
extern "C" OVLIB_API void SetSharedObject(std::vector<Ov_Object> OVObjectss);
*/


