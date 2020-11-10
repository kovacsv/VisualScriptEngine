#ifndef NUIE_GEOMETRY_HPP
#define NUIE_GEOMETRY_HPP

#include "NE_Stream.hpp"

#include <vector>

namespace NUIE
{

extern const double INF;
extern const double EPS;

class Point
{
public:
	Point ();
	Point (double x, double y);

	void	Set (double newX, double newY);
	void	SetX (double newX);
	void	SetY (double newY);

	double	GetX () const;
	double	GetY () const;

	Point	operator+ (const Point& p) const;
	Point	operator- () const;
	Point	operator- (const Point& p) const;
	Point	operator* (double d) const;
	Point	operator/ (double d) const;
	
	bool	operator== (const Point& p) const;
	bool	operator!= (const Point& p) const;

	static double	Distance (const Point& a, const Point& b);
	static Point	GetMidPoint (const Point& a, const Point& b);

private:
	double x;
	double y;
};

class Size
{
public:
	Size ();
	Size (double width, double height);

	double	GetWidth () const;
	double	GetHeight () const;

	void	SetWidth (double newWidth);
	void	SetHeight (double newHeight);

	Size	Grow (double vGrow, double hGrow) const;

	Size	operator+ (const Size& s) const;
	Size	operator- (const Size& s) const;
	Size	operator* (double d) const;
	Size	operator/ (double d) const;

	bool	operator== (const Size& s) const;
	bool	operator!= (const Size& s) const;

private:
	double width;
	double height;
};

class Rect
{
public:
	Rect ();
	Rect (double x, double y, double width, double height);

	double	GetX () const;
	double	GetY () const;
	double	GetWidth () const;
	double	GetHeight () const;

	double	GetLeft () const;
	double	GetRight () const;
	double	GetTop () const;
	double	GetBottom () const;

	Point	GetCenter () const;
	Point	GetLeftCenter () const;
	Point	GetRightCenter () const;
	Point	GetTopCenter () const;
	Point	GetBottomCenter () const;

	Point	GetTopLeft () const;
	Point	GetTopRight () const;
	Point	GetBottomLeft () const;
	Point	GetBottomRight () const;

	Point	GetPosition () const;
	Size	GetSize () const;
	bool	Contains (const Point& p) const;
	bool	Contains (const Rect& r) const;

	Rect	Offset (const Point& p) const;
	Rect	Expand (const Size& s) const;

	bool	operator== (const Rect& r) const;
	bool	operator!= (const Rect& r) const;

	static Rect FromPositionAndSize (const Point& rectPosition, const Size& rectSize);
	static Rect FromCenterAndSize (const Point& rectCenter, const Size& rectSize);
	static Rect FromTwoPoints (const Point& p1, const Point& p2);
	static bool IsInBounds (const Rect& rect, double boundsWidth, double boundsHeight);

private:
	Point	position;
	Size	size;
};

class IntPoint
{
public:
	IntPoint ();
	IntPoint (int x, int y);
	IntPoint (const Point& point);

	int		GetX () const;
	int		GetY () const;

private:
	int x;
	int y;
};

class IntRect
{
public:
	IntRect ();
	IntRect (int x, int y, int width, int height);
	IntRect (const Rect& rect);

	int		GetX () const;
	int		GetY () const;
	int		GetWidth () const;
	int		GetHeight () const;

private:
	int x;
	int y;
	int width;
	int height;
};

class BoundingRect
{
public:
	BoundingRect ();

	void			AddPoint (const Point& point);
	void			AddRect (const Rect& rect);

	bool			IsValid () const;
	const Rect&		GetRect () const;

private:
	Rect	boundingRect;
	bool	isValid;
};

NE::Stream::Status ReadPoint (NE::InputStream& inputStream, Point& point);
NE::Stream::Status ReadSize (NE::InputStream& inputStream, Size& size);
NE::Stream::Status ReadRect (NE::InputStream& inputStream, Rect& rect);

NE::Stream::Status WritePoint (NE::OutputStream& outputStream, const Point& point);
NE::Stream::Status WriteSize (NE::OutputStream& outputStream, const Size& size);
NE::Stream::Status WriteRect (NE::OutputStream& outputStream, const Rect& rect);

bool IsEqual (double a, double b);
bool IsLower (double a, double b);
bool IsGreater (double a, double b);

bool IsEqual (const Point& a, const Point& b);
bool IsEqual (const Size& a, const Size& b);
bool IsEqual (const Rect& a, const Rect& b);

std::vector<Point>	SegmentBezier (size_t segmentCount, const Point& p1, const Point& p2, const Point& p3, const Point& p4);
Rect				GetBezierBoundingRect (const Point& p1, const Point& p2, const Point& p3, const Point& p4);

}

#endif
