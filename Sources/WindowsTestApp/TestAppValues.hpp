#ifndef TESTAPPVALUES_HPP
#define TESTAPPVALUES_HPP

#include "SingleValues.hpp"
#include "Geometry.hpp"

class Color
{
public:
	Color ();
	Color (unsigned char r, unsigned char g, unsigned char b);

	std::wstring ToString () const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Line
{
public:
	Line ();
	Line (NUIE::Point beg, NUIE::Point end, Color color);

	std::wstring ToString () const;

	NUIE::Point		beg;
	NUIE::Point		end;
	Color			color;
};

class Circle
{
public:
	Circle ();
	Circle (NUIE::Point center, double radius, Color color);

	std::wstring ToString () const;

	NUIE::Point		center;
	double			radius;
	Color			color;
};

class ColorValue : public NE::GenericValue<Color>
{
	DYNAMIC_SERIALIZABLE (ColorValue);

public:
	ColorValue ();
	ColorValue (const Color& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class PointValue : public NE::GenericValue<NUIE::Point>
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const NUIE::Point& val);
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
