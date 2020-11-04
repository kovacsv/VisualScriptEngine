#ifndef NUIE_VIEWBOX_HPP
#define NUIE_VIEWBOX_HPP

#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"

namespace NUIE
{

class ViewBox
{
public:
	ViewBox ();
	ViewBox (const Point& offset, double scale);

	void			Reset ();
	void			Set (const Point& newOffset, double newScale);

	const Point&	GetOffset () const;
	void			SetOffset (const Point& newOffset);

	double			GetScale () const;
	void			SetScale (double newScale);
	void			SetScale (double newScale, const Point& fixViewPoint);

	Point			ModelToView (const Point& point) const;
	Size			ModelToView (const Size& size) const;
	Rect			ModelToView (const Rect& rect) const;
	Pen				ModelToView (const Pen& pen) const;
	Font			ModelToView (const Font& font) const;

	Point			ViewToModel (const Point& point) const;
	Rect			ViewToModel (const Rect& rect) const;
	Size			ViewToModel (const Size& size) const;

private:
	Point	offset;
	double	scale;
};

ViewBox FitRectToSize (const Size& targetSize, double targetPadding, const Rect& rect);

}

#endif
