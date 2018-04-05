#include "ParameterDialog.hpp"
#include "NUIE_UINodeParameters.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"

#include <wx/spinctrl.h>

void SetTextValidator (wxTextCtrl* textCtrl, const std::wstring& validChars)
{
	wxTextValidator validator (wxFILTER_INCLUDE_CHAR_LIST);
	wxArrayString includeList;

	for (const wchar_t& character : validChars) {
		includeList.Add (character);
	}
	validator.SetIncludes (includeList);
	textCtrl->SetValidator (validator);
}

ParameterDialog::ParameterDialog (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor) :
	wxDialog (parent, wxID_ANY, L"Set Parameters", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	paramAccessor (paramAccessor),
	gridSizer (new wxGridSizer (2, 5, 5)),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	okButton (new wxButton (this, DialogIds::OkButtonId, L"OK"))
{
	gridSizer->SetRows (paramAccessor->GetParameterCount ());
	for (size_t i = 0; i < paramAccessor->GetParameterCount (); ++i) {
		NUIE::ParameterType type = paramAccessor->GetParameterType (i);
		NE::ValuePtr value = paramAccessor->GetParameterValue (i);

		int controlId = 1000 + i;
		wxTextCtrl* textControl = nullptr;
		if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextValidator (textControl, L"0123456789-");
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				textControl = new wxTextCtrl (this, controlId, NUIE::ParameterValueToString (value, type));
				SetTextValidator (textControl, L"0123456789.-");
			}
		}
		
		if (DBGERROR (textControl == nullptr)) {
			continue;
		}

		wxStaticText* paramNameText = new wxStaticText (this, wxID_ANY, paramAccessor->GetParameterName (i));
		gridSizer->Add (paramNameText, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

		gridSizer->Add (textControl, 1, wxEXPAND);
		paramUIDataList.push_back (ParamUIData (textControl));
	}

	boxSizer->Add (gridSizer, 1, wxEXPAND | wxALL, 5);
	boxSizer->Add (okButton, 0, wxEXPAND | wxALL, 5);

	SetSizerAndFit (boxSizer);
}

void ParameterDialog::OnOkButtonClick (wxCommandEvent& evt)
{
	for (size_t i = 0; i < paramAccessor->GetParameterCount (); ++i) {
		const ParamUIData& uiData = paramUIDataList[i];
		if (!uiData.isChanged) {
			continue;
		}

		// TODO: veto event, and write back correct value in case of failure
		NUIE::ParameterType type = paramAccessor->GetParameterType (i);
		paramAccessor->SetParameterValue (i, NUIE::StringToParameterValue (uiData.control->GetValue ().ToStdWstring (), type));
	}

	EndModal (DialogIds::OkButtonId);
}

void ParameterDialog::OnTextChanged (wxCommandEvent& evt)
{
	int paramIndex = evt.GetId () - 1000;
	paramUIDataList[paramIndex].isChanged = true;
}

ParameterDialog::ParamUIData::ParamUIData (wxTextCtrl* control) :
	control (control),
	isChanged (false)
{

}

BEGIN_EVENT_TABLE (ParameterDialog, wxDialog)
EVT_BUTTON (DialogIds::OkButtonId, ParameterDialog::OnOkButtonClick)
EVT_TEXT (wxID_ANY, ParameterDialog::OnTextChanged)
END_EVENT_TABLE ()
