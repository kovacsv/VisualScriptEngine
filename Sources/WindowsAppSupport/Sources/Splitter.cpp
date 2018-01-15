#include "Splitter.hpp"

static const int splitterWidth = 4;
static const int splitterHalfWidth = splitterWidth / 2;
static const int minItemWidth = 20;

namespace UI
{

Splitter::Splitter (Item& firstItem, Item& secondItem) :
	Item (),
	firstItem (firstItem),
	secondItem (secondItem),
	lastMousePos (0),
	fullRect (0, 0, 0, 0),
	ratio (0.5)
{

}

Splitter::~Splitter ()
{

}

Rect Splitter::GetRect () const
{
	return fullRect;
}

void Splitter::SetRect (const Rect& rect)
{
	fullRect = rect;
	
	Rect firstRect;
	Rect secondRect;
	RecalculateRectsAfterResize (firstRect, secondRect);
	firstItem.SetRect (firstRect);
	secondItem.SetRect (secondRect);
}

bool Splitter::IsSplitterArea (int x, int y) const
{
	Rect firstRect = firstItem.GetRect ();
	Rect secondRect = secondItem.GetRect ();
	return IsPositionInSplitterArea (x, y, firstRect, secondRect);
}

void Splitter::OnMouseDown (int x, int y)
{
	lastMousePos = GetRelevantMousePos (x, y);
}

void Splitter::OnMouseMove (int x, int y)
{
	if (lastMousePos != 0) {
		int currentMousePos = GetRelevantMousePos (x, y);
		MoveSplitter (currentMousePos - lastMousePos);
		lastMousePos = currentMousePos;
	}
}

void Splitter::OnMouseUp ()
{
	lastMousePos = 0;
}

bool Splitter::MoveSplitter (int diff)
{
	Rect firstRect = firstItem.GetRect ();
	Rect secondRect = secondItem.GetRect ();

	if (!RecalculateRectsAfterMove (diff, firstRect, secondRect)) {
		return false;
	}

	firstItem.SetRect (firstRect);
	secondItem.SetRect (secondRect);

	ratio = RecalculateRatio (firstRect, secondRect);
	return true;
}

HorizontalSplitter::HorizontalSplitter (Item& firstItem, Item& secondItem) :
	Splitter (firstItem, secondItem)
{

}

HorizontalSplitter::~HorizontalSplitter ()
{

}

int HorizontalSplitter::GetRelevantMousePos (int x, int y) const
{
	return y;
}

bool HorizontalSplitter::IsPositionInSplitterArea (int x, int y, const Rect& firstRect, const Rect& secondRect) const
{
	return	y >= firstRect.height && y <= secondRect.y &&
			x >= firstRect.x && x <= firstRect.x + firstRect.width;
}

bool HorizontalSplitter::RecalculateRectsAfterMove (int diff, Rect& firstRect, Rect& secondRect) const
{
	firstRect.height += diff;
	secondRect.y += diff;
	secondRect.height -= diff;
	if (firstRect.height < minItemWidth || secondRect.height < minItemWidth) {
		return false;
	}

	return true;
}

void HorizontalSplitter::RecalculateRectsAfterResize (Rect& firstRect, Rect& secondRect) const
{
	int firstItemHeight = (int) (fullRect.height * ratio) - splitterHalfWidth;
	int secondItemHeight = fullRect.height - firstItemHeight - splitterWidth;
	firstRect = Rect (fullRect.x, fullRect.y, fullRect.width, firstItemHeight);
	secondRect = Rect (fullRect.x, fullRect.y + firstItemHeight + splitterWidth, fullRect.width, secondItemHeight);
}

double HorizontalSplitter::RecalculateRatio (const Rect& firstRect, const Rect& secondRect) const
{
	int fullHeight = firstRect.height + secondRect.height;
	return (double) firstRect.height / (double) fullHeight;
}

VerticalSplitter::VerticalSplitter (Item& firstItem, Item& secondItem) :
	Splitter (firstItem, secondItem)
{

}

VerticalSplitter::~VerticalSplitter ()
{

}

int VerticalSplitter::GetRelevantMousePos (int x, int y) const
{
	return x;
}

bool VerticalSplitter::IsPositionInSplitterArea (int x, int y, const Rect& firstRect, const Rect& secondRect) const
{
	return	x >= firstRect.width && x <= secondRect.x &&
			y >= firstRect.y && y <= firstRect.y + firstRect.height;
}

bool VerticalSplitter::RecalculateRectsAfterMove (int diff, Rect& firstRect, Rect& secondRect) const
{
	firstRect.width += diff;
	secondRect.x += diff;
	secondRect.width -= diff;
	if (firstRect.width < minItemWidth || secondRect.width < minItemWidth) {
		return false;
	}

	return true;
}

void VerticalSplitter::RecalculateRectsAfterResize (Rect& firstRect, Rect& secondRect) const
{
	int firstItemWidth = (int) (fullRect.width * ratio) - splitterHalfWidth;
	int secondItemWidth = fullRect.width - firstItemWidth - splitterWidth;
	firstRect = Rect (fullRect.x, fullRect.y, firstItemWidth, fullRect.height);
	secondRect = Rect (fullRect.x + firstItemWidth + splitterWidth, fullRect.y, secondItemWidth, fullRect.height);
}

double VerticalSplitter::RecalculateRatio (const Rect& firstRect, const Rect& secondRect) const
{
	int fullWidth = firstRect.width + secondRect.width;
	return (double) firstRect.width / (double) fullWidth;
}

}
