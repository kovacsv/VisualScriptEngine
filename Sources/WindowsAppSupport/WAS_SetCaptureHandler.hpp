#ifndef WAS_SETCAPTUREHANDLER_HPP
#define WAS_SETCAPTUREHANDLER_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

class SetCaptureHandler
{
public:
	SetCaptureHandler ();

	void HandleMouseDown (HWND hwnd);
	void HandleMouseUp ();

private:
	int counter;
};

}

#endif
