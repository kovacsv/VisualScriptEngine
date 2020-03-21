#include "WXAS_wxDrawingContext.hpp"
#include "NE_Debug.hpp"

#include <wx/wx.h>

namespace WXAS
{

wxDrawingContext::wxDrawingContext () :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	memoryBitmap (new wxBitmap (wxSize (1, 1))),
	memoryDC (new wxMemoryDC ())
{
	memoryDC->SelectObject (*memoryBitmap);
	graphicsContext = wxGraphicsContext::Create (*memoryDC);
}

wxDrawingContext::~wxDrawingContext ()
{
	delete memoryBitmap;
	delete memoryDC;
	delete graphicsContext;
}

void wxDrawingContext::DrawToDC (wxDC* targetDC)
{
	targetDC->Blit (0, 0, width, height, memoryDC, 0, 0);
}

void wxDrawingContext::Init (void*)
{

}

void wxDrawingContext::BlitToWindow (void* nativeHandle)
{
	wxPanel* panel = (wxPanel*) nativeHandle;
	wxPaintDC dc (panel);
	BlitToContext (&dc);
}

void wxDrawingContext::BlitToContext (void* nativeContext)
{
	wxPaintDC* dc = (wxPaintDC*) nativeContext;
	dc->Blit (0, 0, width, height, memoryDC, 0, 0);
}

void wxDrawingContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;

	delete memoryBitmap;
	delete memoryDC;
	delete graphicsContext;

	memoryBitmap = new wxBitmap (newWidth, newHeight);
	memoryDC = new wxMemoryDC ();
	memoryDC->SelectObject (*memoryBitmap);
	graphicsContext = wxGraphicsContext::Create (*memoryDC);
}

double wxDrawingContext::GetWidth () const
{
	return width;
}

double wxDrawingContext::GetHeight () const
{
	return height;
}

void wxDrawingContext::BeginDraw ()
{

}

void wxDrawingContext::EndDraw ()
{

}

bool wxDrawingContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void wxDrawingContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
	graphicsContext->SetPen (GetPen (pen));
	std::vector<wxPoint2DDouble> lines;
	lines.push_back (wxPoint2DDouble (beg.GetX (), beg.GetY ()));
	lines.push_back (wxPoint2DDouble (end.GetX (), end.GetY ()));
	graphicsContext->DrawLines (lines.size (), lines.data ());
}

void wxDrawingContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
	graphicsContext->SetPen (GetPen (pen));
	wxGraphicsPath path = graphicsContext->CreatePath ();
	wxPoint wxp1 = GetPoint (p1);
	wxPoint wxp2 = GetPoint (p2);
	wxPoint wxp3 = GetPoint (p3);
	wxPoint wxp4 = GetPoint (p4);
	path.MoveToPoint (wxp1);
	path.AddCurveToPoint (wxp2.x, wxp2.y, wxp3.x, wxp3.y, wxp4.x, wxp4.y);
	graphicsContext->DrawPath (path);
}

void wxDrawingContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
	graphicsContext->SetPen (GetPen (pen));
	wxRect theRect = GetRect (rect);
	graphicsContext->DrawRectangle (theRect.GetX (), theRect.GetY (), theRect.GetWidth (), theRect.GetHeight ());	
}

void wxDrawingContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	graphicsContext->SetBrush (wxBrush (GetColor (color)));
	graphicsContext->SetPen (*wxTRANSPARENT_PEN);
	wxRect theRect = GetRect (rect);
	graphicsContext->DrawRectangle (theRect.GetX (), theRect.GetY (), theRect.GetWidth (), theRect.GetHeight ());	
}

void wxDrawingContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	graphicsContext->SetBrush (*wxTRANSPARENT_BRUSH);
	graphicsContext->SetPen (GetPen (pen));
	wxRect theRect = GetRect (rect);
	graphicsContext->DrawEllipse (theRect.GetX (), theRect.GetY (), theRect.GetWidth (), theRect.GetHeight ());	
}

void wxDrawingContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	graphicsContext->SetBrush (wxBrush (GetColor (color)));
	graphicsContext->SetPen (*wxTRANSPARENT_PEN);
	wxRect theRect = GetRect (rect);
	graphicsContext->DrawEllipse (theRect.GetX (), theRect.GetY (), theRect.GetWidth (), theRect.GetHeight ());	
}

void wxDrawingContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	memoryDC->SetTextForeground (GetColor (textColor));
	memoryDC->SetFont (GetFont (font));

	int alignment = 0;

	switch (hAnchor) {
		case NUIE::HorizontalAnchor::Left:
			alignment |= wxALIGN_LEFT;
			break;
		case NUIE::HorizontalAnchor::Center:
			alignment |= wxALIGN_CENTER_HORIZONTAL;
			break;
		case NUIE::HorizontalAnchor::Right:
			alignment |= wxALIGN_RIGHT;
			break;
	}

	switch (vAnchor) {
		case NUIE::VerticalAnchor::Top:
			alignment |= wxALIGN_TOP;
			break;
		case NUIE::VerticalAnchor::Center:
			alignment |= wxALIGN_CENTER_VERTICAL;
			break;
		case NUIE::VerticalAnchor::Bottom:
			alignment |= wxALIGN_BOTTOM;
			break;
	}

	memoryDC->DrawLabel (text, GetRect (rect), alignment);
}

NUIE::Size wxDrawingContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	memoryDC->SetFont (GetFont (font));
	wxSize size = memoryDC->GetTextExtent (text);
	return NUIE::Size (size.x, size.y);
}

void wxDrawingContext::DrawIcon (const NUIE::Rect&, const NUIE::IconId&)
{
	DBGBREAK ();
}

wxPoint wxDrawingContext::GetPoint (const NUIE::Point& point)
{
	return wxPoint ((int) std::floor (point.GetX ()) - 1, (int) std::floor (point.GetY ()) - 1);
}

wxRect wxDrawingContext::GetRect (const NUIE::Rect& rect)
{
	NUIE::IntRect intRect (rect);
	return wxRect (intRect.GetX (), intRect.GetY (), intRect.GetWidth (), intRect.GetHeight ());
}

wxColour wxDrawingContext::GetColor (const NUIE::Color& color)
{
	return wxColour (color.GetR (), color.GetG (), color.GetB ());
}

wxPen wxDrawingContext::GetPen (const NUIE::Pen& pen)
{
	int thickness = (int) pen.GetThickness ();
	return wxPen (GetColor (pen.GetColor ()), thickness);
}

wxFont wxDrawingContext::GetFont (const NUIE::Font& font)
{
	int fontSize = (int) font.GetSize ();
	if (fontSize == wxDEFAULT) {
		// TODO: wxFont handles font size 70 as the default, so changes it back to default font size
		fontSize += 1;
	}
	return wxFont (fontSize, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
}

}
