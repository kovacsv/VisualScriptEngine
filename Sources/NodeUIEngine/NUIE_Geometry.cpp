#include "NUIE_Geometry.hpp"
#include <cmath>
#include <algorithm>

namespace NUIE
{

const double INF = 10.0e10;
const double EPS = 10e-5;

Point::Point () :
	x (0.0),
	y (0.0)
{

}

Point::Point (double x, double y) :
	x (x),
	y (y)
{

}

void Point::Set (double newX, double newY)
{
	x = newX;
	y = newY;
}

void Point::SetX (double newX)
{
	x = newX;
}

void Point::SetY (double newY)
{
	y = newY;
}

double Point::GetX () const
{
	return x;
}

double Point::GetY () const
{
	return y;
}

Point Point::operator+ (const Point& p) const
{
	return Point (x + p.x, y + p.y);
}

Point Point::operator- () const
{
	return Point (-x, -y);
}

Point Point::operator- (const Point& p) const
{
	return Point (x - p.x, y - p.y);
}

Point Point::operator* (double d) const
{
	return Point (x * d, y * d);
}

Point Point::operator/ (double d) const
{
	return Point (x / d, y / d);
}

bool Point::operator== (const Point& p) const
{
	return x == p.x && y == p.y;
}

bool Point::operator!= (const Point& p) const
{
	return !operator== (p);
}

double Point::Distance (const Point& a, const Point& b)
{
	return sqrt ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

Point Point::GetMidPoint (const Point& a, const Point& b)
{
	return Point ((a.GetX () + b.GetX ()) / 2.0, (a.GetY () + b.GetY ()) / 2.0);
}

Size::Size () :
	width (0.0),
	height (0.0)
{

}

Size::Size (double width, double height) :
	width (width),
	height (height)
{

}

double Size::GetWidth () const
{
	return width;
}

double Size::GetHeight () const
{
	return height;
}

void Size::SetWidth (double newWidth)
{
	width = newWidth;
}

void Size::SetHeight (double newHeight)
{
	height = newHeight;
}

Size Size::Grow (double vGrow, double hGrow) const
{
	return Size (width + vGrow, height + hGrow);
}

Size Size::operator+ (const Size& s) const
{
	return Size (width + s.width, height + s.height);
}

Size Size::operator- (const Size& s) const
{
	return Size (width - s.width, height - s.height);
}

Size Size::operator* (double d) const
{
	return Size (width * d, height * d);
}

Size Size::operator/ (double d) const
{
	return Size (width / d, height / d);
}

bool Size::operator== (const Size& s) const
{
	return width == s.width && height == s.height;
}

bool Size::operator!= (const Size& s) const
{
	return !operator== (s);
}

Rect::Rect () :
	position (),
	size ()
{

}

Rect::Rect (double x, double y, double width, double height) :
	position (x, y),
	size (width, height)
{

}

double Rect::GetX () const
{
	return position.GetX ();
}

double Rect::GetY () const
{
	return position.GetY ();
}

double Rect::GetWidth () const
{
	return size.GetWidth ();
}

double Rect::GetHeight () const
{
	return size.GetHeight ();
}

double Rect::GetLeft () const
{
	return position.GetX ();
}

double Rect::GetRight () const
{
	return position.GetX () + size.GetWidth ();
}

double Rect::GetTop () const
{
	return position.GetY ();
}

double Rect::GetBottom () const
{
	return position.GetY () + size.GetHeight ();
}

Point Rect::GetCenter () const
{
	return Point (position.GetX () + size.GetWidth () / 2.0, position.GetY () + size.GetHeight () / 2.0);
}

Point Rect::GetLeftCenter() const
{
	return Point::GetMidPoint (GetTopLeft (), GetBottomLeft ());
}

Point Rect::GetRightCenter() const
{
	return Point::GetMidPoint (GetTopRight (), GetBottomRight ());
}

Point Rect::GetTopCenter() const
{
	return Point::GetMidPoint (GetTopLeft (), GetTopRight ());
}

Point Rect::GetBottomCenter() const
{
	return Point::GetMidPoint (GetBottomLeft (), GetBottomRight ());
}

Point Rect::GetTopLeft() const
{
	return position;
}

Point Rect::GetTopRight() const
{
	return Point (position.GetX () + size.GetWidth (), position.GetY ());
}

Point Rect::GetBottomLeft() const
{
	return Point (position.GetX (), position.GetY () + size.GetHeight ());
}

Point Rect::GetBottomRight() const
{
	return Point (position.GetX () + size.GetWidth (), position.GetY () + size.GetHeight ());
}

Point Rect::GetPosition () const
{
	return position;
}

Size Rect::GetSize () const
{
	return size;
}

bool Rect::Contains (const Point& p) const
{
	return p.GetX () >= GetLeft () && p.GetX () <= GetRight () && p.GetY () >= GetTop () && p.GetY () <= GetBottom ();
}

bool Rect::Contains (const Rect& r) const
{
	return r.GetLeft () >= GetLeft () && r.GetRight () <= GetRight () && r.GetTop () >= GetTop () && r.GetBottom () <= GetBottom ();
}

Rect Rect::Offset (const Point& p) const
{
	return Rect::FromPositionAndSize (position + p, size);
}

Rect Rect::Expand (const Size& s) const
{
	Point sp (s.GetWidth (), s.GetHeight ());
	return Rect::FromPositionAndSize (position - sp / 2.0, size + s);
}

bool Rect::operator== (const Rect& r) const
{
	return position == r.position && size == r.size;
}

bool Rect::operator!= (const Rect& r) const
{
	return !operator== (r);
}

Rect Rect::FromPositionAndSize (const Point& rectPosition, const Size& rectSize)
{
	return Rect (rectPosition.GetX (), rectPosition.GetY (), rectSize.GetWidth (), rectSize.GetHeight ());
}

Rect Rect::FromCenterAndSize (const Point& rectCenter, const Size& rectSize)
{
	Point position (rectCenter.GetX () - rectSize.GetWidth () / 2.0, rectCenter.GetY () - rectSize.GetHeight () / 2.0);
	return Rect::FromPositionAndSize (position, rectSize);
}

Rect Rect::FromTwoPoints (const Point& p1, const Point& p2)
{
	Point rectPosition (std::min (p1.GetX (), p2.GetX ()), std::min (p1.GetY (), p2.GetY ()));
	Size rectSize (std::fabs (p2.GetX () - p1.GetX ()), std::fabs (p2.GetY () - p1.GetY ()));
	return Rect::FromPositionAndSize (rectPosition, rectSize);
}

bool Rect::IsInBounds (const Rect& rect, double boundsWidth, double boundsHeight)
{
	if (rect.GetRight () < 0 || rect.GetBottom () < 0) {
		return false;
	}
	if (rect.GetLeft () > boundsWidth || rect.GetTop () > boundsHeight) {
		return false;
	}
	return true;
}

IntPoint::IntPoint () :
	x (0),
	y (0)
{

}

IntPoint::IntPoint (int x, int y) :
	x (x),
	y (y)
{

}

IntPoint::IntPoint (const Point& point) :
	x ((int) std::floor (point.GetX ()) - 1),
	y ((int) std::floor (point.GetY ()) - 1)
{

}

int IntPoint::GetX () const
{
	return x;
}

int IntPoint::GetY () const
{
	return y;
}

IntRect::IntRect () :
	x (0),
	y (0),
	width (0),
	height (0)
{

}

IntRect::IntRect (int x, int y, int width, int height) :
	x (x),
	y (y),
	width (width),
	height (height)
{

}

IntRect::IntRect (const Rect& rect) :
	x ((int) std::floor (rect.GetLeft ()) - 1),
	y ((int) std::floor (rect.GetTop ()) - 1),
	width ((int) std::floor (rect.GetRight ()) - x),
	height ((int) std::floor (rect.GetBottom ()) - y)
{

}

int IntRect::GetX () const
{
	return x;
}

int IntRect::GetY () const
{
	return y;
}

int IntRect::GetWidth () const
{
	return width;
}

int IntRect::GetHeight () const
{
	return height;
}

BoundingRectCalculator::BoundingRectCalculator () :
	boundingRect (),
	isValid (false)
{
}

void BoundingRectCalculator::AddRect (const Rect& rect)
{
	if (!isValid) {
		boundingRect = rect;
		isValid = true;
	} else {
		boundingRect = Rect::FromTwoPoints (
			Point (std::min (rect.GetLeft (), boundingRect.GetLeft ()), std::min (rect.GetTop (), boundingRect.GetTop ())),
			Point (std::max (rect.GetRight (), boundingRect.GetRight ()), std::max (rect.GetBottom (), boundingRect.GetBottom ()))
		);
	}
}

bool BoundingRectCalculator::IsValid () const
{
	return isValid;
}

const Rect& BoundingRectCalculator::GetRect () const
{
	DBGASSERT (isValid);
	return boundingRect;
}

NE::Stream::Status ReadPoint (NE::InputStream& inputStream, Point& point)
{
	double x = 0.0;
	double y = 0.0;
	inputStream.Read (x);
	inputStream.Read (y);
	point.Set (x, y);
	return inputStream.GetStatus ();
}

NE::Stream::Status ReadSize (NE::InputStream& inputStream, Size& size)
{
	double width = 0.0;
	double height = 0.0;
	inputStream.Read (width);
	inputStream.Read (height);
	size.SetWidth (width);
	size.SetHeight (height);
	return inputStream.GetStatus ();
}

NE::Stream::Status ReadRect (NE::InputStream& inputStream, Rect& rect)
{
	Point position;
	Size size;
	ReadPoint (inputStream, position);
	ReadSize (inputStream, size);
	rect = Rect::FromPositionAndSize (position, size);
	return inputStream.GetStatus ();
}

NE::Stream::Status WritePoint (NE::OutputStream& outputStream, const Point& point)
{
	outputStream.Write (point.GetX ());
	outputStream.Write (point.GetY ());
	return outputStream.GetStatus ();
}

NE::Stream::Status WriteSize (NE::OutputStream& outputStream, const Size& size)
{
	outputStream.Write (size.GetWidth ());
	outputStream.Write (size.GetHeight ());
	return outputStream.GetStatus ();
}

NE::Stream::Status WriteRect (NE::OutputStream& outputStream, const Rect& rect)
{
	WritePoint (outputStream, rect.GetTopLeft ());
	WriteSize (outputStream, rect.GetSize ());
	return outputStream.GetStatus ();
}

void AddPointToChecksum (NE::Checksum& checksum, const Point& point)
{
	checksum.Add (point.GetX ());
	checksum.Add (point.GetY ());
}

void AddSizeToChecksum (NE::Checksum& checksum, const Size& size)
{
	checksum.Add (size.GetWidth ());
	checksum.Add (size.GetHeight ());
}

void AddRectToChecksum (NE::Checksum& checksum, const Rect& rect)
{
	AddPointToChecksum (checksum, rect.GetTopLeft ());
	AddSizeToChecksum (checksum, rect.GetSize ());
}

bool IsEqual (double a, double b)
{
	return std::fabs (a - b) < EPS;
}

bool IsLower (double a, double b)
{
	return b - a > EPS;
}

bool IsGreater (double a, double b)
{
	return a - b > EPS;
}

bool IsEqual (const Point& a, const Point& b)
{
	return IsEqual (a.GetX (), b.GetX ()) && IsEqual (a.GetY (), b.GetY ());
}

bool IsEqual (const Size& a, const Size& b)
{
	return IsEqual (a.GetWidth (), b.GetWidth ()) && IsEqual (a.GetHeight (), b.GetHeight ());
}

bool IsEqual (const Rect& a, const Rect& b)
{
	return IsEqual (a.GetPosition (), b.GetPosition ()) && IsEqual (a.GetSize (), b.GetSize ());
}

std::vector<NUIE::Point> SegmentBezier (size_t segmentCount, const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4)
{
	std::vector<NUIE::Point> points;
	double tStep = 1.0 / segmentCount;
	for (size_t i = 0; i <= segmentCount; i++) {
		double t = i * tStep;
		double omt = 1.0 - t;
		NUIE::Point pt = p1 * std::pow (omt, 3) + p2 * (3.0 * std::pow (omt, 2) * t) + p3 * (3.0 * omt * std::pow (t, 2)) + p4 * std::pow (t, 3);
		points.push_back (pt);
	}
	return points;
}

}
