#include "stdafx.h"
#include "TyMemoryHelper.h"


DWORD getProcessHandleFromWideString(wchar_t* procName)
{
	DWORD flags = TH32CS_SNAPPROCESS;
	HANDLE snapHandle = CreateToolhelp32Snapshot(flags, NULL);
	//cout << snapHandle << " " << INVALID_HANDLE_VALUE << endl;
	LPPROCESSENTRY32 curProcess = (LPPROCESSENTRY32)malloc(sizeof(PROCESSENTRY32));
	curProcess->dwSize = sizeof(PROCESSENTRY32);



	bool found = false;
	if (Process32First(snapHandle, curProcess))
	{
		if (wcscmp(curProcess->szExeFile, procName) == 0)
		{
			//wcout << curProcess->szExeFile << endl;
			found = true;
		}

	}
	//loop through running processes looking for csgo
	while (!found)
	{
		//
		if (Process32Next(snapHandle, curProcess))
		{
			if (wcscmp(curProcess->szExeFile, procName) == 0)
			{
				//wcout << curProcess->szExeFile << endl;
				found = true;
			}

		}
		else if (GetLastError() == ERROR_NO_MORE_FILES)
		{
			cout << "Process not running!" << endl;
			return PROCESS_NOT_OPEN;
		}
		else
		{
			cout << "UNKOWN ERROR!" << endl;
			return UNKNOWN_PROC_ACQ_ERROR;
		}
	}

	//retrieve handle to process
	DWORD accessFlags = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE;
	cout << "Returning process handle to csgo.exe" << endl;
	return (DWORD)OpenProcess(accessFlags, FALSE, curProcess->th32ProcessID);
}

DWORD getProcessHandleFromPID(int pid)
{
	DWORD accessFlags = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE;
	return (DWORD)OpenProcess(accessFlags, FALSE, pid);
}

/*
INPUTS:
HANDLE hProcess - A handle to the process you wish to search
string moduleName - The name of the module you want a handle to
OUTPUT:
HMODULE retModule - A handle to the desired module within the process
RETURNS:
1 on success
MODULES_NOT_ENUMERATED if it fails to enumerate the modules
MODULE_NOT_FOUND if the desired module is not loaded in the process
*/
DWORD getModuleHandle(HANDLE hProcess, string moduleName, HMODULE& retModule)
{
	cout << "Module being searched for is: " << moduleName << endl;
	//create an array for 1000 modules
	HMODULE* lphModule = (HMODULE*)malloc(sizeof(HMODULE) * 1000);
	LPDWORD lpcbNeeded = (LPDWORD)malloc(sizeof(DWORD));

	//get a list of all of the modules in the csgo process
	if (EnumProcessModules(hProcess, lphModule, sizeof(HMODULE) * 1000, lpcbNeeded) == 0)
	{
		//enumeration failed
		cout << "MODULES NOT ENUMERATED!" << endl;
		return MODULES_NOT_ENUMERATED;
	}

	DWORD maxStringSize = 1000; //1000 bytes
	LPSTR lpFilename = (LPSTR)malloc(maxStringSize);

	//loop through all of the modules loaded in the process
	for (int i = 0; i < *(lpcbNeeded) / sizeof(HMODULE); i++)
	{
		//get the ascii absolute path of the module

		GetModuleFileNameExA(hProcess, *(lphModule + i), lpFilename, maxStringSize);
		//cout << "Current module being examined: " << (string)lpFilename << endl;
		//if the module path contains the name of the dll we are searching for (client_panorama.dll in this case)
		if (((string)lpFilename).find(moduleName) != string::npos && ((string)lpFilename).find("panorama") == string::npos && ((string)lpFilename).find("steamclient") == string::npos)
		{
			cout << "lpFilename is " << (string)lpFilename << endl;
			//this is the correct module so set it
			retModule = *(lphModule + i);
			return 1;
		}

	}
	cout << "MODULE NOT FOUND!!!" << endl;
	return MODULE_NOT_FOUND;
}

//gets the virtual address of the desired module
DWORD getModuleBaseAddress(HANDLE hProcess, HMODULE hMod, DWORD& baseAddress)
{
	LPMODULEINFO lpmodinfo = (LPMODULEINFO)malloc(sizeof(MODULEINFO));

	if (GetModuleInformation(hProcess, hMod, lpmodinfo, sizeof(MODULEINFO)) == 0)
	{
		cout << "Module info unavailable!" << endl;
		return MODULE_INFO_UNAVAILABLE;
	}

	baseAddress = (DWORD)lpmodinfo->lpBaseOfDll;
	cout << "Module base address is: " << baseAddress << endl;
	return 1;
}

void printInteger32Buffer(LPVOID buf, SIZE_T size)
{
	/*
	for (SIZE_T i = 0; i < size; i++)
	{
	cout << *((char*)buf + i);
	}*/
	cout << std::dec << *((int*)buf) << endl;
}

int readInt32(HANDLE procHandle, LPCVOID address)
{
	SIZE_T readSize = 4; //4 bytes for an integer
	LPVOID readBuf = (LPVOID)malloc(readSize);
	int value = -9999;

	if (ReadProcessMemory(procHandle, address, readBuf, readSize, NULL))
	{
		value = *((int*)readBuf);
		free(readBuf);
	}


	return value;
}

bool readBool(HANDLE procHandle, LPCVOID address)
{
	SIZE_T readSize = 1; //4 bytes for an integer
	LPVOID readBuf = (LPVOID)malloc(readSize);
	bool value = FALSE;

	if (ReadProcessMemory(procHandle, address, readBuf, readSize, NULL))
	{
		value = *((char*)readBuf);
		free(readBuf);
	}

	return value;
}

void writeFloat(HANDLE procHandle, LPVOID address, float value)
{
	SIZE_T writeSize = 4; //float is 4 bytes

	if (!WriteProcessMemory(procHandle, address, &value, writeSize, NULL))
	{
		return;
	}
}

void writeInt32(HANDLE procHandle, LPVOID address, int value)
{
	SIZE_T writeSize = 4; //int is 4 bytes

	if (!WriteProcessMemory(procHandle, address, &value, writeSize, NULL))
	{
		return;
	}
}

void writeBool(HANDLE procHandle, LPVOID address, bool value)
{
	SIZE_T writeSize = 1; //float is 4 bytes

	if (!WriteProcessMemory(procHandle, address, &value, writeSize, NULL))
	{
		return;
	}
}