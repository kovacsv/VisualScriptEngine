#ifndef TESTAPPVALUES_HPP
#define TESTAPPVALUES_HPP

#include "SingleValues.hpp"
#include "Geometry.hpp"
#include "Drawing.hpp"

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
	Line (const Point& beg, const Point& end, const NUIE::Color& color);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Point			beg;
	Point			end;
	NUIE::Color		color;
};

class Circle
{
public:
	Circle ();
	Circle (const Point& center, double radius, const NUIE::Color& color);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Point			center;
	double			radius;
	NUIE::Color		color;
};

class ColorValue : public NE::GenericValue<NUIE::Color>
{
	DYNAMIC_SERIALIZABLE (ColorValue);

public:
	ColorValue ();
	ColorValue (const NUIE::Color& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class PointValue : public NE::GenericValue<Point>
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const Point& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class LineValue : public NE::GenericValue<Line>
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const Line& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
}; 

class CircleValue : public NE::GenericValue<Circle>
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const Circle& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
}; 

#endif
