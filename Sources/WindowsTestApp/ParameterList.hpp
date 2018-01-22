#ifndef PARAMETERLIST_HPP
#define PARAMETERLIST_HPP

#include "wx/wx.h"
#include "wx/dataview.h"

class ParameterAccessor
{
public:
	ParameterAccessor ();
	virtual ~ParameterAccessor ();

	virtual size_t			GetParameterCount () const = 0;
	virtual std::wstring	GetParameterName (int index) const = 0;
	virtual std::wstring	GetParameterValue (int index) const = 0;
	virtual bool			SetParameterValue (int index, const std::wstring& value) = 0;
};

class ParameterList : public wxDataViewListCtrl
{
public:
	ParameterList (wxWindow* parent, ParameterAccessor& paramAccessor);

	void	FillParameters ();
	void	OnEditingDone (wxDataViewEvent& evt);

private:
	ParameterAccessor& paramAccessor;

	DECLARE_EVENT_TABLE ();
};

#endif
