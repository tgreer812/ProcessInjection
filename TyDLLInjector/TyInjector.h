#pragma once
#include "stdafx.h"

class TyInjector
{
private:

public:

	// Attributes
	wchar_t* targetProcessName;

	// Constructors
	TyInjector(string targetProcessName);

	// Methods
	void setTargetProcess(string targetProcessName);
	bool injectDLL(wchar_t* dllName);
	bool injectDLL(wchar_t* dllName, HANDLE targetProcess);
	bool injectShellcode(LPCVOID shellcodeBuffer, SIZE_T bufferLength);
	bool injectShellcode(LPCVOID shellcodeBuffer, SIZE_T bufferLength, HANDLE targetProcess);
};