#ifndef TESTAPPVALUES_HPP
#define TESTAPPVALUES_HPP

#include "SingleValues.hpp"
#include "Geometry.hpp"
#include "Drawing.hpp"
#include "DrawingImage.hpp"

class Color
{
public:
	Color ();
	Color (unsigned char r, unsigned char g, unsigned char b);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Point
{
public:
	Point ();
	Point (double x, double y);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	double	x;
	double	y;
};

class Line
{
public:
	Line ();
	Line (const Point& beg, const Point& end, const Color& color);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Point	beg;
	Point	end;
	Color	color;
};

class Circle
{
public:
	Circle ();
	Circle (const Point& center, double radius, const Color& color);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Point	center;
	double	radius;
	Color	color;
};

template <class Type>
class GeometricValue : public NE::GenericValue<Type>
{
public:
	GeometricValue (const Type& val) :
		NE::GenericValue<Type> (val)
	{
	
	}

	virtual std::wstring ToString () const override
	{
		return val.ToString ();
	}
};

class ColorValue : public NE::GenericValue<Color>
{
	DYNAMIC_SERIALIZABLE (ColorValue);

public:
	ColorValue ();
	ColorValue (const Color& val);

	virtual std::wstring		ToString () const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class PointValue : public GeometricValue<Point>
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const Point& val);

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class LineValue : public GeometricValue<Line>
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const Line& val);

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
}; 

class CircleValue : public GeometricValue<Circle>
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const Circle& val);

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
}; 

#endif
