#include "SimpleTest.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_SvgDrawingContext.hpp"

#include "TestReference.hpp"

using namespace NUIE;

namespace SegmentBezierTest
{

static void DrawBezier (SvgDrawingContext& context, const Point& p1, const Point& p2, const Point& p3, const Point& p4)
{
	Pen pen (Color (200, 200, 200), 10.0);
	context.BeginDraw ();
	
	context.DrawBezier (p1, p2, p3, p4, pen);
	Pen segmentedPen (Color (0, 0, 200), 3.0);
	std::vector<NUIE::Point> points = SegmentBezier (20, p1, p2, p3, p4);
	for (size_t pointIndex = 0; pointIndex < points.size () - 1; pointIndex++) {
		context.DrawLine (points[pointIndex], points[pointIndex + 1], segmentedPen);
	}

	context.EndDraw ();
}

TEST (SegmentBezierTest1)
{
	SvgDrawingContext context (500, 500);
	DrawBezier (context, Point (100.0, 100.0), Point (200.0, 100.0), Point (200.0, 300.0), Point (300.0, 300.0));
	CheckDrawingReference (context, L"SegmentBezier1.svg");
}

TEST (SegmentBezierTest2)
{
	SvgDrawingContext context (500, 500);
	DrawBezier (context, Point (100.0, 100.0), Point (500.0, 100.0), Point (0.0, 300.0), Point (300.0, 300.0));
	CheckDrawingReference (context, L"SegmentBezier2.svg");
}

TEST (SegmentBezierTest3)
{
	SvgDrawingContext context (500, 500);
	DrawBezier (context, Point (100.0, 100.0), Point (500.0, 0.0), Point (-200.0, 400.0), Point (300.0, 300.0));
	CheckDrawingReference (context, L"SegmentBezier3.svg");
}

}
