#ifndef PARAMETERDIALOG_HPP
#define PARAMETERDIALOG_HPP

#include "UINodeParameterAccessor.hpp"

#include <wx/wx.h>
#include <memory>

class ParameterDialog : public wxDialog
{
public:
	enum DialogIds
	{
		OkButtonId = 1001
	};

	ParameterDialog (wxWindow* parent, NUIE::NodeParameterAccessorPtr& paramAccessor);

	void	OnOkButtonClick (wxCommandEvent& evt);
	void	OnTextChanged (wxCommandEvent& evt);

private:
	struct ParamUIData
	{
		ParamUIData (wxControl* control);

		wxControl*	control;
		bool		isChanged;
	};

	NUIE::NodeParameterAccessorPtr	paramAccessor;
	std::vector<ParamUIData>		paramUIDataList;

	wxGridSizer*					gridSizer;
	wxBoxSizer*						boxSizer;
	wxButton*						okButton;

	DECLARE_EVENT_TABLE ();
};

#endif
