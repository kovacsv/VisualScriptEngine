#ifndef WAS_DIRECT2DUTILS_HPP
#define WAS_DIRECT2DUTILS_HPP

#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>

#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"
#include "NUIE_IconId.hpp"
#include "WAS_Direct2DImageLoader.hpp"

namespace WAS
{

class Direct2DHandler
{
public:
	Direct2DHandler ();
	~Direct2DHandler ();

	ID2D1Factory*		direct2DFactory;
	IDWriteFactory*		directWriteFactory;
};

class Direct2DAntialiasGuard
{
public:
	Direct2DAntialiasGuard (ID2D1RenderTarget* renderTarget, D2D1_ANTIALIAS_MODE newMode);
	~Direct2DAntialiasGuard ();

private:
	ID2D1RenderTarget*		renderTarget;
	D2D1_ANTIALIAS_MODE		oldMode;
};

void		D2D1DrawLine (ID2D1RenderTarget* renderTarget, const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen);
void		D2D1DrawBezier (ID2D1Factory* direct2DFactory, ID2D1RenderTarget* renderTarget, const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen);
void		D2D1DrawRect (ID2D1RenderTarget* renderTarget, const NUIE::Rect& rect, const NUIE::Pen& pen);
void		D2D1FillRect (ID2D1RenderTarget* renderTarget, const NUIE::Rect& rect, const NUIE::Color& color);
void		D2D1DrawEllipse (ID2D1RenderTarget* renderTarget, const NUIE::Rect& rect, const NUIE::Pen& pen);
void		D2D1FillEllipse (ID2D1RenderTarget* renderTarget, const NUIE::Rect& rect, const NUIE::Color& color);
void		D2D1DrawFormattedText (IDWriteFactory* directWriteFactory, ID2D1RenderTarget* renderTarget, const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor);
NUIE::Size	D2D1MeasureText (IDWriteFactory* directWriteFactory, ID2D1RenderTarget* renderTarget, const NUIE::Font& font, const std::wstring& text);
void		D2D1DrawIcon (ID2D1RenderTarget* renderTarget, Direct2DImageLoader* imageLoader, const NUIE::Rect& rect, const NUIE::IconId& iconId);

}

#endif
