#ifndef INMEMORYDIALOG_HPP
#define INMEMORYDIALOG_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

#include <string>
#include <vector>

namespace WAS
{

class InMemoryDialog
{
public:
	InMemoryDialog (const std::wstring& dialogTitle, WORD x, WORD y, WORD width, WORD height);

	void		AddStatic (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddEdit (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddDefButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);

	INT_PTR		Show (HWND hwnd, DLGPROC dialogProc, LPARAM initParam) const;

private:
	struct DialogParameters
	{
		DialogParameters (const std::wstring& dialogTitle, WORD x, WORD y, WORD width, WORD height);

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
		ControlParameters (DWORD controlType, const std::wstring& controlText, DWORD style, WORD x, WORD y, WORD width, WORD height, DWORD controlId);

		DWORD			helpId;
		DWORD			extendedStyle;
		DWORD			style;
		WORD			x;
		WORD			y;
		WORD			width;
		WORD			height;
		DWORD			controlId;
		DWORD			controlType;
		std::wstring	controlText;
	};

	DialogParameters				parameters;
	std::vector<ControlParameters>	controls;
};

}

#endif
