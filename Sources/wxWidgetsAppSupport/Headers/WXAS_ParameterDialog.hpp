#ifndef PARAMETERDIALOG_HPP
#define PARAMETERDIALOG_HPP

#include "NUIE_ParameterInterface.hpp"

#include <wx/wx.h>
#include <memory>

class ParameterDialog : public wxDialog
{
public:
	enum DialogIds
	{
		OkButtonId = 1001
	};

	ParameterDialog (wxWindow* parent, NUIE::ParameterInterfacePtr& paramInterface);

	void	OnOkButtonClick (wxCommandEvent& evt);
	void	OnTextChanged (wxCommandEvent& evt);
	void	OnChoiceChanged (wxCommandEvent& evt);

private:
	struct ParamUIData
	{
		ParamUIData (wxControl* control);

		std::wstring	GetStringValue () const;
		int				GetIntegerValue () const;
		void			SetStringValue (const std::wstring& value);

		wxControl*		control;
		bool			isChanged;
	};

	NUIE::ParameterInterfacePtr		paramInterface;
	std::vector<ParamUIData>		paramUIDataList;

	wxGridSizer*					gridSizer;
	wxBoxSizer*						boxSizer;
	wxButton*						okButton;

	DECLARE_EVENT_TABLE ();
};

#endif
