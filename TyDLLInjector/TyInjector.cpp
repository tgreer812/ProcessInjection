#include "stdafx.h"
#include "TyInjector.h"

TyInjector::TyInjector()
{

}


bool TyInjector::injectDLL(wchar_t* dllName, HANDLE targetProcess)
{
	int length = (int)wcslen(dllName) + 1;

	//Allocate space for the name of the DLL to inject in the remote process
	LPVOID buffer;
	if (!(buffer = VirtualAllocEx(targetProcess, NULL, length * 2, MEM_COMMIT, PAGE_EXECUTE)))
	{
		return false;
	}

	//Write the name of the DLL to inject into the newly allocated buffer
	if (!WriteProcessMemory(targetProcess, buffer, dllName, length * 2, NULL))
	{
		return false;
	}
	
	//Obtain the address of kernel32 - kernel32 gets allocated at the same address for every process on the Windows OS
	HMODULE k32;
	if (!(k32 = GetModuleHandleA("kernel32.dll")))
	{
		return false;
	}

	//Get the address of LoadLibraryW within kernel32
	LPVOID loadLibraryW;
	if (!(loadLibraryW = GetProcAddress(k32, "LoadLibraryW")))
	{
		return false;
	}

	//Create a remote thread in the target process and have it call LoadLibraryW on the DLL to inject
	HANDLE remoteThread;
	if (!(remoteThread = CreateRemoteThread(targetProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)loadLibraryW, buffer, NULL, NULL)))
	{
		return false;
	}

	return true;
}