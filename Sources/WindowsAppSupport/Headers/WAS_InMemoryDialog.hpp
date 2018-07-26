#ifndef INMEMORYDIALOG_HPP
#define INMEMORYDIALOG_HPP

#include <windows.h>
#include <string>
#include <vector>

namespace WAS
{

class InMemoryDialog
{
public:
	struct DialogParameters
	{
		DialogParameters ();

		DWORD			helpId;
		DWORD			extendedStyle;
		DWORD			style;
		WORD			x;
		WORD			y;
		WORD			width;
		WORD			height;
		std::wstring	dialogTitle;
	};

	struct ControlParameters
	{
		ControlParameters ();

		DWORD			helpId;
		DWORD			extendedStyle;
		DWORD			style;
		WORD			x;
		WORD			y;
		WORD			width;
		WORD			height;
		DWORD			controlId;
		std::wstring	controlText;
	};

	InMemoryDialog (const DialogParameters& parameters);

	void	AddControl (const ControlParameters& parameters);
	bool	Show (HWND hwnd, DLGPROC dialogProc) const;

private:
	DialogParameters				parameters;
	std::vector<ControlParameters>	controls;
};

}

#endif
