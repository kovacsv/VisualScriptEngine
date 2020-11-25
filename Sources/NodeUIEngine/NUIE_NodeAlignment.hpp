#ifndef NUIE_NODEALIGNMENT_HPP
#define NUIE_NODEALIGNMENT_HPP

#include "NE_NodeId.hpp"
#include "NUIE_Geometry.hpp"

#include <unordered_map>

namespace NUIE
{

enum class Alignment
{
	Left,
	Right,
	Top,
	Bottom,
	HorizontalCenter,
	VerticalCenter
};

std::unordered_map<NE::NodeId, Point> AlignNodes (Alignment alignment, const std::unordered_map<NE::NodeId, Rect>& rects);

}

#endif
