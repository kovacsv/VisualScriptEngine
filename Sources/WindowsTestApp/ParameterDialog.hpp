#ifndef PARAMETERDIALOG_HPP
#define PARAMETERDIALOG_HPP

#include "UINodeParameterAccessor.hpp"

#include "wx/wx.h"
#include "wx/dataview.h"

#include <memory>

class ParameterList : public wxDataViewListCtrl
{
public:
	ParameterList (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor);

	void	OnEditingDone (wxDataViewEvent& evt);

private:
	void	FillParameters ();
	void	ClearList ();

	NUIE::NodeParameterAccessorPtr paramAccessor;

	DECLARE_EVENT_TABLE ();
};

class ParameterDialog : public wxDialog
{
public:
	enum DialogIds
	{
		OkButtonId = 1001
	};

	ParameterDialog (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor);

	void	OnOkButtonClick (wxCommandEvent& evt);

private:
	ParameterList*	parameterList;
	wxButton*		okButton;
	wxBoxSizer*		sizer;

	DECLARE_EVENT_TABLE ();
};

#endif
