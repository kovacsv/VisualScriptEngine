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

InMemoryDialog::DialogParameters::DialogParameters (const std::wstring& dialogTitle, WORD width, WORD height) :
	helpId (0),
	extendedStyle (0),
	style (WS_CAPTION | WS_SYSMENU | DS_SETFONT | DS_MODALFRAME),
	x (0),
	y (0),
	width (width),
	height (height),
	dialogTitle (dialogTitle)
{

}

InMemoryDialog::ControlParameters::ControlParameters (DWORD controlType, const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId) :
	helpId (0),
	extendedStyle (0),
	style (WS_CHILD | WS_VISIBLE | WS_GROUP | WS_TABSTOP | BS_DEFPUSHBUTTON),
	x (x),
	y (y),
	width (width),
	height (height),
	controlId (controlId),
	controlType (controlType),
	controlText (controlText)
{

}

InMemoryDialog::InMemoryDialog (const std::wstring& dialogTitle, WORD width, WORD height) :
	parameters (dialogTitle, width, height)
{
}

void InMemoryDialog::AddButton (const std::wstring& controlText, WORD x, WORD y, WORD width, WORD height, DWORD controlId)
{
	ControlParameters parameters (0x0080FFFF, controlText, x, y, width, height, controlId);
	controls.push_back (parameters);
}

bool InMemoryDialog::Show (HWND hwnd, DLGPROC dialogProc) const
{
	HDC hdc = GetDC (NULL);
	if (DBGERROR (hdc == NULL)) {
		return false;
	}

	NONCLIENTMETRICS metrics = { sizeof (NONCLIENTMETRICS) };
	if (DBGERROR (!SystemParametersInfo (SPI_GETNONCLIENTMETRICS, 0, &metrics, 0))) {
		return false;
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

	for (const ControlParameters& control : controls) {
		writer.AlignToDword ();
		writer.Write<DWORD> (control.helpId);
		writer.Write<DWORD> (control.extendedStyle);
		writer.Write<DWORD> (control.style);
		writer.Write<WORD> (control.x);
		writer.Write<WORD> (control.y);
		writer.Write<WORD> (control.width);
		writer.Write<WORD> (control.height);
		writer.Write<DWORD> (control.controlId);
		writer.Write<DWORD> (control.controlType);
		writer.WriteString (control.controlText.c_str ());
		writer.Write<WORD> (0);
	}
	writer.AlignToDword ();

	DialogBoxIndirect (NULL, writer.ConvertToTemplate (), hwnd, dialogProc);
	ReleaseDC (NULL, hdc);
	return true;
}

}
