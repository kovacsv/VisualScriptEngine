#include "WAS_ParametersDialog.hpp"
#include "WAS_InMemoryDialog.hpp"
#include "NE_Debug.hpp"

#include <windowsx.h>

namespace WAS
{

INT_PTR CALLBACK DlgProc (HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
	switch (wm) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
	{
		DWORD a = GET_WM_COMMAND_ID (wParam, lParam);
		if (GET_WM_COMMAND_ID (wParam, lParam) == IDCANCEL) {
			EndDialog (hwnd, 0);
		}
	}
	break;
	}
	return FALSE;
}

ParametersDialog::ParametersDialog ()
{

}

void ParametersDialog::Show (HWND hwnd) const
{
	InMemoryDialog::DialogParameters dialogParameters;
	InMemoryDialog dialog (dialogParameters);

	InMemoryDialog::ControlParameters controlParameters;
	dialog.AddControl (controlParameters);

	dialog.Show (hwnd, DlgProc);
}

}
