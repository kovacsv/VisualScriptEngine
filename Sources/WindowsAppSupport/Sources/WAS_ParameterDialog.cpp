#include "WAS_ParameterDialog.hpp"
#include "WAS_InMemoryDialog.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

INT_PTR CALLBACK DlgProc (HWND hwnd, UINT wm, WPARAM wParam, LPARAM lParam)
{
	switch (wm) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		{
			WORD commandId = LOWORD (wParam);
			switch (commandId) {
				case 1984:
					EndDialog (hwnd, 0);
					break;
			}
		}
	break;
	}
	return FALSE;
}

ParameterDialog::ParameterDialog ()
{

}

void ParameterDialog::Show (HWND hwnd) const
{
	InMemoryDialog dialog (L"Parameters", 300, 200);
	dialog.AddButton (L"Close", 10, 10, 80, 20, 1984);

	dialog.Show (hwnd, DlgProc);
}

}
