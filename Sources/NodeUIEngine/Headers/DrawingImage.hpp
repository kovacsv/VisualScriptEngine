#ifndef DRAWINGIMAGE_HPP
#define DRAWINGIMAGE_HPP

#include "DrawingContext.hpp"
#include "Checksum.hpp"

#include <string>
#include <vector>
#include <memory>

namespace NUIE
{

class DrawingItem;
typedef std::shared_ptr<DrawingItem> DrawingItemPtr;
typedef std::shared_ptr<const DrawingItem> DrawingItemConstPtr;

class DrawingItem
{
public:
	DrawingItem ();
	virtual ~DrawingItem ();

	virtual void Draw (DrawingContext& context) const = 0;
	virtual void AddToChecksum (NE::Checksum& checksum) const = 0;
};

class DrawingLine : public DrawingItem
{
public:
	DrawingLine (const Point& beg, const Point& end, const Pen& pen);
	virtual ~DrawingLine ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Point	beg;
	Point	end;
	Pen		pen;
};

class DrawingBezier : public DrawingItem
{
public:
	DrawingBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen);
	virtual ~DrawingBezier ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Point	p1;
	Point	p2;
	Point	p3;
	Point	p4;
	Pen		pen;
};

class DrawingRect : public DrawingItem
{
public:
	DrawingRect (const Rect& rect, const Pen& pen);
	virtual ~DrawingRect ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Rect	rect;
	Pen		pen;
};

class DrawingFillRect : public DrawingItem
{
public:
	DrawingFillRect (const Rect& rect, const Color& color);
	virtual ~DrawingFillRect ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Rect	rect;
	Color	color;
};

class DrawingEllipse : public DrawingItem
{
public:
	DrawingEllipse (const Rect& rect, const Pen& pen);
	virtual ~DrawingEllipse ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Rect	rect;
	Pen		pen;
};

class DrawingFillEllipse : public DrawingItem
{
public:
	DrawingFillEllipse (const Rect& rect, const Color& color);
	virtual ~DrawingFillEllipse ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Rect	rect;
	Color	color;
};

class DrawingText : public DrawingItem
{
public:
	DrawingText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& backgroundColor, const Color& textColor);
	virtual ~DrawingText ();

	virtual void Draw (DrawingContext& context) const override;
	virtual void AddToChecksum (NE::Checksum& checksum) const override;

private:
	Rect				rect;
	Font				font;
	std::wstring		text;
	HorizontalAnchor	hAnchor;
	VerticalAnchor		vAnchor;
	Color				backgroundColor;
	Color				textColor;
};

class MultiDrawingItem : public DrawingItem
{
public:
	MultiDrawingItem ();
	virtual ~MultiDrawingItem ();

	void			AddItem (const DrawingItemConstPtr& item);

	virtual void	Draw (DrawingContext& context) const override;
	virtual void	AddToChecksum (NE::Checksum& checksum) const override;

private:
	std::vector<DrawingItemConstPtr> items;
};

class DrawingImage
{
public:
	DrawingImage ();
	virtual ~DrawingImage ();

	bool			IsEmpty () const;
	void			Clear ();

	void			AddItem (const DrawingItemConstPtr& item);
	void			RemoveItem (const DrawingItemConstPtr& item);
	void			Draw (DrawingContext& context) const;

	NE::Checksum	GetChecksum () const;

private:
	std::vector<DrawingItemConstPtr> items;
};

}

#endif
