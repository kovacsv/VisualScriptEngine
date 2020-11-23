#include "WAS_InMemoryDialog.hpp"
#include "NE_Debug.hpp"

// https://devblogs.microsoft.com/oldnewthing/20050429-00/?p=35743
// https://docs.microsoft.com/en-us/windows/win32/dlgbox/dlgtemplateex
// https://docs.microsoft.com/en-us/windows/win32/dlgbox/dlgitemtemplateex

namespace WAS
{

class DialogTemplateWriter
{
public:
	DialogTemplateWriter ()
	{

	}

	template <typename T>
	void Write (const T& data)
	{
		Write (&data, sizeof (data));
	}

	void WriteString (LPCWSTR psz)
	{
		Write (psz, ((DWORD) wcslen (psz) + 1) * sizeof (WCHAR));
	}

	void AlignToDword ()
	{
		if (bytes.size () % 4 != 0) {
			Write (nullptr, 4 - (bytes.size () % 4));
		}
	}

	LPCDLGTEMPLATE ConvertToTemplate ()
	{
		return (LPCDLGTEMPLATE) bytes.data ();
	}

private:
	void Write (LPCVOID data, DWORD size)
	{
		bytes.insert (bytes.end (), size, 0);
		if (data != nullptr) {
			CopyMemory (&bytes[bytes.size () - size], data, size);
		}
	}

	std::vector<BYTE> bytes;
};

DialogParameters::DialogParameters () :
	DialogParameters (L"", 0, 0, 0, 0)
{

}

DialogParameters::DialogParameters (const std::wstring& dialogTitle, short x, short y, short width, short height) :
	dialogTitle (dialogTitle),
	x (x),
	y (y),
	width (width),
	height (height)
{

}

ControlParameters::ControlParameters () :
	ControlParameters (0, 0, 0, 0, 0, 0, 0)
{

}

ControlParameters::ControlParameters (DWORD controlType, DWORD style, short x, short y, short width, short height, DWORD controlId) :
	controlType (controlType),
	style (WS_CHILD | WS_VISIBLE | style),
	x (x),
	y (y),
	width (width),
	height (height),
	controlId (controlId)
{

}

InMemoryControl::InMemoryControl () :
	params (),
	hwnd (NULL)
{

}

InMemoryControl::~InMemoryControl ()
{

}

DWORD InMemoryControl::GetId () const
{
	return params.controlId;
}

const ControlParameters& InMemoryControl::GetParameters () const
{
	return params;
}

void InMemoryControl::Setup (HWND controlHwnd)
{
	hwnd = controlHwnd;
	SetupControl ();
}

class TextBasedControl : public InMemoryControl
{
public:
	TextBasedControl (const std::wstring& controlText) :
		controlText (controlText)
	{
	}

	virtual void SetupControl () override
	{
		SetWindowText (hwnd, controlText.c_str ());
	}

private:
	std::wstring controlText;
};

class StaticControl : public TextBasedControl
{
public:
	StaticControl (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0082FFFF, WS_EX_CLIENTEDGE, x, y, width, height, controlId);
	}
};

class EditControl : public TextBasedControl
{
public:
	EditControl (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0081FFFF, WS_TABSTOP | WS_BORDER, x, y, width, height, controlId);
	}
};

class ButtonControl : public TextBasedControl
{
public:
	ButtonControl (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0080FFFF, WS_TABSTOP | BS_PUSHBUTTON, x, y, width, height, controlId);
	}
};

class DefButtonControl : public TextBasedControl
{
public:
	DefButtonControl (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0080FFFF, WS_TABSTOP | BS_DEFPUSHBUTTON, x, y, width, height, controlId);
	}
};

class ComboBoxControl : public InMemoryControl
{
public:
	ComboBoxControl (int selectedItem, const std::vector<std::wstring>& choices, short x, short y, short width, short height, DWORD controlId) :
		InMemoryControl (),
		selectedItem (selectedItem),
		choices (choices)
	{
		params = ControlParameters (0x0085FFFF, WS_TABSTOP | WS_BORDER | CBS_DROPDOWNLIST, x, y, width, height, controlId);
	}

	virtual void SetupControl () override
	{
		for (const std::wstring& choice : choices) {
			SendMessage (hwnd, CB_ADDSTRING, 0, (LPARAM) choice.c_str ());
		}
		SendMessage (hwnd, CB_SETCURSEL, (WPARAM) selectedItem, 0);
	}

private:
	int selectedItem;
	std::vector<std::wstring> choices;
};

