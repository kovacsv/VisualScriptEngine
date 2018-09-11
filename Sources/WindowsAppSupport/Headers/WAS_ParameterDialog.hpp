#ifndef PARAMETERSDIALOG_HPP
#define PARAMETERSDIALOG_HPP

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
		ChangedParameter (size_t index, const std::wstring& value);

		size_t			index;
		std::wstring	value;
	};

	void	ApplyParameterChanges () const;

	WAS::InMemoryDialog*			inMemoryDialog;
	NUIE::ParameterInterfacePtr		paramInterface;
	std::vector<ChangedParameter>	paramValues;
	std::unordered_set<size_t>		changedParams;
};

}

#endif
