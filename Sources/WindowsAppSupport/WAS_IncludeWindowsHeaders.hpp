#ifndef WAS_INCLUDEWINDOWSHEADER_HPP
#define WAS_INCLUDEWINDOWSHEADER_HPP

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4244)
#pragma warning (disable : 4458)
#endif

#include <minmax.h> // for gdiplus
#include <gdiplus.h>

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif
