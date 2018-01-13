#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include "UIItem.hpp"

namespace UI
{

class Splitter : Item
{
public:
	Splitter (Item& firstItem, Item& secondItem);
	virtual ~Splitter ();

	virtual void	MoveResize (int x, int y, int width, int height) override;

private:
	Item&	firstItem;
	Item&	secondItem;
	double	ratio;
};

}

#endif
