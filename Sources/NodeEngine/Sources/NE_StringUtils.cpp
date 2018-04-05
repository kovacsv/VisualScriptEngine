#include "NE_StringUtils.hpp"

#include <sstream>
#include <iomanip>

namespace NE
{

std::wstring DoubleToString (double val, int precision)
{
    std::wostringstream stream;
    stream << std::fixed << std::setprecision (precision) << val;
	return stream.str ();
}

}
