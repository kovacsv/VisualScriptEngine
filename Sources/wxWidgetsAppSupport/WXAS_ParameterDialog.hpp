#ifndef WXAS_PARAMETERDIALOG_HPP
#define WXAS_PARAMETERDIALOG_HPP

#include "NUIE_ParameterInterface.hpp"

#include <wx/wx.h>
#include <memory>

namespace WXAS
{

class ParameterDialog : public wxDialog
{
public:
	ParameterDialog (wxWindow* parent, NUIE::ParameterInterfacePtr& paramInterface);

	void	OnButtonClick (wxCommandEvent& evt);
	void	OnTextChanged (wxCommandEvent& evt);
	void	OnChoiceChanged (wxCommandEvent& evt);
	void	OnKeyDown (wxKeyEvent& evt);

private:
	struct ParamUIData
	{
		ParamUIData (wxControl* control);

		std::wstring	GetStringValue () const;
		int				GetChoiceValue () const;
		void			SetStringValue (const std::wstring& value);
		NE::ValuePtr	GetValue (NUIE::ParameterType type) const;

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

}

#endif
