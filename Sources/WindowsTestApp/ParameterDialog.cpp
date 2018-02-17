#include "ParameterDialog.hpp"
#include "UINodeParameters.hpp"
#include "Debug.hpp"

#include <wx/univ/renderer.h>

ParameterList::ParameterList (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor) :
	wxDataViewListCtrl (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
	paramAccessor (paramAccessor)
{
	AppendTextColumn ("Name", wxDATAVIEW_CELL_INERT, 120);
	AppendTextColumn ("Value", wxDATAVIEW_CELL_EDITABLE);
	FillParameters ();
}

void ParameterList::OnEditingDone (wxDataViewEvent& evt)
{
	if (DBGERROR (paramAccessor == nullptr)) {
		evt.Veto ();
		return;
	}

	size_t paramIndex = GetItemData (evt.GetItem ());
	NE::ValuePtr newValue = NUIE::StringToParameterValue (evt.GetValue ().GetString ().ToStdWstring (), paramAccessor->GetParameterType (paramIndex));
	if (newValue == nullptr) {
		evt.Veto ();
		return;
	}

	if (!paramAccessor->SetParameterValue (paramIndex, newValue)) {
		evt.Veto ();
		return;
	}
}

void ParameterList::FillParameters ()
{
	if (DBGERROR (paramAccessor == nullptr)) {
		return;
	}

	int selectedRow = GetSelectedRow ();
	ClearList ();

	size_t parameterCount = paramAccessor->GetParameterCount ();
	for (size_t i = 0; i < parameterCount; ++i) {
		wxVector<wxVariant> data;
		data.push_back (wxVariant (paramAccessor->GetParameterName (i)));
		data.push_back (NUIE::ParameterValueToString (paramAccessor->GetParameterValue (i), paramAccessor->GetParameterType (i)));
		AppendItem (data, i);
	}

	if (selectedRow >= 0 && selectedRow < parameterCount) {
		SelectRow (selectedRow);
	}
}

void ParameterList::ClearList ()
{
	wxDataViewColumn* column = GetColumn (1);
	if (DBGVERIFY (column != nullptr)) {
		wxDataViewRenderer* renderer = column->GetRenderer ();
		if (DBGVERIFY (renderer != nullptr)) {
			renderer->CancelEditing ();
		}
	}
	DeleteAllItems ();
}

BEGIN_EVENT_TABLE (ParameterList, wxDataViewListCtrl)
EVT_DATAVIEW_ITEM_EDITING_DONE (wxID_ANY, ParameterList::OnEditingDone)
END_EVENT_TABLE ()

ParameterDialog::ParameterDialog (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor) :
	wxDialog (parent, wxID_ANY, L"Set Parameters", wxDefaultPosition, wxSize (300, 400), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
	parameterList (new ParameterList (this, paramAccessor)),
	okButton (new wxButton (this, DialogIds::OkButtonId, L"OK")),
	sizer (new wxBoxSizer (wxVERTICAL))
{
	sizer->Add (parameterList, 1, wxEXPAND);
	sizer->Add (okButton, 0, wxEXPAND);
	SetSizer (sizer);
}

void ParameterDialog::OnOkButtonClick (wxCommandEvent& evt)
{
	EndModal (DialogIds::OkButtonId);
}

BEGIN_EVENT_TABLE (ParameterDialog, wxDialog)
EVT_BUTTON (DialogIds::OkButtonId, ParameterDialog::OnOkButtonClick)
END_EVENT_TABLE ()
