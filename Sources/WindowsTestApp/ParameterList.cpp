#include "ParameterList.hpp"
#include "Debug.hpp"

#include "wx/univ/renderer.h"

ParameterAccessor::ParameterAccessor ()
{

}

ParameterAccessor::~ParameterAccessor ()
{

}

ParameterList::ParameterList (wxWindow* parent) :
	wxDataViewListCtrl (parent, wxID_ANY, wxDefaultPosition, wxSize (250, 0)),
	paramAccessor (nullptr)
{
	AppendTextColumn ("Name");
	AppendTextColumn ("Value", wxDATAVIEW_CELL_EDITABLE);
}

void ParameterList::SetParameterAccessor (const ParameterAccessorPtr& newParamAccessor)
{
	paramAccessor = newParamAccessor;
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
		data.push_back (wxVariant (paramAccessor->GetParameterValue (i)));
		AppendItem (data, i);
	}

	if (selectedRow >= 0 && selectedRow < parameterCount) {
		SelectRow (selectedRow);
	}
}

void ParameterList::OnEditingDone (wxDataViewEvent& evt)
{
	if (DBGERROR (paramAccessor == nullptr)) {
		evt.Veto ();
		return;
	}

	size_t paramIndex = GetItemData (evt.GetItem ());
	wxVariant val = evt.GetValue ();
	if (!paramAccessor->SetParameterValue (paramIndex, val.GetString ().ToStdWstring ())) {
		evt.Veto ();
	}

	FillParameters ();
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
