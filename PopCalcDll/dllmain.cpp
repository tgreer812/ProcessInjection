
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>

#define POPCALC_ON_LOAD
extern "C" __declspec(dllexport) void PopCalc()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//herpedy derp
	CreateProcess(
		TEXT("C:\\Windows\\System32\\calc.exe"),
		(LPWSTR)TEXT(""),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

/**
	This is DLL main
	I leave such amazing comments
*/
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
#ifdef POPCALC_ON_LOAD
		PopCalc();
#endif
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	default:
		break;
	}

	return true;
}
