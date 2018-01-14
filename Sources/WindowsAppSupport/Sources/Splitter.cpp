#include "Splitter.hpp"

static const int splitterWidth = 4;
static const int splitterHalfWidth = splitterWidth / 2;
static const int minItemWidth = 20;

namespace UI
{

Splitter::Splitter (Item& firstItem, Item& secondItem) :
	firstItem (firstItem),
	secondItem (secondItem),
	fullWidth (0),
	fullHeight (0),
	ratio (0.5)
{

}

Splitter::~Splitter ()
{

}

bool Splitter::IsSplitterArea (int x, int y) const
{
	Rect firstRect = firstItem.GetRect ();
	Rect secondRect = secondItem.GetRect ();

	if (y < firstRect.y || y > firstRect.y + firstRect.height) {
		return false;
	}

	if (x < firstRect.width || x > secondRect.x) {
		return false;
	}

	return true;
}

bool Splitter::MoveSplitter (int diff)
{
	Rect firstRect = firstItem.GetRect ();
	Rect secondRect = secondItem.GetRect ();

	firstRect.width += diff;
	secondRect.x += diff;
	secondRect.width -= diff;
	if (firstRect.width < minItemWidth || secondRect.width < minItemWidth) {
		return false;
	}

	firstItem.SetRect (firstRect);
	secondItem.SetRect (secondRect);

	int fullWidth = firstRect.width + secondRect.width;
	ratio = (double) firstRect.width / (double) fullWidth;
	return true;
}

void Splitter::MoveResizeItems (int x, int y, int width, int height)
{
	int firstItemWidth = (int) (width * ratio) - splitterHalfWidth;
	int secondItemWidth = width - firstItemWidth - splitterWidth;

	firstItem.SetRect (Rect (x, y, firstItemWidth, height));
	secondItem.SetRect (Rect (x + firstItemWidth + splitterWidth, y, secondItemWidth, height));

	fullWidth = width;
	fullHeight = height;
}

}
