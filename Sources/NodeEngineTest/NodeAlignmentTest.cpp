#include "SimpleTest.hpp"
#include "NUIE_NodeAlignment.hpp"

#include <string>

using namespace NE;
using namespace NUIE;

namespace NodeAlignmentTest
{

TEST (NodeAlignmentTestHorizontal)
{
	std::unordered_map<NodeId, Rect> rects = {
		{ NodeId (1), Rect (10.0, 10.0, 20.0, 10.0) },
		{ NodeId (2), Rect (30.0, 30.0, 30.0, 20.0) },
		{ NodeId (3), Rect (20.0, 80.0, 10.0, 40.0) }
	};

	std::unordered_map<NodeId, Point> leftOffsets = AlignNodes (Alignment::HorizontalLeft, rects);
	ASSERT (IsEqual (leftOffsets[NodeId (1)], Point (0.0, 0.0)));
	ASSERT (IsEqual (leftOffsets[NodeId (2)], Point (-20.0, 0.0)));
	ASSERT (IsEqual (leftOffsets[NodeId (3)], Point (-10.0, 0.0)));

	std::unordered_map<NodeId, Point> rightOffsets = AlignNodes (Alignment::HorizontalRight, rects);
	ASSERT (IsEqual (rightOffsets[NodeId (1)], Point (30.0, 0.0)));
	ASSERT (IsEqual (rightOffsets[NodeId (2)], Point (0.0, 0.0)));
	ASSERT (IsEqual (rightOffsets[NodeId (3)], Point (30.0, 0.0)));

	std::unordered_map<NodeId, Point> centerOffsets = AlignNodes (Alignment::HorizontalCenter, rects);
	ASSERT (IsEqual (centerOffsets[NodeId (1)], Point (10.0, 0.0)));
	ASSERT (IsEqual (centerOffsets[NodeId (2)], Point (-15.0, 0.0)));
	ASSERT (IsEqual (centerOffsets[NodeId (3)], Point (5.0, 0.0)));
}

TEST (NodeAlignmentTestVertical)
{
	std::unordered_map<NodeId, Rect> rects = {
		{ NodeId (1), Rect (10.0, 10.0, 10.0, 20.0) },
		{ NodeId (2), Rect (30.0, 30.0, 20.0, 30.0) },
		{ NodeId (3), Rect (80.0, 20.0, 40.0, 10.0) }
	};

	std::unordered_map<NodeId, Point> topOffsets = AlignNodes (Alignment::VerticalTop, rects);
	ASSERT (IsEqual (topOffsets[NodeId (1)], Point (0.0, 0.0)));
	ASSERT (IsEqual (topOffsets[NodeId (2)], Point (0.0, -20.0)));
	ASSERT (IsEqual (topOffsets[NodeId (3)], Point (0.0, -10.0)));

	std::unordered_map<NodeId, Point> bottomOffsets = AlignNodes (Alignment::VerticalBottom, rects);
	ASSERT (IsEqual (bottomOffsets[NodeId (1)], Point (0.0, 30.0)));
	ASSERT (IsEqual (bottomOffsets[NodeId (2)], Point (0.0, 0.0)));
	ASSERT (IsEqual (bottomOffsets[NodeId (3)], Point (0.0, 30.0)));

	std::unordered_map<NodeId, Point> centerOffsets = AlignNodes (Alignment::VerticalCenter, rects);
	ASSERT (IsEqual (centerOffsets[NodeId (1)], Point (0.0, 10.0)));
	ASSERT (IsEqual (centerOffsets[NodeId (2)], Point (0.0, -15.0)));
	ASSERT (IsEqual (centerOffsets[NodeId (3)], Point (0.0, 5.0)));
}

TEST (NodeDistributionTestHorizontal)
{
	std::unordered_map<NodeId, Rect> rects = {
		{ NodeId (1), Rect (10.0, 10.0, 10.0, 20.0) },
		{ NodeId (2), Rect (30.0, 30.0, 20.0, 30.0) },
		{ NodeId (3), Rect (80.0, 20.0, 40.0, 10.0) }
	};

	std::unordered_map<NodeId, Point> distributeOffsets = DistributeNodes (Distribution::Horizontal, rects);
	ASSERT (IsEqual (distributeOffsets[NodeId (1)], Point (0.0, 0.0)));
	ASSERT (IsEqual (distributeOffsets[NodeId (2)], Point (10.0, 0.0)));
	ASSERT (IsEqual (distributeOffsets[NodeId (3)], Point (0.0, 0.0)));
}

TEST (NodeDistributionTestHorizontalOverlapping)
{
	std::unordered_map<NodeId, Rect> rects = {
		{ NodeId (1), Rect (10.0, 10.0, 30.0, 10.0) },
		{ NodeId (2), Rect (20.0, 10.0, 30.0, 10.0) },
		{ NodeId (3), Rect (30.0, 10.0, 30.0, 10.0) }
	};

	std::unordered_map<NodeId, Point> distributeOffsets = DistributeNodes (Distribution::Horizontal, rects);
	ASSERT (IsEqual (distributeOffsets[NodeId (1)], Point (0.0, 0.0)));
	ASSERT (IsEqual (distributeOffsets[NodeId (2)], Point (40.0, 0.0)));
	ASSERT (IsEqual (distributeOffsets[NodeId (3)], Point (80.0, 0.0)));
}

TEST (NodeDistributionTestVertical)
{
	std::unordered_map<NodeId, Rect> rects = {
		{ NodeId (1), Rect (10.0, 10.0, 20.0, 10.0) },
		{ NodeId (2), Rect (30.0, 30.0, 30.0, 20.0) },
		{ NodeId (3), Rect (20.0, 80.0, 10.0, 40.0) }
	};

	std::unordered_map<NodeId, Point> distributeOffsets = DistributeNodes (Distribution::Vertical, rects);
	ASSERT (IsEqual (distributeOffsets[NodeId (1)], Point (0.0, 0.0)));
	ASSERT (IsEqual (distributeOffsets[NodeId (2)], Point (0.0, 10.0)));
	ASSERT (IsEqual (distributeOffsets[NodeId (3)], Point (0.0, 0.0)));
}

}
