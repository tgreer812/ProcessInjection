#include "stdafx.h"

#include "TyStringHelper.h"

const wchar_t* getWideCFromStdString(std::string str)
{
	std::wstring std_wide = std::wstring(str.begin(), str.end());
	const wchar_t* c_wide = std_wide.c_str();
	return c_wide;
}