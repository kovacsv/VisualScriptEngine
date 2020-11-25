#include "NUIE_NodeAlignment.hpp"

#include <algorithm>

namespace NUIE
{

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

std::unordered_map<NE::NodeId, NUIE::Point> AlignNodes (Alignment alignment, const std::unordered_map<NE::NodeId, Rect>& rects)
{
	std::unordered_map<NE::NodeId, NUIE::Point> offsets;
	if (DBGERROR (rects.empty ())) {
		return offsets;
	}

	if (alignment == Alignment::Left) {
		Rect minRect = MinElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetLeft () < b.GetLeft ();
		});
		for (const auto& rect : rects) {
			double offsetX = minRect.GetLeft () - rect.second.GetLeft ();
			offsets.insert ({ rect.first, Point (offsetX, 0.0) });
		}
	} else if (alignment == Alignment::Right) {
		Rect maxRect = MaxElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetRight () < b.GetRight ();
		});
		for (const auto& rect : rects) {
			double offsetX = maxRect.GetRight () - rect.second.GetRight ();
			offsets.insert ({ rect.first, Point (offsetX, 0.0) });
		}
	} else if (alignment == Alignment::Top) {
		Rect minRect = MinElement (rects, [&] (const Rect& a, const Rect& b) {
			return a.GetTop () < b.GetTop ();
		});
		for (const auto& rect : rects) {
			double offsetY = minRect.GetTop () - rect.second.GetTop ();
			offsets.insert ({ rect.first, Point (0.0, offsetY) });
		}
	} else if (alignment == Alignment::Bottom) {
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

}
