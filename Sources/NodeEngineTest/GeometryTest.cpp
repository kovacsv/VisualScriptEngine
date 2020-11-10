#include "SimpleTest.hpp"
#include "NUIE_Geometry.hpp"

using namespace NUIE;

namespace GeometryTest
{

TEST (EpsilonTest)
{
	ASSERT (IsEqual (0.1 + 0.2, 0.3));
	ASSERT (IsEqual (42.000000001, 42.000000002));
	ASSERT (IsLower (42.1, 42.2));
	ASSERT (!IsLower (42.1, 42.100000001));
	ASSERT (IsGreater (42.2, 42.1));
	ASSERT (!IsGreater (42.2, 42.199999999));
}

TEST (PointTest)
{
	Point p1 (1.0, 2.0);
	Point p2 (3.0, 4.0);
	ASSERT (p1.GetX () == 1.0);
	ASSERT (p1.GetY () == 2.0);
	ASSERT (p1 == Point (1.0, 2.0));
	ASSERT (p1 != Point (1.0, 3.0));
	ASSERT (p1 + p2 == Point (4.0, 6.0));
	ASSERT (p1 - p2 == Point (-2.0, -2.0));
	ASSERT (p1 * 2.0 == Point (2.0, 4.0));
	ASSERT (p1 / 2.0 == Point (0.5, 1.0));
	ASSERT (Point::Distance (p1, Point (2.0, 2.0)) == 1.0);
	ASSERT (Point::Distance (p1, Point (1.0, 3.0)) == 1.0);
	ASSERT (Point::GetMidPoint (p1, Point (3.0, 2.0)) == Point (2.0, 2.0));
}

TEST (SizeTest)
{
	Size s1 (1.0, 2.0);
	Size s2 (3.0, 4.0);
	ASSERT (s1.GetWidth () == 1.0);
	ASSERT (s1.GetHeight () == 2.0);
	ASSERT (s1 == Size (1.0, 2.0));
	ASSERT (s1 != Size (1.0, 3.0));
	ASSERT (s1 + s2 == Size (4.0, 6.0));
	ASSERT (s1 - s2 == Size (-2.0, -2.0));
	ASSERT (s1 * 2.0 == Size (2.0, 4.0));
	ASSERT (s1 / 2.0 == Size (0.5, 1.0));
	s1 = s1.Grow (2.0, 3.0);
	ASSERT (s1 == Size (3.0, 5.0));
	s1.SetWidth (4.0);
	s1.SetHeight (6.0);
	ASSERT (s1 == Size (4.0, 6.0));
}

TEST (RectTest)
{
	Rect r1 (1.0, 2.0, 3.0, 4.0);
	
	ASSERT (r1 == Rect (1.0, 2.0, 3.0, 4.0));
	ASSERT (r1 != Rect (2.0, 3.0, 4.0, 5.0));

	ASSERT (r1.GetX () == 1.0);
	ASSERT (r1.GetY () == 2.0);
	ASSERT (r1.GetWidth () == 3.0);
	ASSERT (r1.GetHeight () == 4.0);

	ASSERT (r1.GetLeft () == 1.0);
	ASSERT (r1.GetRight () == 4.0);
	ASSERT (r1.GetTop () == 2.0);
	ASSERT (r1.GetBottom () == 6.0);

	ASSERT (r1.GetTopLeft () == Point (1.0, 2.0));
	ASSERT (r1.GetTopRight () == Point (4.0, 2.0));
	ASSERT (r1.GetBottomLeft () == Point (1.0, 6.0));
	ASSERT (r1.GetBottomRight () == Point (4.0, 6.0));

	ASSERT (r1.GetCenter () == Point (2.5, 4.0));
	ASSERT (r1.GetLeftCenter () == Point (1.0, 4.0));
	ASSERT (r1.GetRightCenter () == Point (4.0, 4.0));
	ASSERT (r1.GetTopCenter () == Point (2.5, 2.0));
	ASSERT (r1.GetBottomCenter () == Point (2.5, 6.0));

	ASSERT (r1.GetSize () == Size (3.0, 4.0));
	ASSERT (r1.Contains (Point (2.0, 4.0)));
	ASSERT (!r1.Contains (Point (0.0, 4.0)));
	ASSERT (!r1.Contains (Point (2.0, 1.0)));
	ASSERT (!r1.Contains (Point (6.0, 4.0)));
	ASSERT (!r1.Contains (Point (2.0, 7.0)));

	ASSERT (IsEqual (r1, Rect::FromPositionAndSize (Point (1.0, 2.0), Size (3.0, 4.0))));
	ASSERT (IsEqual (r1, Rect::FromCenterAndSize (Point (2.5, 4.0), Size (3.0, 4.0))));

	ASSERT (IsEqual (r1.Offset (Point (5.0, 6.0)), Rect::FromPositionAndSize (Point (6.0, 8.0), Size (3.0, 4.0))));
	ASSERT (IsEqual (r1.Expand (Size (5.0, 6.0)), Rect::FromCenterAndSize (r1.GetCenter (), r1.GetSize () + Size (5.0, 6.0))));
}

TEST (BoundingRectTest_FromRects)
{
	BoundingRect boundingRect;
	ASSERT (!boundingRect.IsValid ());

	Rect rect1 = Rect::FromPositionAndSize (Point (0, 0), Size (100, 100));
	Rect rect2 = Rect::FromPositionAndSize (Point (10, 10), Size (50, 50));
	Rect rect3 = Rect::FromPositionAndSize (Point (-10, -10), Size (20, 20));

	boundingRect.AddRect (rect1);
	ASSERT (boundingRect.IsValid ());
	ASSERT (IsEqual (boundingRect.GetRect (), rect1));

	boundingRect.AddRect (rect2);
	ASSERT (boundingRect.IsValid ());
	ASSERT (IsEqual (boundingRect.GetRect (), rect1));

	boundingRect.AddRect (rect3);
	ASSERT (boundingRect.IsValid ());
	ASSERT (IsEqual (boundingRect.GetRect (), Rect::FromPositionAndSize (Point (-10, -10), Size (110, 110))));
}

TEST (BoundingRectTest_FromPoints)
{
	BoundingRect boundingRect;
	ASSERT (!boundingRect.IsValid ());

	boundingRect.AddPoint (Point (10, 10));
	ASSERT (boundingRect.IsValid ());
	ASSERT (IsEqual (boundingRect.GetRect (), Rect::FromPositionAndSize (Point (10, 10), Size (0, 0))));

	boundingRect.AddPoint (Point (20, 20));
	ASSERT (boundingRect.IsValid ());
	ASSERT (IsEqual (boundingRect.GetRect (), Rect::FromPositionAndSize (Point (10, 10), Size (10, 10))));

	boundingRect.AddPoint (Point (-10, -10));
	ASSERT (boundingRect.IsValid ());
	ASSERT (IsEqual (boundingRect.GetRect (), Rect::FromPositionAndSize (Point (-10, -10), Size (30, 30))));
}

TEST (BoundingRectTest_FromRectAndPoint)
{
	{
		BoundingRect boundingRect;
		ASSERT (!boundingRect.IsValid ());

		boundingRect.AddPoint (Point (-10, -10));
		boundingRect.AddRect (Rect::FromPositionAndSize (Point (0, 0), Size (20, 20)));
		ASSERT (boundingRect.IsValid ());
		ASSERT (IsEqual (boundingRect.GetRect (), Rect::FromPositionAndSize (Point (-10, -10), Size (30, 30))));
	}

	{
		BoundingRect boundingRect;
		ASSERT (!boundingRect.IsValid ());

		boundingRect.AddRect (Rect::FromPositionAndSize (Point (0, 0), Size (20, 20)));
		boundingRect.AddPoint (Point (-10, -10));
		ASSERT (boundingRect.IsValid ());
		ASSERT (IsEqual (boundingRect.GetRect (), Rect::FromPositionAndSize (Point (-10, -10), Size (30, 30))));
	}
}

}
