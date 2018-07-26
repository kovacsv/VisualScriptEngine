#ifndef PARAMETERSDIALOG_HPP
#define PARAMETERSDIALOG_HPP

#include "NUIE_ParameterInterface.hpp"

#include <windows.h>

namespace WAS
{

class ParameterDialog
{
public:
	ParameterDialog (NUIE::ParameterInterfacePtr& paramInterface);

	bool	Show (HWND parent) const;
	void	OnOkButtonPressed (HWND hwnd);

private:
	NUIE::ParameterInterfacePtr		paramInterface;
	std::vector<std::wstring>		paramValues;
};

}

#endif
