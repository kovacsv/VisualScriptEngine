#ifndef PARAMETERSDIALOG_HPP
#define PARAMETERSDIALOG_HPP

#include <windows.h>

namespace WAS
{

class ParameterDialog
{
public:
	ParameterDialog ();

	void Show (HWND hwnd) const;
};

}

#endif
