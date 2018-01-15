#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include "UIItem.hpp"

namespace UI
{

class Splitter : public Item
{
public:
	Splitter (Item& firstItem, Item& secondItem);
	virtual ~Splitter ();

	virtual Rect	GetRect () const override;
	virtual void	SetRect (const Rect& rect) override;

	bool			IsSplitterArea (int x, int y) const;
	void			OnMouseDown (int x, int y);
	void			OnMouseMove (int x, int y);
	void			OnMouseUp ();

private:
	bool			MoveSplitter (int diff);

	virtual int		GetRelevantMousePos (int x, int y) const = 0;
	virtual bool	IsPositionInSplitterArea (int x, int y, const Rect& firstRect, const Rect& secondRect) const = 0;
	virtual bool	RecalculateRectsAfterMove (int diff, Rect& firstRect, Rect& secondRect) const = 0;
	virtual void	RecalculateRectsAfterResize (Rect& firstRect, Rect& secondRect) const = 0;
	virtual double	RecalculateRatio (const Rect& firstRect, const Rect& secondRect) const = 0;

	Item&			firstItem;
	Item&			secondItem;
	int				lastMousePos;

protected:
	Rect			fullRect;
	double			ratio;
};

class HorizontalSplitter : public Splitter
{
public:
	HorizontalSplitter (Item& firstItem, Item& secondItem);
	virtual ~HorizontalSplitter ();

private:
	virtual int		GetRelevantMousePos (int x, int y) const override;
	virtual bool	IsPositionInSplitterArea (int x, int y, const Rect& firstRect, const Rect& secondRect) const override;
	virtual bool	RecalculateRectsAfterMove (int diff, Rect& firstRect, Rect& secondRect) const override;
	virtual void	RecalculateRectsAfterResize (Rect& firstRect, Rect& secondRect) const override;
	virtual double	RecalculateRatio (const Rect& firstRect, const Rect& secondRect) const override;
};

class VerticalSplitter : public Splitter
{
public:
	VerticalSplitter (Item& firstItem, Item& secondItem);
	virtual ~VerticalSplitter ();

private:
	virtual int		GetRelevantMousePos (int x, int y) const override;
	virtual bool	IsPositionInSplitterArea (int x, int y, const Rect& firstRect, const Rect& secondRect) const override;
	virtual bool	RecalculateRectsAfterMove (int diff, Rect& firstRect, Rect& secondRect) const override;
	virtual void	RecalculateRectsAfterResize (Rect& firstRect, Rect& secondRect) const override;
	virtual double	RecalculateRatio (const Rect& firstRect, const Rect& secondRect) const override;
};

}

#endif
