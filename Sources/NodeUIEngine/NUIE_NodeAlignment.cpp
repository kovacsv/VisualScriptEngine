#include "NUIE_NodeAlignment.hpp"

#include <algorithm>

namespace NUIE
{

static const double DefaultGapIfOverlap = 20.0;

enum class Direction
{
	Horizontal,
	Vertical
};

static Rect MinElement (const std::unordered_map<NE::NodeId, Rect>& rects, const std::function<bool (const Rect&, const Rect&)>& comp)
{
	auto minElement = std::min_element (rects.begin (), rects.end (), [&] (const std::pair<NE::NodeId, Rect>& a, const std::pair<NE::NodeId, Rect>& b) {
		return comp (a.second, b.second);
	});
	return minElement->second;
}

static Rect MaxElement (const std::unordered_map<NE::NodeId, Rect>& rects, const std::function<bool (const Rect&, const Rect&)>& comp)
{
	auto minElement = std::max_element (rects.begin (), rects.end (), [&] (const std::pair<NE::NodeId, Rect>& a, const std::pair<NE::NodeId, Rect>& b) {
		return comp (a.second, b.second);
	});
	return minElement->second;
}

static double GetAverage (const std::unordered_map<NE::NodeId, Rect>& rects, const std::function<double (const Rect&)>& proc)
{
	double sum = 0.0;
	for (const auto& rect : rects) {
		sum += proc (rect.second);
	}
	return sum / (double) rects.size ();
}

static double GetRectBegin (const Rect& rect, Direction dir)
{
	if (dir == Direction::Horizontal) {
		return rect.GetLeft ();
	} else if (dir == Direction::Vertical) {
		return rect.GetTop ();
	} else {
		DBGBREAK ();
		return 0.0;
	}
}

static double GetRectEnd (const Rect& rect, Direction dir)
{
	if (dir == Direction::Horizontal) {
		return rect.GetRight ();
	} else if (dir == Direction::Vertical) {
		return rect.GetBottom ();
	} else {
		DBGBREAK ();
		return 0.0;
	}
}

static double GetRectSize (const Rect& rect, Direction dir)
{
	if (dir == Direction::Horizontal) {
		return rect.GetWidth ();
	} else if (dir == Direction::Vertical) {
		return rect.GetHeight ();
	} else {
		DBGBREAK ();
		return 0.0;
	}
}

static double GetComponent (const Point& point, Direction dir)
{
	if (dir == Direction::Horizontal) {
		return point.GetX ();
	} else if (dir == Direction::Vertical) {
		return point.GetY ();
	} else {
		DBGBREAK ();
		return 0.0;
	}
}

static Point CreatePoint (double component, Direction dir)
{
	if (dir == Direction::Horizontal) {
		return Point (component, 0.0);
	} else if (dir == Direction::Vertical) {
		return Point (0.0, component);
	} else {
		DBGBREAK ();
		return Point (0.0, 0.0);
	}
}

std::unordered_map<NE::NodeId, NUIE::Point> AlignNodes (Alignment alignment, const std::unordered_map<NE::NodeId, Rect>& rects)
{
	std::unordered_map<NE::NodeId, NUIE::Point> offsets;
	if (DBGERROR (rects.empty ())) {
		return offsets;
	}

	if (alignment == Alignment::HorizontalLeft) {
		Rect minRect = MinElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetLeft () < b.GetLeft ();
		});
		for (const auto& rect : rects) {
			double offsetX = minRect.GetLeft () - rect.second.GetLeft ();
			offsets.insert ({ rect.first, Point (offsetX, 0.0) });
		}
	} else if (alignment == Alignment::HorizontalRight) {
		Rect maxRect = MaxElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetRight () < b.GetRight ();
		});
		for (const auto& rect : rects) {
			double offsetX = maxRect.GetRight () - rect.second.GetRight ();
			offsets.insert ({ rect.first, Point (offsetX, 0.0) });
		}
	} else if (alignment == Alignment::VerticalTop) {
		Rect minRect = MinElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetTop () < b.GetTop ();
		});
		for (const auto& rect : rects) {
			double offsetY = minRect.GetTop () - rect.second.GetTop ();
			offsets.insert ({ rect.first, Point (0.0, offsetY) });
		}
	} else if (alignment == Alignment::VerticalBottom) {
		Rect maxRect = MaxElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetBottom () < b.GetBottom ();
		});
		for (const auto& rect : rects) {
			double offsetY = maxRect.GetBottom () - rect.second.GetBottom ();
			offsets.insert ({ rect.first, Point (0.0, offsetY) });
		}
	} else if (alignment == Alignment::HorizontalCenter) {
		double avgCenterX = GetAverage (rects, [&] (const Rect& rect) {
			return rect.GetCenter ().GetX ();
		});
		for (const auto& rect : rects) {
			double offsetX = avgCenterX - rect.second.GetCenter ().GetX ();
			offsets.insert ({ rect.first, Point (offsetX, 0.0) });
		}
	} else if (alignment == Alignment::VerticalCenter) {
		double avgCenterY = GetAverage (rects, [&] (const Rect& rect) {
			return rect.GetCenter ().GetY ();
		});
		for (const auto& rect : rects) {
			double offsetY = avgCenterY - rect.second.GetCenter ().GetY ();
			offsets.insert ({ rect.first, Point (0.0, offsetY) });
		}
	} else {
		DBGBREAK ();
	}

	DBGASSERT (rects.size () == offsets.size ());
	return offsets;
}

