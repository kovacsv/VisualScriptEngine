#include "WAS_CustomControl.hpp"
#include "NE_Debug.hpp"

#include <vector>

namespace WAS
{

class StaticWindowRegistrator
{
public:
	StaticWindowRegistrator () :
		classNames ()
	{
	}

	~StaticWindowRegistrator ()
	{
		for (const std::wstring& className : classNames) {
			bool unregResult = UnregisterClass (className.c_str (), NULL);
			if (DBGERROR (!unregResult)) {
				continue;
			}
		}
		classNames.clear ();
	}

	bool RegisterWindowClass (WNDPROC windowProc, LPCWSTR className)
	{
		if (std::find (classNames.begin (), classNames.end (), className) != classNames.end ()) {
			return true;
		}

		WNDCLASSEX windowClass;
		ZeroMemory (&windowClass, sizeof (WNDCLASSEX));
		windowClass.cbSize = sizeof (WNDCLASSEX);
		windowClass.style = 0;
		windowClass.lpfnWndProc = windowProc;
		windowClass.style = CS_DBLCLKS;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = NULL;
		windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
		windowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = className;

		ATOM atom = RegisterClassEx (&windowClass);
		if (DBGERROR (atom == 0)) {
			return false;
		}

		classNames.push_back (className);
		return true;
	}

private:
	std::vector<std::wstring> classNames;
};

static StaticWindowRegistrator windowRegistrator;

CustomControl::CustomControl () :
	windowClassName (),
	windowHandle (NULL)
{
}

CustomControl::~CustomControl ()
{
}

bool CustomControl::Init (HWND parentHandle, WNDPROC windowProc, LPCWSTR className, LPVOID lParam)
{
	bool windowRegistered = windowRegistrator.RegisterWindowClass (windowProc, className);
	if (DBGERROR (!windowRegistered)) {
		return false;
	};

	windowClassName = className;
	windowHandle = CreateWindowEx (
		0, windowClassName.c_str (), L"", WS_CHILD | WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, parentHandle, NULL, NULL, lParam
	);

	ShowWindow (windowHandle, SW_SHOW);
	UpdateWindow (windowHandle);

	return true;
}

HWND CustomControl::GetWindowHandle () const
{
	return windowHandle;
}

}
