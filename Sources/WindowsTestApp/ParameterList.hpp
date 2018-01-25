#ifndef PARAMETERLIST_HPP
#define PARAMETERLIST_HPP

#include "wx/wx.h"
#include "wx/dataview.h"

#include <memory>

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

typedef std::shared_ptr<ParameterAccessor> ParameterAccessorPtr;
typedef std::shared_ptr<const ParameterAccessor> ParameterAccessorConstPtr;

class ParameterList : public wxDataViewListCtrl
{
public:
	ParameterList (wxWindow* parent);

	void	SetParameterAccessor (const ParameterAccessorPtr& newParamAccessor);

	void	FillParameters ();
	void	OnEditingDone (wxDataViewEvent& evt);

private:
	void	ClearList ();

	ParameterAccessorPtr paramAccessor;

	DECLARE_EVENT_TABLE ();
};

#endif
