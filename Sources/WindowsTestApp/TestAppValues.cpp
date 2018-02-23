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

static std::wstring PointToString (const NUIE::Point& point)
{
	std::wstring result = L"";
	result += L"Point (";
	result += NE::DoubleToString (point.GetX (), 2);
	result += L", ";
	result += NE::DoubleToString (point.GetY (), 2);
	result += L")";
	return result;
}

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

Line::Line () :
	Line (NUIE::Point (), NUIE::Point (), NUIE::Color ())
{

}

Line::Line (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Color& color) :
	beg (beg),
	end (end),
	color (color)
{

}

std::wstring Line::ToString () const
{
	std::wstring result = L"";
	result += L"Line (";
	result += PointToString (beg);
	result += L" - ";
	result += PointToString (end);
	result += L")";
	return result;
}

Circle::Circle () :
	Circle (NUIE::Point (), 0, NUIE::Color ())
{

}

Circle::Circle (const NUIE::Point& center, double radius, const NUIE::Color& color) :
	center (center),
	radius (radius),
	color (color)
{

}

std::wstring Circle::ToString () const
{
	std::wstring result = L"";
	result += L"Circle (";
	result += PointToString (center);
	result += L", ";
	result += NE::DoubleToString (radius, 23);
	result += L")";
	return result;
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
	NE::GenericValue<NUIE::Point> (NUIE::Point ())
{

}

PointValue::PointValue (const NUIE::Point& val) :
	NE::GenericValue<NUIE::Point> (val)
{

}

std::wstring PointValue::ToString () const
{
	const NUIE::Point& point = GetValue ();
	return PointToString (point);
}

NE::Stream::Status PointValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ReadPoint (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	WritePoint (outputStream, val);
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
	ReadPoint (inputStream, val.beg);
	ReadPoint (inputStream, val.end);
	ReadColor (inputStream, val.color);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	WritePoint (outputStream, val.beg);
	WritePoint (outputStream, val.end);
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
	ReadPoint (inputStream, val.center);
	inputStream.Read (val.radius);
	ReadColor (inputStream, val.color);
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	WritePoint (outputStream, val.center);
	outputStream.Write (val.radius);
	WriteColor (outputStream, val.color);
	return outputStream.GetStatus ();
}
