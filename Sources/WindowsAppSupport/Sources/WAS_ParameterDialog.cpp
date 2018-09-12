#include "WAS_ParameterDialog.hpp"

#include "NUIE_UINodeParameters.hpp"
#include "NE_Debug.hpp"
#include "NE_SingleValues.hpp"

#include <string>

namespace WAS
{

static const DWORD FirstControlId = 10000;
static const DWORD StaticControlIdOffset = 1000;
static const DWORD OkButtonId = 2000;

static DWORD ParamIdToControlId (size_t paramId)
{
	return FirstControlId + (DWORD) paramId;
}

static size_t ControlIdToParamId (DWORD controlId)
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
					paramDialog->SetupControls (hwnd);
				}
			}
			break;
		case WM_CLOSE:
			EndDialog (hwnd, 0);
			break;
		case WM_COMMAND:
			{
				ParameterDialog* paramDialog = (ParameterDialog*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
				if (paramDialog == nullptr) {
					break;
				}

				WORD commandId = LOWORD (wParam);
				if (commandId == OkButtonId) {
					if (paramDialog->CollectChangedValues (hwnd)) {
						EndDialog (hwnd, 1);
					}
				} else {
					WORD notificationCode = HIWORD (wParam);
					switch (notificationCode) {
						case EN_CHANGE:
						case CBN_SELCHANGE:
							{
								DWORD controlId = GetWindowLong ((HWND) lParam, GWL_ID);
								paramDialog->SetParameterChanged (controlId);
							}
							break;
					}
				}
			}
			break;
	}

	return 0;
}

ParameterDialog::ChangedParameter::ChangedParameter (size_t index, const std::wstring& value) :
	index (index),
	value (value)
{

}

ParameterDialog::ParameterDialog (NUIE::ParameterInterfacePtr& paramInterface) :
	paramInterface (paramInterface),
	inMemoryDialog (nullptr),
	isSetUp (false)
{

}

bool ParameterDialog::Show (HWND parent, WORD x, WORD y)
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
	for (size_t paramId = 0; paramId < paramCount; ++paramId) {
		NUIE::ParameterType type = paramInterface->GetParameterType (paramId);
		NE::ValuePtr value = paramInterface->GetParameterValue (paramId);

		DWORD controlId = ParamIdToControlId (paramId);
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

		dialog.AddStatic (paramInterface->GetParameterName (paramId).c_str (), padding, currentY, staticWidth, controlHeight, controlId + StaticControlIdOffset);
		dialog.AddEdit (controlText.c_str (), staticWidth + 2 * padding, currentY, editWidth, controlHeight, controlId);
		currentY += controlHeight + padding;
	}

	dialog.AddDefButton (L"Save", padding, currentY, staticWidth + editWidth + padding, buttonHeight, OkButtonId);

	inMemoryDialog = &dialog;
	if (dialog.Show (parent, DlgProc, (LPARAM) this) == 1) {
		ApplyParameterChanges ();
		return true;
	}
	inMemoryDialog = nullptr;

	return false;
}

void ParameterDialog::SetupControls (HWND dialogHwnd)
{
	if (DBGVERIFY (inMemoryDialog != nullptr)) {
		inMemoryDialog->SetupControls (dialogHwnd);
		isSetUp = true;
	}
}

void ParameterDialog::SetParameterChanged (DWORD controlId)
{
	if (!isSetUp) {
		return;
	}
	changedParams.insert (ControlIdToParamId (controlId));
}

bool ParameterDialog::CollectChangedValues (HWND hwnd)
{
	bool isAllParameterValid = true;
	for (size_t paramId = 0; paramId < paramInterface->GetParameterCount (); ++paramId) {
		if (changedParams.find (paramId) == changedParams.end ()) {
			continue;
		}

		wchar_t itemText[1024];
		DWORD controlId = ParamIdToControlId (paramId);
		GetDlgItemText (hwnd, controlId, itemText, 1024);
		paramValues.push_back (ChangedParameter (paramId, std::wstring (itemText)));

		NUIE::ParameterType type = paramInterface->GetParameterType (paramId);
		if (!paramInterface->IsValidParameterValue (paramId, NUIE::StringToParameterValue (itemText, type))) {
			NE::ValuePtr oldValue = paramInterface->GetParameterValue (paramId);
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

void ParameterDialog::ApplyParameterChanges () const
{
	for (const ChangedParameter& param : paramValues) {
		NUIE::ParameterType type = paramInterface->GetParameterType (param.index);
		paramInterface->SetParameterValue (param.index, NUIE::StringToParameterValue (param.value, type));
	}
}

}
