#ifndef WAS_PARAMETERSDIALOG_HPP
#define WAS_PARAMETERSDIALOG_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_InMemoryDialog.hpp"
#include "NUIE_ParameterInterface.hpp"

#include <unordered_set>

namespace WAS
{

class ParameterDialog
{
public:
	ParameterDialog (NUIE::ParameterInterfacePtr& paramInterface, HWND parentHwnd);

	bool	Show (const std::wstring& dialogTitle, short x, short y);
	void	Init ();

	void	SetParameterChanged (size_t paramIndex);
	bool	ApplyParameterChanges ();

	void	SetDialogHandle (HWND hwnd);

private:
	class ChangedParameter
	{
	public:
		ChangedParameter (size_t index, const NE::ValuePtr& value);

		size_t					GetIndex () const;
		const NE::ValuePtr&		GetValue () const;

	private:
		size_t			index;
		NE::ValuePtr	value;
	};

	bool							ShowDialog ();
	void							CenterToParent ();
	bool							CollectChangedValues (std::vector<ChangedParameter>& changedParamValues) const;

	NUIE::ParameterInterfacePtr		paramInterface;
	std::unordered_set<size_t>		changedParams;

	InMemoryDialog					paramDialog;
	HWND							parentWindowHandle;
	HWND							dialogHandle;
};

}

#endif
