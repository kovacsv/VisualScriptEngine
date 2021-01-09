#ifndef MAS_PARAMETERDIALOG_HPP
#define MAS_PARAMETERDIALOG_HPP

#include "NUIE_ParameterDialog.hpp"

#ifdef __cplusplus
#ifdef __OBJC__
	@class ParameterWindow;
#else
	struct ParameterWindow;
#endif
#endif

namespace MAS
{

class ParameterDialog : public NUIE::ParameterDialogBase
{
public:
	ParameterDialog (const std::wstring& dialogTitle, NUIE::ParameterInterfacePtr& paramInterface);
	
private:
	virtual void			SetDialogRect (const NUIE::IntRect& rect) override;
	virtual void			AddParamNameStatic (size_t paramIndex, const std::wstring& controlText, const NUIE::IntRect& rect) override;
	virtual void			AddParamEditText (size_t paramIndex, const std::wstring& controlText, const NUIE::IntRect& rect) override;
	virtual void			AddParamComboBox (size_t paramIndex, int selectedChoice, const std::vector<std::wstring>& choices, const NUIE::IntRect& rect) override;
	virtual void			AddHorizontalSeparator (int x, int y, int width) override;
	virtual void			AddCancelButton (const std::wstring& controlText, const NUIE::IntRect& rect) override;
	virtual void			AddOkButton (const std::wstring& controlText, const NUIE::IntRect& rect) override;
	virtual bool			ShowDialog () override;
	
	virtual std::wstring	GetEditTextValue (size_t paramIndex) override;
	virtual void			SetEditTextValue (size_t paramIndex, const std::wstring& text) override;
	virtual int				GetComboboxSelectedItem (size_t paramIndex) override;
	
	ParameterWindow*		paramDialog;
};
	
}

#endif
