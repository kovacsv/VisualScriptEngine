#include "WAS_InMemoryDialog.hpp"
#include "NE_Debug.hpp"

// https://blogs.msdn.microsoft.com/oldnewthing/20050429-00/?p=35743

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
		Write (psz, (lstrlen (psz) + 1) * sizeof (WCHAR));
	}

	void AlignToDword ()
	{
		if (bytes.size () % 4 != 0) {
			Write (nullptr, 4 - (bytes.size () % 4));
		}
	}

	LPCDLGTEMPLATE ConvertToTemplate ()
	{
		return (LPCDLGTEMPLATE) &bytes[0];
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
	helpId (0),
	extendedStyle (0),
	style (0),
	x (0),
	y (0),
	width (0),
	height (0),
	dialogTitle ()
{

}

DialogParameters::DialogParameters (const std::wstring& dialogTitle, WORD x, WORD y, WORD width, WORD height) :
	helpId (0),
	extendedStyle (0),
	style (WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME),
	x (x),
	y (y),
	width (width),
	height (height),
	dialogTitle (dialogTitle)
{

}

ControlParameters::ControlParameters () :
	helpId (0),
	extendedStyle (0),
	style (0),
	x (0),
	y (0),
	width (0),
	height (0),
	controlId (0),
	controlType (0)
{

}

ControlParameters::ControlParameters (DWORD controlType, DWORD style, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
	helpId (0),
	extendedStyle (0),
	style (WS_CHILD | WS_VISIBLE | style),
	x (x),
	y (y),
	width (width),
	height (height),
	controlId (controlId),
	controlType (controlType)
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
	StaticControl (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0082FFFF, WS_EX_CLIENTEDGE, x, y, width, height, controlId);
	}
};

class EditControl : public TextBasedControl
{
public:
	EditControl (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0081FFFF, WS_TABSTOP | WS_BORDER, x, y, width, height, controlId);
	}
};

class ButtonControl : public TextBasedControl
{
public:
	ButtonControl (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0080FFFF, WS_TABSTOP | BS_PUSHBUTTON, x, y, width, height, controlId);
	}
};

class DefButtonControl : public TextBasedControl
{
public:
	DefButtonControl (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
		TextBasedControl (controlText)
	{
		params = ControlParameters (0x0080FFFF, WS_TABSTOP | BS_DEFPUSHBUTTON, x, y, width, height, controlId);
	}
};

class ComboBoxControl : public InMemoryControl
{
public:
	ComboBoxControl (int selectedItem, const std::vector<std::wstring>& choices, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
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

InMemoryDialog::InMemoryDialog (const std::wstring& dialogTitle, WORD x, WORD y, WORD width, WORD height) :
	parameters (dialogTitle, x, y, width, height)
{
}

void InMemoryDialog::AddStatic (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new StaticControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddEdit (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new EditControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new ButtonControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddDefButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId)
{
	controls.push_back (std::unique_ptr<InMemoryControl> (new DefButtonControl (controlText, x, y, width, height, controlId)));
}

void InMemoryDialog::AddComboBox (int selectedItem, const std::vector<std::wstring>& choices, WORD x, WORD y, WORD width, WORD height, DWORD controlId)
{
	static const WORD comboBoxListHeight = 200;
	controls.push_back (std::unique_ptr<InMemoryControl> (new ComboBoxControl (selectedItem, choices, x, y, width, height + comboBoxListHeight, controlId)));
}

INT_PTR InMemoryDialog::Show (HWND parent, DLGPROC dialogProc, LPARAM initParam) const
{
	HDC hdc = GetDC (NULL);
	if (DBGERROR (hdc == NULL)) {
		return -1;
	}

	NONCLIENTMETRICS metrics = { sizeof (NONCLIENTMETRICS) };
	if (DBGERROR (!SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &metrics, 0))) {
		return -1;
	}

	DialogTemplateWriter writer;
	writer.Write<WORD> (1);
	writer.Write<WORD> (0xFFFF);
	writer.Write<DWORD> (parameters.helpId);
	writer.Write<DWORD> (parameters.extendedStyle);
	writer.Write<DWORD> (parameters.style);
	writer.Write<WORD> ((WORD) controls.size ());
	writer.Write<WORD> (parameters.x);
	writer.Write<WORD> (parameters.y);
	writer.Write<WORD> (parameters.width);
	writer.Write<WORD> (parameters.height);
	writer.WriteString (L"");
	writer.WriteString (L"");
	writer.WriteString (parameters.dialogTitle.c_str ());

	if (metrics.lfMessageFont.lfHeight < 0) {
		metrics.lfMessageFont.lfHeight = -MulDiv (metrics.lfMessageFont.lfHeight, 72, GetDeviceCaps (hdc, LOGPIXELSY));
	}
	writer.Write<WORD> ((WORD) metrics.lfMessageFont.lfHeight);
	writer.Write<WORD> ((WORD) metrics.lfMessageFont.lfWeight);
	writer.Write<BYTE> (metrics.lfMessageFont.lfItalic);
	writer.Write<BYTE> (metrics.lfMessageFont.lfCharSet);
	writer.WriteString (metrics.lfMessageFont.lfFaceName);

	for (const std::unique_ptr<InMemoryControl>& control : controls) {
		const ControlParameters& params = control->GetParameters ();
		writer.AlignToDword ();
		writer.Write<DWORD> (params.helpId);
		writer.Write<DWORD> (params.extendedStyle);
		writer.Write<DWORD> (params.style);
		writer.Write<WORD> (params.x);
		writer.Write<WORD> (params.y);
		writer.Write<WORD> (params.width);
		writer.Write<WORD> (params.height);
		writer.Write<DWORD> (params.controlId);
		writer.Write<DWORD> (params.controlType);
		writer.Write<WORD> (0);
	}
	writer.AlignToDword ();

	INT_PTR result = DialogBoxIndirectParam (NULL, writer.ConvertToTemplate (), parent, dialogProc, initParam);
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
