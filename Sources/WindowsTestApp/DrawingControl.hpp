#ifndef DRAWINGCONTROL_HPP
#define DRAWINGCONTROL_HPP

#include "CustomControl.hpp"
#include "ResultImage.hpp"
#include "WinDrawingContext.hpp"
#include "ViewBox.hpp"

#include <memory>

class DrawingControl : public UI::CustomControl
{
public:
	DrawingControl (const std::shared_ptr<ResultImage>& resultImage);

	virtual void	OnCreate (HWND hwnd) override;
	virtual void	OnPaint (HWND hwnd) override;
	virtual void	OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void	OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void	OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y) override;
	virtual void	OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta) override;
	virtual void	OnResize (HWND hwnd, int newWidth, int newHeight) override;

	void			ClearImage ();
	void			RedrawImage ();

private:
	std::shared_ptr<ResultImage>		resultImage;

	std::shared_ptr<WinDrawingContext>	drawingContext;
	NUIE::ViewBox						viewBox;
	std::unique_ptr<NUIE::Point>		lastMousePos;
};

#endif
