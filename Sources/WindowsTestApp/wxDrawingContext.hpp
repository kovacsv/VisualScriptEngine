#ifndef WXDRAWINGCONTEXT_HPP
#define WXDRAWINGCONTEXT_HPP

#include "DrawingContext.hpp"

#include "wx/dcmemory.h"

class wxDrawingContext : public NUIE::DrawingContext
{
public:
	wxDrawingContext ();
	wxDrawingContext (const wxDrawingContext& rhs) = delete;
	virtual ~wxDrawingContext ();

	void						DrawToDC (wxDC* target);

	virtual void				Resize (int newWidth, int newHeight) override;

	virtual double				GetWidth () const override;
	virtual double				GetHeight () const override;

	virtual void				BeginDraw () override;
	virtual void				EndDraw () override;

	virtual void				DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen) override;
	virtual void				DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen) override;

	virtual void				DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void				FillRect (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void				DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void				FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void				DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor) override;
	virtual NUIE::Size			MeasureText (const NUIE::Font& font, const std::wstring& text) override;

private:
	wxPoint						GetPoint (const NUIE::Point& point);
	wxRect						GetRect (const NUIE::Rect& rect);
	wxColour					GetColor (const NUIE::Color& color);
	wxPen						GetPen (const NUIE::Pen& pen);

	int							width;
	int							height;
	wxBitmap*					memoryBitmap;
	wxMemoryDC*					memoryDC;

};

#endif
