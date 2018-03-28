#ifndef TESTAPPVALUES_HPP
#define TESTAPPVALUES_HPP

#include "SingleValues.hpp"
#include "Geometry.hpp"
#include "Drawing.hpp"
#include "DrawingImage.hpp"
#include "GeometricValues2D.hpp"

class GeometricValue
{
public:
	virtual NE::ValuePtr				Transform () const = 0;
};

class DrawableValue
{
public:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const = 0;
};

class PointValue :	public GeometricValue,
					public DrawableValue,
					public NE::GenericValue<BI::Point>
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const BI::Point& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform () const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
};

class LineValue :	public GeometricValue,
					public DrawableValue,
					public NE::GenericValue<BI::Line>
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const BI::Line& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform () const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
}; 

class CircleValue : public GeometricValue,
					public DrawableValue,
					public NE::GenericValue<BI::Circle>
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const BI::Circle& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform () const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
}; 

#endif
