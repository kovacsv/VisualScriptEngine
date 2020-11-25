#ifndef NUIE_NODEALIGNMENT_HPP
#define NUIE_NODEALIGNMENT_HPP

#include "NE_NodeId.hpp"
#include "NUIE_Geometry.hpp"

#include <unordered_map>

namespace NUIE
{

enum class Alignment
{
	HorizontalLeft,
	HorizontalRight,
	VerticalTop,
	VerticalBottom,
	HorizontalCenter,
	VerticalCenter
};

enum class Distribution
{
	Horizontal,
	Vertical
};

std::unordered_map<NE::NodeId, Point> AlignNodes (Alignment alignment, const std::unordered_map<NE::NodeId, Rect>& rects);
std::unordered_map<NE::NodeId, Point> DistributeNodes (Distribution distribution, const std::unordered_map<NE::NodeId, Rect>& rects);

}

#endif
