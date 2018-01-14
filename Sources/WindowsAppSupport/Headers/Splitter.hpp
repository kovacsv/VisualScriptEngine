#ifndef SPLITTER_HPP
#define SPLITTER_HPP

#include "UIItem.hpp"

namespace UI
{

class Splitter
{
public:
	Splitter (Item& firstItem, Item& secondItem);
	virtual ~Splitter ();

	bool	IsSplitterArea (int x, int y) const;

	bool	MoveSplitter (int diff);
	void	MoveResizeItems (int x, int y, int width, int height);

private:
	Item&	firstItem;
	Item&	secondItem;

	int		fullWidth;
	int		fullHeight;
	double	ratio;
};

}

#endif
