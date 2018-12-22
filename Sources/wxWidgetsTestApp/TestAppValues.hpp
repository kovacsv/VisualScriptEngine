#ifndef TESTAPPVALUES_HPP
#define TESTAPPVALUES_HPP

#include "NE_SingleValues.hpp"
#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"
#include "NUIE_DrawingImage.hpp"

class Color
{
public:
	Color ();
	Color (unsigned char r, unsigned char g, unsigned char b);

	std::wstring		ToString (const NE::StringSettings& stringSettings) const;
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
	Transformation (double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33);
	~Transformation ();

	std::wstring			ToString (const NE::StringSettings& stringSettings) const;
	NE::Stream::Status		Read (NE::InputStream& inputStream);
	NE::Stream::Status		Write (NE::OutputStream& outputStream) const;

	void					Apply (double& x, double& y) const;

	static Transformation	Translation (double x, double y);

private:
	double matrix[9];
};

class Point
{
public:
	Point ();
	Point (double x, double y);

	std::wstring		ToString (const NE::StringSettings& stringSettings) const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Point				Transform (const Transformation& transformation) const;

	double	x;
	double	y;
};

class Line
{
public:
	Line ();
	Line (const Point& beg, const Point& end, const Color& color);

	std::wstring		ToString (const NE::StringSettings& stringSettings) const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Line				Transform (const Transformation& transformation) const;

	Point	beg;
	Point	end;
	Color	color;
};

class Circle
{
public:
	Circle ();
	Circle (const Point& center, double radius, const Color& color);

	std::wstring		ToString (const NE::StringSettings& stringSettings) const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Circle				Transform (const Transformation& transformation) const;

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
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class GeometricValue
{
public:
	GeometricValue ();
	virtual ~GeometricValue ();

	virtual NE::ValuePtr Transform (const Transformation& transformation) const = 0;
};

class DrawableValue
{
public:
	DrawableValue ();
	virtual ~DrawableValue ();

	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const = 0;
};

class PointValue :	public NE::GenericValue<Point>,
					public GeometricValue,
					public DrawableValue
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const Point& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform (const Transformation& transformation) const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
};

class LineValue :	public NE::GenericValue<Line>,
					public GeometricValue,
					public DrawableValue
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const Line& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform (const Transformation& transformation) const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
}; 

class CircleValue : public NE::GenericValue<Circle>,
					public GeometricValue,
					public DrawableValue
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const Circle& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString (const NE::StringSettings& stringSettings) const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform (const Transformation& transformation) const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
}; 

#endif
