#ifndef WAS_PARAMETERSDIALOG_HPP
#define WAS_PARAMETERSDIALOG_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_InMemoryDialog.hpp"
#include "NUIE_ParameterInterface.hpp"
#include "NUIE_ParameterDialog.hpp"

#include <unordered_set>

namespace WAS
{

class ParameterDialog : public NUIE::ParameterDialogBase
{
public:
	ParameterDialog (const std::wstring& dialogTitle, NUIE::ParameterInterfacePtr& paramInterface, HWND parentHwnd);

	void					Init (HWND hwnd);
	bool					IsInitialized () const;

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

	void					CenterToParent ();

	InMemoryDialog			paramDialog;
	HWND					parentWindowHandle;
	HWND					dialogHandle;
};

}

#endif
