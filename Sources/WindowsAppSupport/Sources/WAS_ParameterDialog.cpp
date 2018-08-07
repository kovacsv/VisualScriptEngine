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
					if (paramDialog->FillParameterValues (hwnd)) {
						EndDialog (hwnd, 1);
					}
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

bool ParameterDialog::Show (HWND parent, WORD x, WORD y) const
{
	static const int padding = 5;
	static const int staticWidth = 60;
	static const int editWidth = 100;
	static const int controlHeight = 12;
	static const int buttonHeight = 16;

	int paramCount = (int) paramInterface->GetParameterCount ();
	int dialogWidth = staticWidth + editWidth + 3 * padding;
	int dialogHeight = paramCount * controlHeight + (paramCount + 2) * padding + buttonHeight;
	InMemoryDialog dialog (L"Parameters", x, y, dialogWidth, dialogHeight);

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

bool ParameterDialog::FillParameterValues (HWND hwnd)
{
	// TODO: modifies every selected fields, not just the changed ones

	bool isAllParameterValid = true;
	for (size_t i = 0; i < paramInterface->GetParameterCount (); ++i) {
		DWORD controlId = 1000 + (DWORD) i;
		wchar_t itemText[1024];
		GetDlgItemText (hwnd, controlId, itemText, 1024);
		paramValues.push_back (std::wstring (itemText));

		NUIE::ParameterType type = paramInterface->GetParameterType (i);
		if (!paramInterface->IsValidParameterValue (i, NUIE::StringToParameterValue (itemText, type))) {
			NE::ValuePtr oldValue = paramInterface->GetParameterValue (i);
			std::wstring oldValueString = NUIE::ParameterValueToString (oldValue, type);
			SetDlgItemText (hwnd, controlId, oldValueString.c_str ());
			isAllParameterValid = false;
		}
	}

	if (!isAllParameterValid) {
		paramValues.clear ();
	}
	return isAllParameterValid;
}

}
