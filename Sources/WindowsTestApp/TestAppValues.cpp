#include "TestAppValues.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"

NE::DynamicSerializationInfo	ColorValue::serializationInfo (NE::ObjectId ("{E6D2DBDC-6311-4BA5-9B1A-A0FFF8CA2444}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{D10E20B6-856A-4AAC-A806-FC60E6D1E82F}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineValue::serializationInfo (NE::ObjectId ("{E899A12E-F87F-4B6B-ACD8-5C86573C382F}"), NE::ObjectVersion (1), LineValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleValue::serializationInfo (NE::ObjectId ("{82190020-867B-4260-94BA-49D8FE94418E}"), NE::ObjectVersion (1), CircleValue::CreateSerializableInstance);

static void ReadColor (NE::InputStream& inputStream, Color& color)
{
	inputStream.Read (color.r);
	inputStream.Read (color.g);
	inputStream.Read (color.b);
}

static void WriteColor (NE::OutputStream& outputStream, const Color& color)
{
	outputStream.Write (color.r);
	outputStream.Write (color.g);
	outputStream.Write (color.b);
}

static void ReadPoint (NE::InputStream& inputStream, Point& point)
{
	inputStream.Read (point.x);
	inputStream.Read (point.y);
	inputStream.Read (point.size);
	ReadColor (inputStream, point.color);
}

static void WritePoint (NE::OutputStream& outputStream, const Point& point)
{
	outputStream.Write (point.x);
	outputStream.Write (point.y);
	outputStream.Write (point.size);
	WriteColor (outputStream, point.color);
}

Color::Color () :
	Color (0, 0, 0)
{

}

Color::Color (unsigned char r, unsigned char g, unsigned char b) :
	r (r),
	g (g),
	b (b)
{

}

std::wstring Color::ToString () const
{
	std::wstring result = L"";
	result += L"Color (";
	result += std::to_wstring (r);
	result += L", ";
	result += std::to_wstring (g);
	result += L", ";
	result += std::to_wstring (b);
	result += L")";
	return result;
}

Point::Point () :
	Point (0, 0, 0, Color ())
{

}

Point::Point (int x, int y, int size, Color color) :
	x (x),
	y (y),
	size (size),
	color (color)
{

}

std::wstring Point::ToString () const
{
	std::wstring result = L"";
	result += L"Point (";
	result += std::to_wstring (x);
	result += L", ";
	result += std::to_wstring (y);
	result += L")";
	return result;
}

Line::Line () :
	Line (Point (), Point (), Color ())
{

}

Line::Line (Point beg, Point end, Color color) :
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

Circle::Circle () :
	Circle (Point (), 0, Color ())
{

}

Circle::Circle (Point center, int radius, Color color) :
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
	result += std::to_wstring (radius);
	result += L")";
	return result;
}

ColorValue::ColorValue () :
	NE::GenericValue<Color> (Color ())
{

}

ColorValue::ColorValue (const Color& val) :
	NE::GenericValue<Color> (val)
{

}

std::wstring ColorValue::ToString () const
{
	return GetValue ().ToString ();
}

NE::Stream::Status ColorValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	ReadColor (inputStream, val);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	WriteColor (outputStream, val);
	return outputStream.GetStatus ();
}

PointValue::PointValue () :
	NE::GenericValue<Point> (Point ())
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
