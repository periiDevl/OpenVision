#include"DLL.h"
#include "string"

DLL::DLL()
{
	
}
void DLL::loadDLL(std::string name) {
    std::wstring wname = stringToWString(name);
    hDLL = LoadLibrary(wname.c_str());

    if (!hDLL) {
        std::cout << "Failed to load DLL: " << name << "\n";
        std::cout << "Error code: " << GetLastError() << "\n";
        return;
    }
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