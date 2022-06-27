#pragma once


class TyInjector
{
private:

public:
	TyInjector();

	bool injectDLL(wchar_t* dllName, HANDLE targetProcess);
};