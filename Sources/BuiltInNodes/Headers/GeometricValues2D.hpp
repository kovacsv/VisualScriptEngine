#ifndef GEOMETRICVALUES2D_HPP
#define GEOMETRICVALUES2D_HPP

#include "Stream.hpp"
#include "GenericValue.hpp"
#include <string>

namespace BI
{

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

class Transformation
{
public:
	Transformation ();
	~Transformation ();

	std::wstring			ToString () const;
	NE::Stream::Status		Read (NE::InputStream& inputStream);
	NE::Stream::Status		Write (NE::OutputStream& outputStream) const;
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

class ColorValue : public NE::GenericValue<Color>
{
	DYNAMIC_SERIALIZABLE (ColorValue);

public:
	ColorValue ();
	ColorValue (const Color& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TransformationValue : public NE::GenericValue<Transformation>
{
	DYNAMIC_SERIALIZABLE (TransformationValue);

public:
	TransformationValue ();
	TransformationValue (const Transformation& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class GeometricValue
{
public:
	virtual NE::ValuePtr		Transform () const = 0;
};

class PointValue :	public NE::GenericValue<BI::Point>,
					public GeometricValue
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const BI::Point& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr		Transform () const override;
};

class LineValue :	public NE::GenericValue<BI::Line>,
					public GeometricValue
					
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const BI::Line& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr		Transform () const override;
}; 

class CircleValue : public NE::GenericValue<BI::Circle>,
					public GeometricValue					
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const BI::Circle& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr		Transform () const override;
}; 

}

#endif
