#ifndef PARAMETERSDIALOG_HPP
#define PARAMETERSDIALOG_HPP

#include <windows.h>

namespace WAS
{

class ParametersDialog
{
public:
	ParametersDialog ();

	void Show (HWND hwnd) const;
};

}

#endif
