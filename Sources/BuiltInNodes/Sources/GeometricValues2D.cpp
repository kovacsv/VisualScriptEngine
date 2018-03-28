#include "GeometricValues2D.hpp"
#include "StringUtils.hpp"

namespace BI
{

NE::DynamicSerializationInfo	ColorValue::serializationInfo (NE::ObjectId ("{E6D2DBDC-6311-4BA5-9B1A-A0FFF8CA2444}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformationValue::serializationInfo (NE::ObjectId ("{D90233EC-EFC9-4B18-BA45-DA1DB9C0EF63}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);

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

NE::Stream::Status Color::Read (NE::InputStream& inputStream)
{
	inputStream.Read (r);
	inputStream.Read (g);
	inputStream.Read (b);
	return inputStream.GetStatus ();
}

NE::Stream::Status Color::Write (NE::OutputStream& outputStream) const
{
	outputStream.Write (r);
	outputStream.Write (g);
	outputStream.Write (b);
	return outputStream.GetStatus ();
}

Transformation::Transformation ()
{

}

Transformation::~Transformation ()
{

}


std::wstring Transformation::ToString () const
{
	// TODO
	std::wstring result = L"";
	return result;
}

NE::Stream::Status Transformation::Read (NE::InputStream& inputStream)
{
	// TODO
	return inputStream.GetStatus ();
}

NE::Stream::Status Transformation::Write (NE::OutputStream& outputStream) const
{
	// TODO
	return outputStream.GetStatus ();
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
	Line (Point (), Point (), Color ())
{

}

Line::Line (const Point& beg, const Point& end, const Color& color) :
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
	color.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status Line::Write (NE::OutputStream& outputStream) const
{
	beg.Write (outputStream);
	end.Write (outputStream);
	color.Write (outputStream);
	return outputStream.GetStatus ();
}

Circle::Circle () :
	Circle (Point (), 0, Color ())
{

}

Circle::Circle (const Point& center, double radius, const Color& color) :
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
	result += NE::DoubleToString (radius, 2);
	result += L")";
	return result;
}

NE::Stream::Status Circle::Read (NE::InputStream& inputStream)
{
	center.Read (inputStream);
	inputStream.Read (radius);
	color.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status Circle::Write (NE::OutputStream& outputStream) const
{
	center.Write (outputStream);
	outputStream.Write (radius);
	color.Write (outputStream);
	return outputStream.GetStatus ();
}

ColorValue::ColorValue () :
	ColorValue (Color ())
{

}

ColorValue::ColorValue (const Color& val) :
	NE::GenericValue<Color> (val)
{

}

NE::ValuePtr ColorValue::Clone () const
{
	return NE::ValuePtr (new ColorValue (val));
}

std::wstring ColorValue::ToString () const
{
	return val.ToString ();
}

NE::Stream::Status ColorValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

TransformationValue::TransformationValue () :
	TransformationValue (Transformation ())
{

}

TransformationValue::TransformationValue (const Transformation& val) :
	NE::GenericValue<Transformation> (val)
{

}

NE::ValuePtr TransformationValue::Clone () const
{
	return NE::ValuePtr (new TransformationValue (val));
}

std::wstring TransformationValue::ToString () const
{
	return val.ToString ();
}

NE::Stream::Status TransformationValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformationValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

}
