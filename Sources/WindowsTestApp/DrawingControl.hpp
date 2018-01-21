#ifndef DRAWINGCONTROL_HPP
#define DRAWINGCONTROL_HPP

#include "ResultImage.hpp"
#include "BitmapContextGdiplus.hpp"
#include "ViewBox.hpp"

#include "wx/wx.h"
#include <memory>

class DrawingControl : public wxPanel
{
public:
	DrawingControl (wxWindow *parent, const std::shared_ptr<ResultImage>& resultImage);

	void	OnPaint (wxPaintEvent& evt);
	void	OnResize (wxSizeEvent& evt);

	void	OnMiddleButtonDown (wxMouseEvent& evt);
	void	OnMiddleButtonUp (wxMouseEvent& evt);
	void	OnMouseMove (wxMouseEvent& evt);
	void	OnMouseWheel (wxMouseEvent& evt);

	void	ClearImage ();
	void	RedrawImage ();

private:
	std::shared_ptr<ResultImage>		resultImage;

	BitmapContextGdiplus				drawingContext;
	NUIE::ViewBox						viewBox;
	std::unique_ptr<NUIE::Point>		lastMousePos;

	DECLARE_EVENT_TABLE ()
};


#endif
