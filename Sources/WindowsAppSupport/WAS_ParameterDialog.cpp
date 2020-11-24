#include "WAS_ParameterDialog.hpp"

#include "NUIE_NodeParameters.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

#include <string>

namespace WAS
{

static const short DialogPadding = 5;
static const short StaticWidth = 60;
static const short ControlWidth = 150;
static const short ControlHeight = 12;
static const short ButtonWidth = 50;
static const short ButtonHeight = 15;

static const DWORD FirstControlId = 10000;
static const DWORD StaticControlIdOffset = 1000;
static const DWORD SeparatorId = 2000;
static const DWORD CancelButtonId = IDCANCEL;
static const DWORD OkButtonId = IDOK;

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
					paramDialog->CenterToParent (hwnd);
					paramDialog->SetupControls (hwnd);
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
					if (paramDialog->CollectChangedValues (hwnd)) {
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

ParameterDialog::ChangedParameter::ChangedParameter (size_t index, const NE::ValuePtr& value) :
	index (index),
	value (value)
{

}

size_t ParameterDialog::ChangedParameter::GetIndex () const
{
	return index;
}

const NE::ValuePtr& ParameterDialog::ChangedParameter::GetValue () const
{
	return value;
}

ParameterDialog::ParameterDialog (NUIE::ParameterInterfacePtr& paramInterface) :
	paramInterface (paramInterface),
	paramDialog (),
	parentWindowHandle (NULL)
{

}

bool ParameterDialog::Show (const std::wstring& dialogTitle, HWND parentHwnd, short x, short y)
{
	WORD paramCount = (WORD) paramInterface->GetParameterCount ();
	short dialogInnerWidth = StaticWidth + ControlWidth + DialogPadding;
	short dialogWidth = dialogInnerWidth + 2 * DialogPadding;
	short dialogHeight = paramCount * ControlHeight + (paramCount + 3) * DialogPadding + ButtonHeight;
	paramDialog.SetParameters (dialogTitle, x, y, dialogWidth, dialogHeight);

	short currentY = DialogPadding;
	for (WORD paramIndex = 0; paramIndex < paramCount; ++paramIndex) {
		DWORD controlId = ParamIdToControlId (paramIndex);

		NUIE::ParameterType type = paramInterface->GetParameterType (paramIndex);
		paramDialog.AddStatic (paramInterface->GetParameterName (paramIndex).c_str (), DialogPadding, currentY, StaticWidth, ControlHeight, controlId + StaticControlIdOffset);

		NE::ValueConstPtr value = paramInterface->GetParameterValue (paramIndex);
		if (type == NUIE::ParameterType::Boolean) {
			if (DBGVERIFY (NE::Value::IsType<NE::BooleanValue> (value))) {
				int selectedChoice = NE::BooleanValue::Get (value) ? 0 : 1;
				std::vector<std::wstring> choices = { NE::LocalizeString (L"true"), NE::LocalizeString (L"false") };
				paramDialog.AddComboBox (selectedChoice, choices, StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight, controlId);
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				paramDialog.AddEdit (controlText.c_str (), StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight, controlId);
			}
		} else if (type == NUIE::ParameterType::Float) {
			if (DBGVERIFY (NE::Value::IsType<NE::FloatValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				paramDialog.AddEdit (controlText.c_str (), StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight, controlId);
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				paramDialog.AddEdit (controlText.c_str (), StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight, controlId);
			}
		} else if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				std::wstring controlText = NUIE::ParameterValueToString (value, type);
				paramDialog.AddEdit (controlText.c_str (), StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight, controlId);
			}
		} else if (type == NUIE::ParameterType::Enumeration) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				int selectedChoice = NE::IntValue::Get (value);
				std::vector<std::wstring> choices = paramInterface->GetParameterValueChoices (paramIndex);
				paramDialog.AddComboBox (selectedChoice, choices, StaticWidth + 2 * DialogPadding, currentY, ControlWidth, ControlHeight, controlId);
			}
		} else {
			DBGBREAK ();
		}

