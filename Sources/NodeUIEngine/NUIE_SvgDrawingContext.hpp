#ifndef NUIE_SVGDRAWINGCONTEXT_HPP
#define NUIE_SVGDRAWINGCONTEXT_HPP

#include "NUIE_DrawingContext.hpp"

#include <vector>
#include <sstream>

namespace NUIE
{

class SvgBuilder
{
public:
	SvgBuilder ();

	void AddTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes);
	void AddTag (const std::wstring& tag, const std::wstring& content, const std::vector<std::pair<std::wstring, std::wstring>>& attributes);

	void AddOpenTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes);
	void AddCloseTag (const std::wstring& tag);

	void Clear ();

	std::wstring GetAsString () const;

	static std::wstring ToString (double val);
	static std::wstring BegToString (double val);
	static std::wstring EndToString (double val);
	static std::wstring PenToStrokeStyle (const Pen& pen);
	static std::wstring ColorToFillStyle (const Color& color);
	static std::wstring FontToFontStyle (const Font& font);
	static std::wstring PointToPath (const Point& point);

private:
	void AddAttributes (const std::vector<std::pair<std::wstring, std::wstring>>& attributes);

	std::wostringstream svgContent;
};

class SvgDrawingContext : public DrawingContext
{
public:
	SvgDrawingContext (int width, int height);

	std::wstring		GetAsString () const;

	virtual void		Resize (int newWidth, int newHeight) override;

	virtual int			GetWidth () const override;
	virtual int			GetHeight () const override;

	virtual void		BeginDraw () override;
	virtual void		EndDraw () override;

	virtual bool		NeedToDraw (ItemPreviewMode mode) override;

	virtual void		DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void		DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;

	virtual void		DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void		FillRect (const Rect& rect, const Color& color) override;

	virtual void		DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void		FillEllipse (const Rect& rect, const Color& color) override;

	virtual void		DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const NUIE::Color& textColor) override;
	virtual Size		MeasureText (const Font& font, const std::wstring& text) override;
	
	virtual bool		CanDrawIcon () override;
	virtual void		DrawIcon (const Rect& rect, const IconId& iconId) override;

private:
	SvgBuilder	svgBuilder;
	int			width;
	int			height;
};

}

#endif
