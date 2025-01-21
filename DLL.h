#ifndef DLL_H
#define DLL_H
#include<Windows.h>
#include "iostream"
#include "string"
class DLL
{
public:
	// define func  name                 param
	typedef void (*PrintStringFromMain)(const char* str);
	typedef const char* (*getStringFromDLL)();

	DLL();
	~DLL();
	void loadDLL(std::string name);
	const char* ReciveStringDLL();
	void SendStringToDll(std::string string);
	void clean() { FreeLibrary(hDLL); }


private:
	HMODULE hDLL;
};

#endif