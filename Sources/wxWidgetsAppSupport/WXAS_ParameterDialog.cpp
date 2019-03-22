#include "WXAS_ParameterDialog.hpp"
#include "WXAS_ControlUtilities.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Localization.hpp"
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

ParameterDialog::ParameterDialog (wxWindow* parent, NUIE::ParameterInterfacePtr& paramInterface) :
	wxDialog (parent, wxID_ANY, NE::Localize (L"Set Parameters"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	paramInterface (paramInterface),
	gridSizer (new wxGridSizer (2, 5, 5)),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	okButton (new wxButton (this, wxID_OK, NE::Localize (L"OK")))
{
	gridSizer->SetRows (paramInterface->GetParameterCount ());
	for (size_t paramIndex = 0; paramIndex < paramInterface->GetParameterCount (); ++paramIndex) {
		NUIE::ParameterType type = paramInterface->GetParameterType (paramIndex);
		NE::ValueConstPtr value = paramInterface->GetParameterValue (paramIndex);

		int controlId = ParamIdToControlId (paramIndex);
		wxControl* control = nullptr;
		if (type == NUIE::ParameterType::Boolean) {
			if (DBGVERIFY (NE::Value::IsType<NE::BooleanValue> (value))) {
				wxChoice* choiceControl = new wxChoice (this, controlId);
				choiceControl->Append (wxString (NE::Localize (L"true")));
				choiceControl->Append (wxString (NE::Localize (L"false")));
				choiceControl->Select (NE::BooleanValue::Get (value) ? 0 : 1);
				control = choiceControl;
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextControlValidator (textControl, L"0123456789-");
				control = textControl;
			}
		} else if (type == NUIE::ParameterType::Float) {
			if (DBGVERIFY (NE::Value::IsType<NE::FloatValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextControlValidator (textControl, L"0123456789.-");
				control = textControl;
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextControlValidator (textControl, L"0123456789.-");
				control = textControl;
			}
		} else if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				wxTextCtrl* textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
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
	CenterOnParent ();
	SetEscapeId (wxID_CANCEL);
}

void ParameterDialog::OnButtonClick (wxCommandEvent& evt)
{
	if (evt.GetId () == wxID_OK) {
		bool isAllParameterValid = true;
		for (size_t i = 0; i < paramInterface->GetParameterCount (); ++i) {
			ParamUIData& uiData = paramUIDataList[i];
			if (!uiData.isChanged) {
				continue;
			}

			NUIE::ParameterType type = paramInterface->GetParameterType (i);
			NE::ValuePtr value = uiData.GetValue (type);
			if (DBGERROR (value == nullptr)) {
				isAllParameterValid = false;
				continue;
			}

			if (!paramInterface->IsValidParameterValue (i, value)) {
				NE::ValueConstPtr oldValue = paramInterface->GetParameterValue (i);
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
			NE::ValuePtr value = uiData.GetValue (type);
			if (DBGERROR (value == nullptr)) {
				continue;
			}

			paramInterface->SetParameterValue (i, value);
		}

		if (isAllParameterValid) {
			EndModal (wxID_OK);
		}
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

void ParameterDialog::OnKeyDown (wxKeyEvent& evt)
{
	if (evt.GetKeyCode () == WXK_ESCAPE) {
		EndModal (wxID_CANCEL);
	}
	evt.Skip ();
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
		return std::wstring ();
	}
}

int ParameterDialog::ParamUIData::GetChoiceValue () const
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

NE::ValuePtr ParameterDialog::ParamUIData::GetValue (NUIE::ParameterType type) const
{
	NE::ValuePtr value = nullptr;
	if (type == NUIE::ParameterType::Boolean) {
		int choiceValue = GetChoiceValue ();
		value = NE::ValuePtr (new NE::BooleanValue (choiceValue == 0 ? true : false));
	} else if (type == NUIE::ParameterType::Integer || type == NUIE::ParameterType::Float || type == NUIE::ParameterType::Double || type == NUIE::ParameterType::String) {
		value = NUIE::StringToParameterValue (GetStringValue (), type);
	} else if (type == NUIE::ParameterType::Enumeration) {
		value = NE::ValuePtr (new NE::IntValue (GetChoiceValue ()));
	}
	return value;
}

BEGIN_EVENT_TABLE (ParameterDialog, wxDialog)
EVT_BUTTON (wxID_ANY, ParameterDialog::OnButtonClick)
EVT_TEXT (wxID_ANY, ParameterDialog::OnTextChanged)
EVT_CHOICE (wxID_ANY, ParameterDialog::OnChoiceChanged)
EVT_CHAR_HOOK (ParameterDialog::OnKeyDown)
END_EVENT_TABLE ()

}
