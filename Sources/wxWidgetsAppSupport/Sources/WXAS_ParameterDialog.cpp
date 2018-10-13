#include "WXAS_ParameterDialog.hpp"
#include "NUIE_UINodeParameters.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"

namespace WXAS
{

static const wxWindowID FirstControlId = 10000;

static wxWindowID ParamIdToControlId (size_t paramId)
{
	return FirstControlId + (wxWindowID) paramId;
}

static size_t ControlIdToParamId (wxWindowID controlId)
{
	return (size_t) controlId - FirstControlId;
}

static void SetTextValidator (wxTextCtrl* textCtrl, const std::wstring& validChars)
{
	wxTextValidator validator (wxFILTER_INCLUDE_CHAR_LIST);
	wxArrayString includeList;

	for (const wchar_t& character : validChars) {
		includeList.Add (character);
	}

	validator.SetIncludes (includeList);
	textCtrl->SetValidator (validator);
}

ParameterDialog::ParameterDialog (wxWindow* parent, NUIE::ParameterInterfacePtr& paramInterface) :
	wxDialog (parent, wxID_ANY, L"Set Parameters", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	paramInterface (paramInterface),
	gridSizer (new wxGridSizer (2, 5, 5)),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	okButton (new wxButton (this, DialogIds::OkButtonId, L"OK"))
{
	gridSizer->SetRows (paramInterface->GetParameterCount ());
	for (size_t paramIndex = 0; paramIndex < paramInterface->GetParameterCount (); ++paramIndex) {
		NUIE::ParameterType type = paramInterface->GetParameterType (paramIndex);
		NE::ValuePtr value = paramInterface->GetParameterValue (paramIndex);

		int controlId = ParamIdToControlId (paramIndex);
		wxControl* control = nullptr;
		if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				control = textControl;
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextValidator (textControl, L"0123456789-");
				control = textControl;
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextValidator (textControl, L"0123456789.-");
				control = textControl;
			}
		} else if (type == NUIE::ParameterType::Enumeration) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				wxChoice* choiceControl = new wxChoice (this, controlId);
				std::vector<std::wstring> choices = paramInterface->GetParameterValueChoices (paramIndex);
				for (const std::wstring& choice : choices) {
					choiceControl->Append (wxString (choice));
				}
				choiceControl->Select (NE::IntValue::Get (value));
				control = choiceControl;
			}
		}
		
		if (DBGERROR (control == nullptr)) {
			continue;
		}

		wxStaticText* paramNameText = new wxStaticText (this, wxID_ANY, paramInterface->GetParameterName (paramIndex));
		gridSizer->Add (paramNameText, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

		gridSizer->Add (control, 1, wxEXPAND);
		paramUIDataList.push_back (ParamUIData (control));
	}

	boxSizer->Add (gridSizer, 1, wxEXPAND | wxALL, 5);
	boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 5);

	SetSizerAndFit (boxSizer);
}

void ParameterDialog::OnOkButtonClick (wxCommandEvent& evt)
{
	bool isAllParameterValid = true;
	for (size_t i = 0; i < paramInterface->GetParameterCount (); ++i) {
		ParamUIData& uiData = paramUIDataList[i];
		if (!uiData.isChanged) {
			continue;
		}

		NUIE::ParameterType type = paramInterface->GetParameterType (i);
		NE::ValuePtr value = nullptr;
		if (type == NUIE::ParameterType::Integer || type == NUIE::ParameterType::Double || type == NUIE::ParameterType::String) {
			value = NUIE::StringToParameterValue (uiData.GetStringValue (), type);
		} else if (type == NUIE::ParameterType::Enumeration) {
			value = NE::ValuePtr (new NE::IntValue (uiData.GetIntegerValue ()));
		}
		
		if (DBGERROR (value == nullptr)) {
			isAllParameterValid = false;
			continue;
		}

		if (!paramInterface->IsValidParameterValue (i, value)) {
			NE::ValuePtr oldValue = paramInterface->GetParameterValue (i);
			std::wstring oldValueString = NUIE::ParameterValueToString (oldValue, type);
			uiData.SetStringValue (oldValueString);
			uiData.isChanged = false;
			isAllParameterValid = false;
		}
	}

	if (!isAllParameterValid) {
		return;
	}

	for (size_t i = 0; i < paramInterface->GetParameterCount (); ++i) {
		const ParamUIData& uiData = paramUIDataList[i];
		if (!uiData.isChanged) {
			continue;
		}

		NUIE::ParameterType type = paramInterface->GetParameterType (i);
		NE::ValuePtr value = nullptr;
		if (type == NUIE::ParameterType::Integer || type == NUIE::ParameterType::Double || type == NUIE::ParameterType::String) {
			value = NUIE::StringToParameterValue (uiData.GetStringValue (), type);
		} else if (type == NUIE::ParameterType::Enumeration) {
			value = NE::ValuePtr (new NE::IntValue (uiData.GetIntegerValue ()));
		}
		
		if (DBGERROR (value == nullptr)) {
			continue;
		}

		paramInterface->SetParameterValue (i, value);
	}

	if (isAllParameterValid) {
		EndModal (DialogIds::OkButtonId);
	}
}

void ParameterDialog::OnTextChanged (wxCommandEvent& evt)
{
	int paramIndex = ControlIdToParamId (evt.GetId ());
	paramUIDataList[paramIndex].isChanged = true;
}

void ParameterDialog::OnChoiceChanged (wxCommandEvent& evt)
{
	int paramIndex = ControlIdToParamId (evt.GetId ());
	paramUIDataList[paramIndex].isChanged = true;
}

ParameterDialog::ParamUIData::ParamUIData (wxControl* control) :
	control (control),
	isChanged (false)
{

}

std::wstring ParameterDialog::ParamUIData::GetStringValue () const
{
	if (dynamic_cast<wxTextCtrl*> (control) != nullptr) {
		return dynamic_cast<wxTextCtrl*> (control)->GetValue ().ToStdWstring ();
	} else {
		DBGBREAK ();
		return L"";
	}
}

int ParameterDialog::ParamUIData::GetIntegerValue () const
{
	if (dynamic_cast<wxChoice*> (control) != nullptr) {
		return dynamic_cast<wxChoice*> (control)->GetSelection ();
	} else {
		DBGBREAK ();
		return -1;
	}
}

void ParameterDialog::ParamUIData::SetStringValue (const std::wstring& value)
{
	if (dynamic_cast<wxTextCtrl*> (control) != nullptr) {
		dynamic_cast<wxTextCtrl*> (control)->SetValue (value);
	} else {
		DBGBREAK ();
	}
}

BEGIN_EVENT_TABLE (ParameterDialog, wxDialog)
EVT_BUTTON (DialogIds::OkButtonId, ParameterDialog::OnOkButtonClick)
EVT_TEXT (wxID_ANY, ParameterDialog::OnTextChanged)
EVT_CHOICE (wxID_ANY, ParameterDialog::OnChoiceChanged)
END_EVENT_TABLE ()

}