class SeparatorControl : public InMemoryControl
{
public:
	SeparatorControl (short x, short y, short width, DWORD controlId) :
		InMemoryControl ()
	{
		params = ControlParameters (0x0082FFFF, WS_EX_CLIENTEDGE | SS_ETCHEDHORZ, x, y, width, 1, controlId);
	}

	virtual void SetupControl () override
	{
		
	}
};

InMemoryDialog::InMemoryDialog (const std::wstring& dialogTitle, short x, short y, short width, short height) :
	parameters (dialogTitle, x, y, width, height)
{
}

void InMemoryDialog::AddStatic (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new StaticControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddEdit (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new EditControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddButton (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new ButtonControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddDefButton (const std::wstring& controlText, short x, short y, short width, short height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new DefButtonControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddComboBox (int selectedItem, const std::vector<std::wstring>& choices, short x, short y, short width, short height, DWORD controlId)
{
	static const short comboBoxListHeight = 200;
	controls.push_back (std::unique_ptr<InMemoryControl> (new ComboBoxControl (selectedItem, choices, x, y, width, height + comboBoxListHeight, controlId)));
}

void InMemoryDialog::AddSeparator (short x, short y, short width, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new SeparatorControl (x, y, width, controlId)));
}

INT_PTR InMemoryDialog::Show (HWND parentHwnd, DLGPROC dialogProc, LPARAM initParam) const
{
	HDC hdc = GetDC (NULL);
	if (DBGERROR (hdc == NULL)) {
		return -1;
	}

	NONCLIENTMETRICS metrics = { sizeof (NONCLIENTMETRICS) };
	if (DBGERROR (!SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &metrics, 0))) {
		return -1;
	}
	if (metrics.lfMessageFont.lfHeight < 0) {
		metrics.lfMessageFont.lfHeight = -MulDiv (metrics.lfMessageFont.lfHeight, 72, GetDeviceCaps (hdc, LOGPIXELSY));
	}

	DialogTemplateWriter writer;
	writer.Write<WORD> (1); // dlgVer
	writer.Write<WORD> (0xFFFF); // signature
	writer.Write<DWORD> (0); // helpID
	writer.Write<DWORD> (0); // exStyle
	writer.Write<DWORD> (WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME); // style
	writer.Write<WORD> ((WORD) controls.size ()); // cDlgItems
	writer.Write<short> (parameters.x); // x
	writer.Write<short> (parameters.y); // y
	writer.Write<short> (parameters.width); // cx
	writer.Write<short> (parameters.height); // cy
	writer.WriteString (L""); // menu
	writer.WriteString (L""); // windowClass
	writer.WriteString (parameters.dialogTitle.c_str ()); // title
	writer.Write<WORD> ((WORD) metrics.lfMessageFont.lfHeight); // pointsize
	writer.Write<WORD> ((WORD) metrics.lfMessageFont.lfWeight); // weight
	writer.Write<BYTE> (metrics.lfMessageFont.lfItalic); // italic
	writer.Write<BYTE> (metrics.lfMessageFont.lfCharSet); // charset
	writer.WriteString (metrics.lfMessageFont.lfFaceName); // typeface

	for (const std::unique_ptr<InMemoryControl>& control : controls) {
		const ControlParameters& params = control->GetParameters ();
		writer.AlignToDword ();
		writer.Write<DWORD> (0); // helpID
		writer.Write<DWORD> (0); // exStyle
		writer.Write<DWORD> (params.style); // style
		writer.Write<short> (params.x); // x
		writer.Write<short> (params.y); // y
		writer.Write<short> (params.width); // cx
		writer.Write<short> (params.height); // cy
		writer.Write<DWORD> (params.controlId); // id
		writer.Write<DWORD> (params.controlType); // windowClass
		writer.Write<WORD> (0); // title
		writer.Write<WORD> (0); // extraCount
	}
	writer.AlignToDword ();

	INT_PTR result = DialogBoxIndirectParam (NULL, writer.ConvertToTemplate (), parentHwnd, dialogProc, initParam);
	ReleaseDC (NULL, hdc);
	return result;
}

void InMemoryDialog::SetupControls (HWND dialogHwnd)
{
	for (const std::unique_ptr<InMemoryControl>& control : controls) {
		HWND controlHwnd = GetDlgItem (dialogHwnd, control->GetId ());
		control->Setup (controlHwnd);
	}
}

}
