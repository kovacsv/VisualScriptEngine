#include "WAS_ParameterDialog.hpp"
#include "WAS_InMemoryDialog.hpp"

#include "NUIE_UINodeParameters.hpp"
#include "NE_Debug.hpp"
#include "NE_SingleValues.hpp"

#include <string>

namespace WAS
{

static const WORD OkButtonId = 2000;

INT_PTR CALLBACK DlgProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			SetWindowLongPtr (hwnd, GWLP_USERDATA, lParam);
			break;
		case WM_CLOSE:
			EndDialog (hwnd, 0);
			break;
		case WM_COMMAND:
			{
				WORD commandId = LOWORD (wParam);
				if (commandId == OkButtonId) {
					ParameterDialog* paramDialog = (ParameterDialog*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
					paramDialog->OnOkButtonPressed (hwnd);
					EndDialog (hwnd, 1);
				}
			}
			break;
	}

	return 0;
}

ParameterDialog::ParameterDialog (NUIE::ParameterInterfacePtr& paramInterface) :
	paramInterface (paramInterface)
{

}

bool ParameterDialog::Show (HWND parent) const
{
	static const int padding = 5;
	static const int staticWidth = 60;
	static const int editWidth = 100;
	static const int controlHeight = 12;
	static const int buttonHeight = 16;

	int paramCount = (int) paramInterface->GetParameterCount ();
	int dialogWidth = staticWidth + editWidth + 3 * padding;
	int dialogHeight = paramCount * controlHeight + (paramCount + 2) * padding + buttonHeight;
	InMemoryDialog dialog (L"Parameters", dialogWidth, dialogHeight);

	int currentY = padding;
	for (size_t i = 0; i < paramCount; ++i) {
		NUIE::ParameterType type = paramInterface->GetParameterType (i);
		NE::ValuePtr value = paramInterface->GetParameterValue (i);

		DWORD controlId = 1000 + (DWORD) i;
		std::wstring controlText;
		if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				controlText = NUIE::ParameterValueToString (value, type);
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				controlText = NUIE::ParameterValueToString (value, type);
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				controlText = NUIE::ParameterValueToString (value, type);
			}
		}

		dialog.AddStatic (paramInterface->GetParameterName (i).c_str (), padding, currentY, staticWidth, controlHeight, 0);
		dialog.AddEdit (controlText.c_str (), staticWidth + 2 * padding, currentY, editWidth, controlHeight, controlId);
		currentY += controlHeight + padding;
	}

	dialog.AddDefButton (L"Save", padding, currentY, staticWidth + editWidth + padding, buttonHeight, OkButtonId);
	if (dialog.Show (parent, DlgProc, (LPARAM) this) == 1) {
		for (size_t i = 0; i < paramValues.size (); ++i) {
			NUIE::ParameterType type = paramInterface->GetParameterType (i);
			paramInterface->SetParameterValue (i, NUIE::StringToParameterValue (paramValues[i], type));
		}
		return true;
	}

	return false;
}

void ParameterDialog::OnOkButtonPressed (HWND hwnd)
{
	for (size_t i = 0; i < paramInterface->GetParameterCount (); ++i) {
		DWORD controlId = 1000 + (DWORD) i;
		wchar_t itemText[1024];
		GetDlgItemText (hwnd, controlId, itemText, 1024);
		paramValues.push_back (std::wstring (itemText));
	}
}

}
