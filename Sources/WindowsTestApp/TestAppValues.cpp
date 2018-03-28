#include "TestAppValues.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "StringUtils.hpp"

NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{D10E20B6-856A-4AAC-A806-FC60E6D1E82F}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineValue::serializationInfo (NE::ObjectId ("{E899A12E-F87F-4B6B-ACD8-5C86573C382F}"), NE::ObjectVersion (1), LineValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleValue::serializationInfo (NE::ObjectId ("{82190020-867B-4260-94BA-49D8FE94418E}"), NE::ObjectVersion (1), CircleValue::CreateSerializableInstance);

PointValue::PointValue () :
	PointValue (BIN::Point ())
{

}

PointValue::PointValue (const BIN::Point& val) :
	NE::GenericValue<BIN::Point> (val)
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
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr PointValue::Transform () const
{
	return NE::ValuePtr (new PointValue (BIN::Point (val.x + 10, val.y + 10)));
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
	LineValue (BIN::Line ())
{

}

LineValue::LineValue (const BIN::Line& val) :
	NE::GenericValue<BIN::Line> (val)
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
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr LineValue::Transform () const
{
	return nullptr;
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
	CircleValue (BIN::Circle ())
{

}

CircleValue::CircleValue (const BIN::Circle& val) :
	NE::GenericValue<BIN::Circle> (val)
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
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr CircleValue::Transform () const
{
	return nullptr;
}

NUIE::DrawingItemConstPtr CircleValue::CreateDrawingItem () const
{
	NUIE::Rect rect = NUIE::Rect::FromCenterAndSize (NUIE::Point (val.center.x, val.center.y), NUIE::Size (val.radius * 2.0, val.radius * 2.0));
	return NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (rect, NUIE::Pen (NUIE::Color (val.color.r, val.color.g, val.color.b), 1.0)));
}
