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
	DialogParameters (const std::wstring& dialogTitle, short x, short y, short width, short height);

	std::wstring	dialogTitle;
	short			x;
	short			y;
	short			width;
	short			height;
};

struct ControlParameters
{
	ControlParameters ();
	ControlParameters (DWORD controlType, DWORD style, short x, short y, short width, short height, DWORD controlId);

	DWORD			controlType;
	DWORD			style;
	short			x;
	short			y;
	short			width;
	short			height;
	DWORD			controlId;
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
	enum class Status
	{
		Draft,
		Opened,
		Initialized
	};

	InMemoryDialog ();

	void		SetTitle (const std::wstring& dialogTitle);
	void		SetRect (short x, short y, short width, short height);
	void		AddStatic (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId);
	void		AddEdit (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId);
	void		AddButton (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId);
	void		AddDefButton (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId);
	void		AddComboBox (int selectedItem, const std::vector<std::wstring>& choices, short x, short y, short width, short height, DWORD controlId);
	void		AddSeparator (short x, short y, short width, DWORD controlId);

	INT_PTR		Show (HWND parentHwnd, DLGPROC dialogProc, LPARAM initParam);
	void		InitControls (HWND dialogHwnd);
	Status		GetStatus () const;

private:
	DialogParameters								parameters;
	std::vector<std::unique_ptr<InMemoryControl>>	controls;
	Status											status;
};

}

#endif
