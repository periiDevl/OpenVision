#include"DLL.h"
#include "string"

DLL::DLL()
{
	
}

void DLL::loadDLL(std::string name) {

	LPCWSTR libname = LPCWSTR(name.c_str());
	hDLL = LoadLibrary(libname);
	if (!hDLL) { std::cout << "Failed to load dll";  return; }

}

const char* DLL::ReciveStringDLL() {

	getStringFromDLL getStringDll = (getStringFromDLL)GetProcAddress(hDLL, "getStringFromDLL");
	const char* stri = getStringDll();
	return stri;
}

void DLL::SendStringToDll(std::string string) {
	PrintStringFromMain printStringFromMain = (PrintStringFromMain)GetProcAddress(hDLL, "printStringFromMain");
	printStringFromMain(string.c_str());

}

DLL::~DLL()
{

}