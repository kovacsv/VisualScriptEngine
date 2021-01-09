#include "WAS_ParameterDialog.hpp"

#include "NUIE_NodeParameters.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

#include <string>

namespace WAS
{

static const DWORD FirstControlId = 1000;
static const DWORD StaticControlIdOffset = 2000;
static const DWORD SeparatorId = 3000;
static const DWORD CancelButtonId = IDCANCEL;
static const DWORD OkButtonId = IDOK;

static DWORD ParamIndexToStaticId (size_t paramIndex)
{
	return StaticControlIdOffset + (DWORD) paramIndex;
}

static DWORD ParamIndexToControlId (size_t paramIndex)
{
	return FirstControlId + (DWORD) paramIndex;
}

static size_t ControlIdToParamIndex (DWORD controlId)
{
	return (size_t) controlId - FirstControlId;
}

INT_PTR CALLBACK DlgProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			{
				SetWindowLongPtr (hwnd, GWLP_USERDATA, lParam);
				ParameterDialog* paramDialog = (ParameterDialog*) lParam;
				if (paramDialog != nullptr) {
					paramDialog->Init (hwnd);
				}
			}
			break;
		case WM_CLOSE:
			{
				EndDialog (hwnd, IDCANCEL);
			}
			break;
		case WM_COMMAND:
			{
				ParameterDialog* paramDialog = (ParameterDialog*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
				if (paramDialog == nullptr) {
					break;
				}

				WORD commandId = LOWORD (wParam);
				if (commandId == OkButtonId) {
					if (paramDialog->ApplyParameterChanges ()) {
						EndDialog (hwnd, IDOK);
					}
				} else if (commandId == CancelButtonId) {
					EndDialog (hwnd, IDCANCEL);
				} else {
					WORD notificationCode = HIWORD (wParam);
					switch (notificationCode) {
						case EN_CHANGE:
						case CBN_SELCHANGE:
							{
								if (paramDialog->IsInitialized ()) {
									DWORD controlId = GetWindowLong ((HWND) lParam, GWL_ID);
									size_t paramIndex = ControlIdToParamIndex (controlId);
									paramDialog->SetParameterChanged (paramIndex);
								}
							}
							break;
					}
				}
			}
			break;
	}

	return 0;
}

ParameterDialog::ParameterDialog (const std::wstring& dialogTitle, NUIE::ParameterInterfacePtr& paramInterface, HWND parentHwnd) :
	NUIE::ParameterDialogBase (dialogTitle, paramInterface),
	paramDialog (),
	parentWindowHandle (parentHwnd),
	dialogHandle (NULL)
{

}

void ParameterDialog::Init (HWND hwnd)
{
	dialogHandle = hwnd;
	paramDialog.InitControls (dialogHandle);
	CenterToParent ();
}

bool ParameterDialog::IsInitialized () const
{
	return paramDialog.GetStatus () == InMemoryDialog::Status::Initialized;
}

void ParameterDialog::SetDialogRect (const NUIE::IntRect& rect)
{
	paramDialog.SetParameters (dialogTitle, (short) rect.GetLeft (), (short) rect.GetTop (), (short) rect.GetWidth (), (short) rect.GetHeight ());
}

void ParameterDialog::AddParamNameStatic (size_t paramIndex, const std::wstring& controlText, const NUIE::IntRect& rect)
{
	DWORD controlId = ParamIndexToStaticId (paramIndex);
	paramDialog.AddStatic (controlText, (short) rect.GetLeft (), (short) rect.GetTop (), (short) rect.GetWidth (), (short) rect.GetHeight (), controlId);
}

void ParameterDialog::AddParamEditText (size_t paramIndex, const std::wstring& controlText, const NUIE::IntRect& rect)
{
	DWORD controlId = ParamIndexToControlId (paramIndex);
	paramDialog.AddEdit (controlText, (short) rect.GetLeft (), (short) rect.GetTop (), (short) rect.GetWidth (), (short) rect.GetHeight (), controlId);
}

void ParameterDialog::AddParamComboBox (size_t paramIndex, int selectedChoice, const std::vector<std::wstring>& choices, const NUIE::IntRect& rect)
{
	DWORD controlId = ParamIndexToControlId (paramIndex);
	paramDialog.AddComboBox (selectedChoice, choices, (short) rect.GetLeft (), (short) rect.GetTop (), (short) rect.GetWidth (), (short) rect.GetHeight (), controlId);
}

void ParameterDialog::AddHorizontalSeparator (int x, int y, int width)
{
	paramDialog.AddSeparator ((short) x, (short) y, (short) width + 2, SeparatorId);
}

void ParameterDialog::AddCancelButton (const std::wstring& controlText, const NUIE::IntRect& rect)
{
	paramDialog.AddButton (controlText, (short) rect.GetLeft (), (short) rect.GetTop (), (short) rect.GetWidth (), (short) rect.GetHeight (), CancelButtonId);
}

void ParameterDialog::AddOkButton (const std::wstring& controlText, const NUIE::IntRect& rect)
{
	paramDialog.AddDefButton (controlText, (short) rect.GetLeft (), (short) rect.GetTop (), (short) rect.GetWidth (), (short) rect.GetHeight (), OkButtonId);
}

bool ParameterDialog::ShowDialog ()
{
	INT_PTR dialogResult = paramDialog.Show (parentWindowHandle, DlgProc, (LPARAM) this);
	if (dialogResult == IDOK) {
		return true;
	}

	return false;
}

std::wstring ParameterDialog::GetEditTextValue (size_t paramIndex)
{
	wchar_t itemText[1024];
	DWORD controlId = ParamIndexToControlId (paramIndex);
	GetDlgItemText (dialogHandle, controlId, itemText, 1024);
	return std::wstring (itemText);
}

void ParameterDialog::SetEditTextValue (size_t paramIndex, const std::wstring& text)
{
	DWORD controlId = ParamIndexToControlId (paramIndex);
	SetDlgItemText (dialogHandle, controlId, text.c_str ());
}

int ParameterDialog::GetComboboxSelectedItem (size_t paramIndex)
{
	DWORD controlId = ParamIndexToControlId (paramIndex);
	HWND comboBoxHwnd = GetDlgItem (dialogHandle, controlId);
	LRESULT selected = SendMessage (comboBoxHwnd, CB_GETCURSEL, 0, 0);
	return (int) selected;
}

void ParameterDialog::CenterToParent ()
{
	if (DBGERROR (parentWindowHandle == NULL || dialogHandle == NULL)) {
		return;
	}

	RECT parentRect;
	GetWindowRect (parentWindowHandle, &parentRect);
	LONG parentWidth = parentRect.right - parentRect.left;
	LONG parentHeight = parentRect.bottom - parentRect.top;

	RECT dialogRect;
	GetWindowRect (dialogHandle, &dialogRect);
	LONG dialogWidth = dialogRect.right - dialogRect.left;
	LONG dialogHeight = dialogRect.bottom - dialogRect.top;

	MoveWindow (dialogHandle,
		parentRect.left + (parentWidth - dialogWidth) / 2,
		parentRect.top + (parentHeight - dialogHeight) / 2,
		dialogWidth,
		dialogHeight,
		FALSE
	);
}

}
