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
	std::wstring stringToWString(const std::string& str) {
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		std::wstring wstr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size_needed);
		return wstr;
	}

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