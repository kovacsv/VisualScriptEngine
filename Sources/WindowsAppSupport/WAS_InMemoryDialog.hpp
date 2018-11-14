#ifndef WAS_INMEMORYDIALOG_HPP
#define WAS_INMEMORYDIALOG_HPP

#include "WAS_IncludeWindowsHeaders.hpp"

#include <string>
#include <vector>
#include <memory>

namespace WAS
{

struct DialogParameters
{
	DialogParameters ();
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
	ControlParameters ();
	ControlParameters (DWORD controlType, DWORD style, WORD x, WORD y, WORD width, WORD height, DWORD controlId);

	DWORD			helpId;
	DWORD			extendedStyle;
	DWORD			style;
	WORD			x;
	WORD			y;
	WORD			width;
	WORD			height;
	DWORD			controlId;
	DWORD			controlType;
};

class InMemoryControl
{
public:
	InMemoryControl ();
	virtual ~InMemoryControl ();

	DWORD						GetId () const;
	const ControlParameters&	GetParameters () const;

	void						Setup (HWND controlHwnd);
	virtual void				SetupControl () = 0;

protected:
	ControlParameters	params;
	HWND				hwnd;
};

class InMemoryDialog
{
public:
	InMemoryDialog (const std::wstring& dialogTitle, WORD x, WORD y, WORD width, WORD height);

	void		AddStatic (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddEdit (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddDefButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId);
	void		AddComboBox (int selectedItem, const std::vector<std::wstring>& choices, WORD x, WORD y, WORD width, WORD height, DWORD controlId);

	INT_PTR		Show (HWND parent, DLGPROC dialogProc, LPARAM initParam) const;
	void		SetupControls (HWND dialogHwnd);

private:
	DialogParameters								parameters;
	std::vector<std::unique_ptr<InMemoryControl>>	controls;
};

}

#endif
