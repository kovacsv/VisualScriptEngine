#include "ParameterDialog.hpp"
#include "UINodeParameters.hpp"
#include "SingleValues.hpp"
#include "Debug.hpp"

#include <wx/spinctrl.h>

ParameterDialog::ParameterDialog (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor) :
	wxDialog (parent, wxID_ANY, L"Set Parameters", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE),
	paramAccessor (paramAccessor),
	gridSizer (new wxGridSizer (2, 5, 5)),
	boxSizer (new wxBoxSizer (wxVERTICAL)),
	okButton (new wxButton (this, DialogIds::OkButtonId, L"OK"))
{
	gridSizer->SetRows (paramAccessor->GetParameterCount ());
	for (size_t i = 0; i < paramAccessor->GetParameterCount (); ++i) {
		wxStaticText* paramNameText = new wxStaticText (this, wxID_ANY, paramAccessor->GetParameterName (i));
		gridSizer->Add (paramNameText, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL);

		NUIE::ParameterType type = paramAccessor->GetParameterType (i);
		NE::ValuePtr value = paramAccessor->GetParameterValue (i);

		int controlId = 1000 + i;
		wxControl* editorControl = nullptr;
		if (type == NUIE::ParameterType::String) {
			if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
				editorControl = new wxTextCtrl (this, controlId, NE::StringValue::Get (value));
			}
		} else if (type == NUIE::ParameterType::Integer) {
			if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
				wxSpinCtrl* spinCtrl = new wxSpinCtrl (this, controlId);
				spinCtrl->SetRange (std::numeric_limits<int>::min (), std::numeric_limits<int>::max ());
				spinCtrl->SetValue (NE::IntValue::Get (value));
				editorControl = spinCtrl;
			}
		} else if (type == NUIE::ParameterType::Double) {
			if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
				wxSpinCtrlDouble* spinCtrl = new wxSpinCtrlDouble (this, controlId);
				spinCtrl->SetRange (-std::numeric_limits<double>::max (), std::numeric_limits<double>::max ());
				spinCtrl->SetValue (NE::DoubleValue::Get (value));
				editorControl = spinCtrl;
			}
		}
		
		if (DBGERROR (editorControl == nullptr)) {
			editorControl = new wxStaticText (this, wxID_ANY, "N/A");
		}

		gridSizer->Add (editorControl, 1, wxEXPAND);
		paramUIDataList.push_back (ParamUIData (editorControl));
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
		if (type == NUIE::ParameterType::String) {
			wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*> (uiData.control);
			paramAccessor->SetParameterValue (i, NE::ValuePtr (new NE::StringValue (textCtrl->GetValue ().ToStdWstring ())));
		} else if (type == NUIE::ParameterType::Integer) {
			wxSpinCtrl* spinCtrl = dynamic_cast<wxSpinCtrl*> (uiData.control);
			paramAccessor->SetParameterValue (i, NE::ValuePtr (new NE::IntValue (spinCtrl->GetValue ())));
		} else if (type == NUIE::ParameterType::Double) {
			wxSpinCtrlDouble* spinCtrl = dynamic_cast<wxSpinCtrlDouble*> (uiData.control);
			paramAccessor->SetParameterValue (i, NE::ValuePtr (new NE::DoubleValue (spinCtrl->GetValue ())));
		} else {
			DBGBREAK ();
		}
	}

	EndModal (DialogIds::OkButtonId);
}

void ParameterDialog::OnTextChanged (wxCommandEvent& evt)
{
	int paramIndex = evt.GetId () - 1000;
	paramUIDataList[paramIndex].isChanged = true;
}

ParameterDialog::ParamUIData::ParamUIData (wxControl* control) :
	control (control),
	isChanged (false)
{

}

BEGIN_EVENT_TABLE (ParameterDialog, wxDialog)
EVT_BUTTON (DialogIds::OkButtonId, ParameterDialog::OnOkButtonClick)
EVT_TEXT (wxID_ANY, ParameterDialog::OnTextChanged)
END_EVENT_TABLE ()
