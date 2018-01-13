#include "Splitter.hpp"

namespace UI
{

Splitter::Splitter (Item& firstItem, Item& secondItem) :
	Item (),
	firstItem (firstItem),
	secondItem (secondItem),
	ratio (0.5)
{

}

Splitter::~Splitter ()
{

}

void Splitter::MoveResize (int x, int y, int width, int height)
{
	static const int splitterWidth = 4;
	static const int splitterHalfWidth = splitterWidth / 2;

	int firstItemWidth = (int) (width * ratio) - splitterHalfWidth;
	int secondItemWidth = width - firstItemWidth - splitterWidth;

	firstItem.MoveResize (x, y, firstItemWidth, height);
	secondItem.MoveResize (x + firstItemWidth + splitterWidth, y, secondItemWidth, height);
}

}
