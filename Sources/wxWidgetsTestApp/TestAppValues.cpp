#include "TestAppValues.hpp"
#include "ResultImage.hpp"
#include "NE_StringUtils.hpp"
#include "NE_ValueCombination.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"

NE::DynamicSerializationInfo	ColorValue::serializationInfo (NE::ObjectId ("{E6D2DBDC-6311-4BA5-9B1A-A0FFF8CA2444}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{2C242A9E-1054-4E16-82C1-759C006097C9}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineValue::serializationInfo (NE::ObjectId ("{2C860493-09EE-4EA2-8BC4-7FD8DE97BBC8}"), NE::ObjectVersion (1), LineValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleValue::serializationInfo (NE::ObjectId ("{88E71F51-D47D-4DE5-9182-D608E7E71741}"), NE::ObjectVersion (1), CircleValue::CreateSerializableInstance);

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

std::wstring Color::ToString (const NE::StringSettings&) const
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

Transformation::Transformation () :
	matrix { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 }
{

}

Transformation::Transformation (double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33) :
	matrix { m11, m12, m13, m21, m22, m23, m31, m32, m33 }
{

}

Transformation::~Transformation ()
{

}

Transformation Transformation::Translation (double x, double y)
{
	return Transformation (
		1.0, 0.0, x,
		0.0, 1.0, y,
		0.0, 0.0, 1.0
	);
}

std::wstring Transformation::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += L"Matrix (";
	for (int i = 0; i < 9; i++) {
		result += NE::DoubleToString (matrix[i], stringSettings);
		if (i < 9 - 1) {
			result += L", ";
		}
	}
	result += L")";
	return result;
}

NE::Stream::Status Transformation::Read (NE::InputStream& inputStream)
{
	for (int i = 0; i < 9; i++) {
		inputStream.Read (matrix[i]);
	}
	return inputStream.GetStatus ();
}

NE::Stream::Status Transformation::Write (NE::OutputStream& outputStream) const
{
	for (int i = 0; i < 9; i++) {
		outputStream.Write (matrix[i]);
	}
	return outputStream.GetStatus ();
}

void Transformation::Apply (double& x, double& y) const
{
	double oX = x;
	double oY = y;
	x = oX * matrix[0] + oY * matrix[1] + 1.0 * matrix[2];
	y = oX * matrix[3] + oY * matrix[4] + 1.0 * matrix[5];
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

std::wstring Point::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += L"Point (";
	result += NE::DoubleToString (x, stringSettings);
	result += L", ";
	result += NE::DoubleToString (y, stringSettings);
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

Point Point::Transform (const Transformation& transformation) const
{
	Point result (x, y);
	transformation.Apply (result.x, result.y);
	return result;
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

std::wstring Line::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += L"Line (";
	result += beg.ToString (stringSettings);
	result += L" - ";
	result += end.ToString (stringSettings);
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

Line Line::Transform (const Transformation& transformation) const
{
	return Line (beg.Transform (transformation), end.Transform (transformation), color);
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

std::wstring Circle::ToString (const NE::StringSettings& stringSettings) const
{
	std::wstring result = L"";
	result += L"Circle (";
	result += center.ToString (stringSettings);
	result += L", ";
	result += NE::DoubleToString (radius, stringSettings);
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

Circle Circle::Transform (const Transformation& transformation) const
{
	return Circle (center.Transform (transformation), radius, color);
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

std::wstring ColorValue::ToString (const NE::StringSettings& stringSettings) const
{
	return val.ToString (stringSettings);
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

GeometricValue::GeometricValue ()
{

}

GeometricValue::~GeometricValue ()
{

}

DrawableValue::DrawableValue ()
{

}

DrawableValue::~DrawableValue ()
{

}

PointValue::PointValue () :
	PointValue (Point ())
{

}

PointValue::PointValue (const Point& val) :
	NE::GenericValue<Point> (val)
{

}

NE::ValuePtr PointValue::Clone () const
{
	return NE::ValuePtr (new PointValue (val));
}

std::wstring PointValue::ToString (const NE::StringSettings& stringSettings) const
{
	return val.ToString (stringSettings);
}

NE::Stream::Status PointValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<Point>::Read (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<Point>::Write (outputStream);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr PointValue::Transform (const Transformation& transformation) const
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
	LineValue (Line ())
{

}

LineValue::LineValue (const Line& val) :
	NE::GenericValue<Line> (val)
{

}

NE::ValuePtr LineValue::Clone () const
{
	return NE::ValuePtr (new LineValue (val));
}

std::wstring LineValue::ToString (const NE::StringSettings& stringSettings) const
{
	return val.ToString (stringSettings);
}

NE::Stream::Status LineValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<Line>::Read (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<Line>::Write (outputStream);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr LineValue::Transform (const Transformation& transformation) const
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
	CircleValue (Circle ())
{

}

CircleValue::CircleValue (const Circle& val) :
	NE::GenericValue<Circle> (val)
{

}

NE::ValuePtr CircleValue::Clone () const
{
	return NE::ValuePtr (new CircleValue (val));
}

std::wstring CircleValue::ToString (const NE::StringSettings& stringSettings) const
{
	return val.ToString (stringSettings);
}

NE::Stream::Status CircleValue::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NE::GenericValue<Circle>::Read (inputStream);
	val.Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NE::GenericValue<Circle>::Write (outputStream);
	val.Write (outputStream);
	return outputStream.GetStatus ();
}

NE::ValuePtr CircleValue::Transform (const Transformation& transformation) const
{
	return NE::ValuePtr (new CircleValue (val.Transform (transformation)));
}

NUIE::DrawingItemConstPtr CircleValue::CreateDrawingItem () const
{
	NUIE::Rect rect = NUIE::Rect::FromCenterAndSize (NUIE::Point (val.center.x, val.center.y), NUIE::Size (val.radius * 2.0, val.radius * 2.0));
	return NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (rect, NUIE::Pen (NUIE::Color (val.color.r, val.color.g, val.color.b), 1.0)));
}
