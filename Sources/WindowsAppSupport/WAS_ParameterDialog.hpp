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
	ParameterDialog (NUIE::ParameterInterfacePtr& paramInterface);

	bool	Show (HWND parent, WORD x, WORD y);
	void	SetupControls (HWND dialogHwnd);
	void	SetParameterChanged (DWORD controlId);
	bool	CollectChangedValues (HWND hwnd);

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

	void	ApplyParameterChanges () const;

	NUIE::ParameterInterfacePtr		paramInterface;
	std::vector<ChangedParameter>	paramValues;
	std::unordered_set<size_t>		changedParams;

	WAS::InMemoryDialog*			inMemoryDialog;
	bool							isSetUp;
};

}

#endif
