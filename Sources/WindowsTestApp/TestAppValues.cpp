#include "TestAppValues.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "StringUtils.hpp"

NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{2C242A9E-1054-4E16-82C1-759C006097C9}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineValue::serializationInfo (NE::ObjectId ("{2C860493-09EE-4EA2-8BC4-7FD8DE97BBC8}"), NE::ObjectVersion (1), LineValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleValue::serializationInfo (NE::ObjectId ("{88E71F51-D47D-4DE5-9182-D608E7E71741}"), NE::ObjectVersion (1), CircleValue::CreateSerializableInstance);

PointValue::PointValue () :
	PointValue (BI::Point ())
{

}

PointValue::PointValue (const BI::Point& val) :
	NE::GenericValue<BI::Point> (val)
{

}

NE::ValuePtr PointValue::Clone () const
{
	return NE::ValuePtr (new PointValue (val));
}

std::wstring PointValue::ToString () const
{
	return val.ToString ();
}

NE::Stream::Status PointValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<BI::Point>::Read (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<BI::Point>::Write (outputStream);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr PointValue::Transform (const BI::Transformation& transformation) const
{
	return NE::ValuePtr (new PointValue (val.Transform (transformation)));
}

NUIE::DrawingItemConstPtr PointValue::CreateDrawingItem () const
{
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	static const int pointSize = 10;
	NUIE::Rect pointRect (val.x - pointSize / 2, val.y - pointSize / 2, pointSize, pointSize);
	NUIE::Pen pointPen (NUIE::Color (80, 80, 80), 1.0);
	result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingLine (pointRect.GetTopLeft (), pointRect.GetBottomRight (), pointPen)));
	result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingLine (pointRect.GetBottomLeft (), pointRect.GetTopRight (), pointPen)));
	return result;
}

LineValue::LineValue () :
	LineValue (BI::Line ())
{

}

LineValue::LineValue (const BI::Line& val) :
	NE::GenericValue<BI::Line> (val)
{

}

NE::ValuePtr LineValue::Clone () const
{
	return NE::ValuePtr (new LineValue (val));
}

std::wstring LineValue::ToString () const
{
	return val.ToString ();
}

NE::Stream::Status LineValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<BI::Line>::Read (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<BI::Line>::Write (outputStream);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr LineValue::Transform (const BI::Transformation& transformation) const
{
	return NE::ValuePtr (new LineValue (val.Transform (transformation)));
}

NUIE::DrawingItemConstPtr LineValue::CreateDrawingItem () const
{
	return NUIE::DrawingItemConstPtr (
		new NUIE::DrawingLine (
			NUIE::Point (val.beg.x, val.beg.y),
			NUIE::Point (val.end.x, val.end.y),
			NUIE::Pen (NUIE::Color (val.color.r, val.color.g, val.color.b), 1.0)
		)
	);
}

CircleValue::CircleValue () :
	CircleValue (BI::Circle ())
{

}

CircleValue::CircleValue (const BI::Circle& val) :
	NE::GenericValue<BI::Circle> (val)
{

}

NE::ValuePtr CircleValue::Clone () const
{
	return NE::ValuePtr (new CircleValue (val));
}

std::wstring CircleValue::ToString () const
{
	return val.ToString ();
}

NE::Stream::Status CircleValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<BI::Circle>::Read (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<BI::Circle>::Write (outputStream);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr CircleValue::Transform (const BI::Transformation& transformation) const
{
	return NE::ValuePtr (new CircleValue (val.Transform (transformation)));
}

NUIE::DrawingItemConstPtr CircleValue::CreateDrawingItem () const
{
	NUIE::Rect rect = NUIE::Rect::FromCenterAndSize (NUIE::Point (val.center.x, val.center.y), NUIE::Size (val.radius * 2.0, val.radius * 2.0));
	return NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (rect, NUIE::Pen (NUIE::Color (val.color.r, val.color.g, val.color.b), 1.0)));
}
