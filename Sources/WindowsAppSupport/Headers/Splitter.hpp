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
	bool			MoveSplitter (int diff);

private:
	Item&	firstItem;
	Item&	secondItem;

	Rect	fullRect;
	double	ratio;
};

}

#endif
