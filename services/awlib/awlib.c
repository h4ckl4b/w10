#include <windows.h>

BOOL WINAPI DllMainCRTStartup(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved){
	return TRUE;
}

int __declspec( dllexport ) __stdcall Inc(int a){
	return a + 1;
}

int __declspec( dllexport ) __stdcall Hello(){
	MessageBox(0, "Hello from this awesome library!", "Hello", MB_OK);
	return 0;
}