		currentY += ControlHeight + DialogPadding;
	}

	paramDialog.AddSeparator (DialogPadding, currentY, dialogInnerWidth + 2, SeparatorId);
	paramDialog.AddButton (NE::LocalizeString (L"Cancel"), dialogInnerWidth - 2 * ButtonWidth, currentY + DialogPadding, ButtonWidth, ButtonHeight, CancelButtonId);
	paramDialog.AddDefButton (NE::LocalizeString (L"OK"), dialogInnerWidth - ButtonWidth + DialogPadding, currentY + DialogPadding, ButtonWidth, ButtonHeight, OkButtonId);

	parentWindowHandle = parentHwnd;
	INT_PTR dialogResult = paramDialog.Show (parentHwnd, DlgProc, (LPARAM) this);
	if (dialogResult == IDOK) {
		ApplyParameterChanges ();
		return true;
	}

	return false;
}

void ParameterDialog::CenterToParent (HWND dialogHwnd)
{
	if (DBGERROR (parentWindowHandle == NULL)) {
		return;
	}

	RECT parentRect;
	GetWindowRect (parentWindowHandle, &parentRect);
	LONG parentWidth = parentRect.right - parentRect.left;
	LONG parentHeight = parentRect.bottom - parentRect.top;
	
	RECT dialogRect;
	GetWindowRect (dialogHwnd, &dialogRect);
	LONG dialogWidth = dialogRect.right - dialogRect.left;
	LONG dialogHeight = dialogRect.bottom - dialogRect.top;

	MoveWindow (dialogHwnd,
		parentRect.left + (parentWidth - dialogWidth) / 2,
		parentRect.top + (parentHeight - dialogHeight) / 2,
		dialogWidth,
		dialogHeight,
		FALSE
	);
}

void ParameterDialog::SetupControls (HWND dialogHwnd)
{
	if (DBGERROR (paramDialog.GetStatus () != InMemoryDialog::Status::Opened)) {
		return;
	}
	paramDialog.InitControls (dialogHwnd);
}

void ParameterDialog::SetParameterChanged (DWORD controlId)
{
	if (paramDialog.GetStatus () != InMemoryDialog::Status::Initialized) {
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

		DWORD controlId = ParamIdToControlId (paramId);

		NUIE::ParameterType type = paramInterface->GetParameterType (paramId);

		NE::ValuePtr value;
		if (type == NUIE::ParameterType::Boolean) {
			HWND comboBoxHwnd = GetDlgItem (hwnd, controlId);
			LRESULT selected = SendMessage (comboBoxHwnd, CB_GETCURSEL, 0, 0);
			value = NE::ValuePtr (new NE::BooleanValue (selected == 0 ? true : false));
		} else if (type == NUIE::ParameterType::Integer || type == NUIE::ParameterType::Float || type == NUIE::ParameterType::Double || type == NUIE::ParameterType::String) {
			wchar_t itemText[1024];
			GetDlgItemText (hwnd, controlId, itemText, 1024);
			value = NUIE::StringToParameterValue (std::wstring (itemText), type);
		} else if (type == NUIE::ParameterType::Enumeration) {
			HWND comboBoxHwnd = GetDlgItem (hwnd, controlId);
			LRESULT selected = SendMessage (comboBoxHwnd, CB_GETCURSEL, 0, 0);
			value = NE::ValuePtr (new NE::IntValue ((int) selected));
		} else {
			DBGBREAK ();
			continue;
		}

		if (paramInterface->IsValidParameterValue (paramId, value)) {
			ChangedParameter param (paramId, value);
			paramValues.push_back (param);
		} else {
			NE::ValueConstPtr oldValue = paramInterface->GetParameterValue (paramId);
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
		paramInterface->SetParameterValue (param.GetIndex (), param.GetValue ());
	}
}

}
