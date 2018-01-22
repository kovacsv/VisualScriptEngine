#include "ParameterList.hpp"

ParameterAccessor::ParameterAccessor ()
{

}

ParameterAccessor::~ParameterAccessor ()
{

}

ParameterList::ParameterList (wxWindow* parent, ParameterAccessor& paramAccessor) :
	wxDataViewListCtrl (parent, wxID_ANY, wxDefaultPosition, wxSize (250, 0)),
	paramAccessor (paramAccessor)
{
	AppendTextColumn ("Name");
	AppendTextColumn ("Value", wxDATAVIEW_CELL_EDITABLE);
}

void ParameterList::FillParameters ()
{
	for (size_t i = 0; i < paramAccessor.GetParameterCount (); ++i) {
		wxVector<wxVariant> data;
		data.push_back (wxVariant (paramAccessor.GetParameterName (i)));
		data.push_back (wxVariant (paramAccessor.GetParameterValue (i)));
		AppendItem (data, i);
	}
}

void ParameterList::OnEditingDone (wxDataViewEvent& evt)
{
	size_t paramIndex = GetItemData (evt.GetItem ());
	wxVariant val = evt.GetValue ();
	if (!paramAccessor.SetParameterValue (paramIndex, val.GetString ().ToStdWstring ())) {
		evt.SetValue (wxVariant (paramAccessor.GetParameterValue (paramIndex)));
		evt.Veto ();
	}
}

BEGIN_EVENT_TABLE (ParameterList, wxDataViewListCtrl)
EVT_DATAVIEW_ITEM_EDITING_DONE (wxID_ANY, ParameterList::OnEditingDone)
END_EVENT_TABLE ()
