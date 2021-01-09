#ifndef NUIE_PARAMETERDIALOG_HPP
#define NUIE_PARAMETERDIALOG_HPP

#include "NUIE_ParameterInterface.hpp"

namespace NUIE
{

class ParameterDialogBase
{
public:
	ParameterDialogBase (const std::wstring& dialogTitle, const ParameterInterfacePtr& paramInterface);
	virtual ~ParameterDialogBase ();

	bool	Show (int x, int y);

	void	SetParameterChanged (size_t paramIndex);
	bool	ApplyParameterChanges ();

protected:
	virtual void			SetDialogRect (const IntRect& rect) = 0;
	virtual void			AddParamNameStatic (size_t paramIndex, const std::wstring& controlText, const IntRect& rect) = 0;
	virtual void			AddParamEditText (size_t paramIndex, const std::wstring& controlText, const IntRect& rect) = 0;
	virtual void			AddParamComboBox (size_t paramIndex, int selectedChoice, const std::vector<std::wstring>& choices, const IntRect& rect) = 0;
	virtual void			AddHorizontalSeparator (int x, int y, int width) = 0;
	virtual void			AddCancelButton (const std::wstring& controlText, const IntRect& rect) = 0;
	virtual void			AddOkButton (const std::wstring& controlText, const IntRect& rect) = 0;
	virtual bool			ShowDialog () = 0;

	virtual std::wstring	GetEditTextValue (size_t paramIndex) = 0;
	virtual void			SetEditTextValue (size_t paramIndex, const std::wstring& text) = 0;
	virtual int				GetComboboxSelectedItem (size_t paramIndex) = 0;

	std::wstring				dialogTitle;
	ParameterInterfacePtr		paramInterface;
	std::unordered_set<size_t>	changedParamIndices;
};

}

#endif
