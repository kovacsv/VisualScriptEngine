#ifndef WINDRAWINGCONTEXT_HPP
#define WINDRAWINGCONTEXT_HPP

#include "DrawingContext.hpp"
#include "DrawingCacheKeys.hpp"

#include <windows.h>

class WinDrawingContext : public NUIE::DrawingContext
{
public:
	WinDrawingContext ();
	WinDrawingContext (const WinDrawingContext& rhs) = delete;
	virtual ~WinDrawingContext ();

	virtual void	Init (HWND hwnd) = 0;
	virtual void	DrawToHDC (HDC hdc) = 0;
};

#endif
