#ifndef TESTREFERENCE_HPP
#define TESTREFERENCE_HPP

#include "SvgDrawingContext.hpp"

#include <string>

using namespace NE;
using namespace NUIE;

std::wstring	GetTestFilesPath ();
std::wstring	GetCompatibilityTestFilesPath ();
bool			CheckDrawingReference (const SvgDrawingContext& context, const std::wstring& referenceFileName);

#endif
