#include "TestAppValues.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "StringUtils.hpp"

NE::DynamicSerializationInfo	ColorValue::serializationInfo (NE::ObjectId ("{E6D2DBDC-6311-4BA5-9B1A-A0FFF8CA2444}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{D10E20B6-856A-4AAC-A806-FC60E6D1E82F}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineValue::serializationInfo (NE::ObjectId ("{E899A12E-F87F-4B6B-ACD8-5C86573C382F}"), NE::ObjectVersion (1), LineValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleValue::serializationInfo (NE::ObjectId ("{82190020-867B-4260-94BA-49D8FE94418E}"), NE::ObjectVersion (1), CircleValue::CreateSerializableInstance);

static std::wstring ColorToString (const NUIE::Color& color)
{
	std::wstring result = L"";
	result += L"Color (";
	result += std::to_wstring (color.GetR ());
	result += L", ";
	result += std::to_wstring (color.GetG ());
	result += L", ";
	result += std::to_wstring (color.GetB ());
	result += L")";
	return result;
}

Point::Point () :
	Point (0.0, 0.0)
{

}

Point::Point (double x, double y) :
	x (x),
	y (y)
{

}

std::wstring Point::ToString () const
{
	std::wstring result = L"";
	result += L"Point (";
	result += NE::DoubleToString (x, 2);
	result += L", ";
	result += NE::DoubleToString (y, 2);
	result += L")";
	return result;
}

NE::Stream::Status Point::Read (NE::InputStream& inputStream)
{
	inputStream.Read (x);
	inputStream.Read (y);
	return inputStream.GetStatus ();
}

NE::Stream::Status Point::Write (NE::OutputStream& outputStream) const
{
	outputStream.Write (x);
	outputStream.Write (y);
	return outputStream.GetStatus ();
}

Line::Line () :
	Line (Point (), Point (), NUIE::Color ())
{

}

Line::Line (const Point& beg, const Point& end, const NUIE::Color& color) :
	beg (beg),
	end (end),
	color (color)
{

}

std::wstring Line::ToString () const
{
	std::wstring result = L"";
	result += L"Line (";
	result += beg.ToString ();
	result += L" - ";
	result += end.ToString ();
	result += L")";
	return result;
}

NE::Stream::Status Line::Read (NE::InputStream& inputStream)
{
	beg.Read (inputStream);
	end.Read (inputStream);
	ReadColor (inputStream, color);
	return inputStream.GetStatus ();
}

NE::Stream::Status Line::Write (NE::OutputStream& outputStream) const
{
	beg.Write (outputStream);
	end.Write (outputStream);
	WriteColor (outputStream, color);
	return outputStream.GetStatus ();
}

Circle::Circle () :
	Circle (Point (), 0, NUIE::Color ())
{

}

Circle::Circle (const Point& center, double radius, const NUIE::Color& color) :
	center (center),
	radius (radius),
	color (color)
{

}

std::wstring Circle::ToString () const
{
	std::wstring result = L"";
	result += L"Circle (";
	result += center.ToString ();
	result += L", ";
	result += NE::DoubleToString (radius, 23);
	result += L")";
	return result;
}

NE::Stream::Status Circle::Read (NE::InputStream& inputStream)
{
	center.Read (inputStream);
	inputStream.Read (radius);
	ReadColor (inputStream, color);
	return inputStream.GetStatus ();
}

NE::Stream::Status Circle::Write (NE::OutputStream& outputStream) const
{
	center.Write (outputStream);
	outputStream.Write (radius);
	WriteColor (outputStream, color);
	return outputStream.GetStatus ();
}

ColorValue::ColorValue () :
	ColorValue (NUIE::Color ())
{

}

ColorValue::ColorValue (const NUIE::Color& val) :
	NE::GenericValue<NUIE::Color> (val)
{

}

std::wstring ColorValue::ToString () const
{
	return ColorToString (GetValue ());
}

NE::Stream::Status ColorValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::ReadColor (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::WriteColor (outputStream, val);
	return outputStream.GetStatus ();
}

PointValue::PointValue () :
	PointValue (Point ())
{

}

PointValue::PointValue (const Point& val) :
	NE::GenericValue<Point> (val)
{

}

std::wstring PointValue::ToString () const
{
	return GetValue ().ToString ();
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

LineValue::LineValue () :
	NE::GenericValue<Line> (Line ())
{

}

LineValue::LineValue (const Line& val) :
	NE::GenericValue<Line> (val)
{

}

std::wstring LineValue::ToString () const
{
	return GetValue ().ToString ();
}

NE::Stream::Status LineValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	val.beg.Read (inputStream);
	val.end.Read (inputStream);
	ReadColor (inputStream, val.color);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	val.beg.Write (outputStream);
	val.end.Write (outputStream);
	WriteColor (outputStream, val.color);
	return outputStream.GetStatus ();
}

CircleValue::CircleValue () :
	NE::GenericValue<Circle> (Circle ())
{

}

CircleValue::CircleValue (const Circle& val) :
	NE::GenericValue<Circle> (val)
{

}

std::wstring CircleValue::ToString () const
{
	return GetValue ().ToString ();
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