std::unordered_map<NE::NodeId, NUIE::Point> DistributeNodes (Distribution distribution, const std::unordered_map<NE::NodeId, Rect>& rects)
{
	std::unordered_map<NE::NodeId, NUIE::Point> offsets;
	if (DBGERROR (rects.empty ())) {
		return offsets;
	}

	Direction dir;
	if (distribution == Distribution::Horizontal) {
		dir = Direction::Horizontal;
	} else if (distribution == Distribution::Vertical) {
		dir = Direction::Vertical;
	} else {
		DBGBREAK ();
		return offsets;
	}

	double min = std::numeric_limits<double>::max ();
	double max = std::numeric_limits<double>::min ();
	double rectsSumSize = 0.0;
	std::vector<NE::NodeId> sortedNodes;
	for (const auto& rect : rects) {
		const Rect& nodeRect = rect.second;
		min = std::min (min, GetRectBegin (nodeRect, dir));
		max = std::max (max, GetRectEnd (nodeRect, dir));
		rectsSumSize += GetRectSize (nodeRect, dir);
		sortedNodes.push_back (rect.first);
	}

	double rangeSize = max - min;
	double gapSize = DefaultGapIfOverlap;
	if (IsGreater (rangeSize, rectsSumSize)) {
		gapSize = (rangeSize - rectsSumSize) / ((double) rects.size () - 1);
	}
	
	std::sort (sortedNodes.begin (), sortedNodes.end (), [&] (const NE::NodeId& a, const NE::NodeId& b) {
		return GetRectBegin (rects.at (a), dir) < GetRectBegin (rects.at (b), dir);
	});
	for (size_t i = 0; i < sortedNodes.size (); i++) {
		const NE::NodeId& nodeId = sortedNodes[i];
		if (i == 0) {
			offsets.insert ({ nodeId, Point (0.0, 0.0) });
		} else {
			const Rect& nodeRect = rects.at (nodeId);
			const NE::NodeId& prevNodeId = sortedNodes[i - 1];
			const Rect& prevRect = rects.at (prevNodeId);
			const Point& prevOffset = offsets.at (prevNodeId);
			double startPosition = GetRectEnd (prevRect, dir) + GetComponent (prevOffset, dir) + gapSize;
			double offset = startPosition - GetRectBegin (nodeRect, dir);
			offsets.insert ({ nodeId, CreatePoint (offset, dir) });
		}
	}

	DBGASSERT (rects.size () == offsets.size ());
	return offsets;
}

}
