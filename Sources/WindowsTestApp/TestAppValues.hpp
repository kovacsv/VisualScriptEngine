#ifndef TESTAPPVALUES_HPP
#define TESTAPPVALUES_HPP

#include "SingleValues.hpp"
#include "Geometry.hpp"
#include "Drawing.hpp"
#include "DrawingImage.hpp"
#include "GeometricNodes2D.hpp"

class DrawableValue
{
public:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const = 0;
};

class PointValue :	public NE::GenericValue<BI::Point>,
					public BI::GeometricValue,
					public DrawableValue
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const BI::Point& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform (const BI::Transformation& transformation) const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
};

class LineValue :	public NE::GenericValue<BI::Line>,
					public BI::GeometricValue,
					public DrawableValue
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const BI::Line& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform (const BI::Transformation& transformation) const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
}; 

class CircleValue : public NE::GenericValue<BI::Circle>,
					public BI::GeometricValue,
					public DrawableValue
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const BI::Circle& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	virtual NE::ValuePtr				Transform (const BI::Transformation& transformation) const override;
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem () const override;
}; 

#endif
