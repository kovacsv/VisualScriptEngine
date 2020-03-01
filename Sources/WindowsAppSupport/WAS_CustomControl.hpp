#ifndef WAS_CUSTOMCONTROL_HPP
#define WAS_CUSTOMCONTROL_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include <string>

namespace WAS
{

class CustomControl
{
public:
	CustomControl ();
	~CustomControl ();

	bool	Init (HWND parentHandle, WNDPROC windowProc, LPCWSTR className, LPVOID lParam);
	HWND	GetWindowHandle () const;
	
private:
	std::wstring	windowClassName;
	HWND			windowHandle;
};

}

#endif
