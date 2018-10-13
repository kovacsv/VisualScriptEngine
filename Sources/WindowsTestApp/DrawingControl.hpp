#ifndef DRAWINGCONTROL_HPP
#define DRAWINGCONTROL_HPP

#include "NUIE_DrawingContext.hpp"
#include "NUIE_ViewBox.hpp"
#include "WXAS_ControlUtilities.hpp"
#include "ResultImage.hpp"

#include <wx/wx.h>
#include <memory>

class DrawingControl : public wxPanel
{
public:
	DrawingControl (wxWindow *parent, const std::shared_ptr<ResultImage>& resultImage);

	void	OnPaint (wxPaintEvent& evt);
	void	OnResize (wxSizeEvent& evt);
	void	OnMouseCaptureLost (wxMouseCaptureLostEvent& evt);

	void	OnRightButtonDown (wxMouseEvent& evt);
	void	OnRightButtonUp (wxMouseEvent& evt);
	void	OnMouseMove (wxMouseEvent& evt);
	void	OnMouseWheel (wxMouseEvent& evt);

	void	ClearImage ();
	void	RedrawImage ();

private:
	WXAS::MouseCaptureHandler			captureHandler;
	std::shared_ptr<ResultImage>		resultImage;

	std::shared_ptr<NUIE::NativeDrawingContext>		drawingContext;
	NUIE::ViewBox									viewBox;
	std::unique_ptr<NUIE::Point>					lastMousePos;

	DECLARE_EVENT_TABLE ()
};


#endif
