#pragma once
#include "stdafx.h"

using namespace std;

//ERROR DEFINITIONS
#define PROCESS_NOT_OPEN 0xFFFF
#define UNKNOWN_PROC_ACQ_ERROR 0xDDDD
#define MODULES_NOT_ENUMERATED 0xEEEE;
#define MODULE_NOT_FOUND 0xEFEF;
#define MODULE_INFO_UNAVAILABLE 0xFEFE;


DWORD getProcessHandleFromWideString(wchar_t* procName);

DWORD getProcessHandleFromPID(int PID);

DWORD getModuleHandle(HANDLE hProcess, string moduleName, HMODULE& retModule);

DWORD getModuleBaseAddress(HANDLE hProcess, HMODULE hMod, DWORD& baseAddress);

void printInteger32Buffer(LPVOID buf, SIZE_T size);

int readInt32(HANDLE procHandle, LPCVOID address);

bool readBool(HANDLE procHandle, LPCVOID address);

void writeFloat(HANDLE procHandle, LPVOID address, float value);

void writeInt32(HANDLE procHandle, LPVOID address, int value);

void writeBool(HANDLE procHandle, LPVOID address, bool value);