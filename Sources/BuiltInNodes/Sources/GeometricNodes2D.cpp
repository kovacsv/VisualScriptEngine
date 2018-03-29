#include "GeometricNodes2D.hpp"
#include "StringUtils.hpp"
#include "SingleValues.hpp"
#include "UINodeCommonParameters.hpp"
#include "UINodeLayouts.hpp"

namespace BI
{

NE::DynamicSerializationInfo	ColorValue::serializationInfo (NE::ObjectId ("{E6D2DBDC-6311-4BA5-9B1A-A0FFF8CA2444}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformationValue::serializationInfo (NE::ObjectId ("{D90233EC-EFC9-4B18-BA45-DA1DB9C0EF63}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);

NE::DynamicSerializationInfo	ColorNode::serializationInfo (NE::ObjectId ("{CBB0BCBD-488B-4A35-A796-9A7FED2E9420}"), NE::ObjectVersion (1), ColorNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	TransformNode::serializationInfo (NE::ObjectId ("{76B41F97-8819-4F7E-8377-BD0FC0491C1A}"), NE::ObjectVersion (1), TransformNode::CreateSerializableInstance);

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

Point Point::Transform (const Transformation&) const
{
	// TODO
	return Point (x + 10.0, y + 10.0);
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

Line Line::Transform (const Transformation&) const
{
	// TODO
	return Line ();
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

Circle Circle::Transform (const Transformation&) const
{
	// TODO
	return Circle ();
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

ColorNode::ColorNode () :
	ColorNode (L"", NUIE::Point ())
{

}

ColorNode::ColorNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::UINode (name, position),
	BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest)
{

}

void ColorNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("r"), L"Red", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("g"), L"Green", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), L"Blue", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("color"), L"Color")));
}

NE::ValuePtr ColorNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr r = EvaluateSingleInputSlot (NE::SlotId ("r"), env);
	NE::ValuePtr g = EvaluateSingleInputSlot (NE::SlotId ("g"), env);
	NE::ValuePtr b = EvaluateSingleInputSlot (NE::SlotId ("b"), env);
	if (!NE::IsComplexType<NE::NumberValue> (r) || !NE::IsComplexType<NE::NumberValue> (g) || !NE::IsComplexType<NE::NumberValue> (b)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({ r, g, b }, [&] (const NE::ValueCombination& combination) {
		unsigned char rColor = (unsigned char) NE::NumberValue::ToInteger (combination.GetValue (0));
		unsigned char gColor = (unsigned char) NE::NumberValue::ToInteger (combination.GetValue (1));
		unsigned char bColor = (unsigned char) NE::NumberValue::ToInteger (combination.GetValue (2));
		result->Push (NE::ValuePtr (new BI::ColorValue (BI::Color (rColor, gColor, bColor))));
	});

	return result;
}

void ColorNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class RedParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		RedParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> (L"Red", NUIE::ParameterType::Integer, NE::SlotId ("r"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	class GreenParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		GreenParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> (L"Green", NUIE::ParameterType::Integer, NE::SlotId ("g"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	class BlueParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		BlueParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> (L"Blue", NUIE::ParameterType::Integer, NE::SlotId ("b"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new RedParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new GreenParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new BlueParameter ()));
}

void ColorNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	ValueCombinationFeature::RegisterFeatureCommands (commandRegistrator);
}

NE::Stream::Status ColorNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	return outputStream.GetStatus ();
}

void ColorNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	BI::DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

TransformNode::TransformNode () :
	TransformNode (L"", NUIE::Point ())
{

}

TransformNode::TransformNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::UINode (name, position),
	BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest)
{

}

void TransformNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("geometry"), L"Geometry", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("transformation"), L"Transformation", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("geometry"), L"Geometry")));
}

NE::ValuePtr TransformNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr geometry = EvaluateSingleInputSlot (NE::SlotId ("geometry"), env);
	NE::ValuePtr transformation = EvaluateSingleInputSlot (NE::SlotId ("transformation"), env);
	if (!NE::IsComplexType<BI::GeometricValue> (geometry) || !NE::IsComplexType<BI::TransformationValue> (transformation)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({ geometry, transformation }, [&] (const NE::ValueCombination& combination) {
		BI::GeometricValue* geomValue = NE::Value::Cast<BI::GeometricValue> (combination.GetValue (0).get ());
		const BI::Transformation& transformation = BI::TransformationValue::Get (combination.GetValue (1));
		if (DBGVERIFY (geomValue != nullptr)) {
			result->Push (geomValue->Transform (transformation));
		}
	});

	return result;
}

NE::Stream::Status TransformNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	return outputStream.GetStatus ();
}

void TransformNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	BI::DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

}
