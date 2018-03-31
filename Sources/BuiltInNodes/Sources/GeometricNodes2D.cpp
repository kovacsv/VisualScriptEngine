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

NE::SerializationInfo			TransformationMatrixNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	TranslationMatrixNode::serializationInfo (NE::ObjectId ("{C59400B2-87DA-4CDC-84A0-07F40E26B23E}"), NE::ObjectVersion (1), TranslationMatrixNode::CreateSerializableInstance);

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

std::wstring Transformation::ToString () const
{
	std::wstring result = L"";
	result += L"Matrix (";
	for (int i = 0; i < 9; i++) {
		result += NE::DoubleToString (matrix[i], 2);
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

TransformationMatrixNode::TransformationMatrixNode () :
	TransformationMatrixNode (L"", NUIE::Point ())
{

}

TransformationMatrixNode::TransformationMatrixNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::UINode (name, position),
	BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest)
{

}

void TransformationMatrixNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	ValueCombinationFeature::RegisterFeatureCommands (commandRegistrator);
}

NE::Stream::Status TransformationMatrixNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TransformationMatrixNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	return outputStream.GetStatus ();
}

void TransformationMatrixNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	BI::DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

TranslationMatrixNode::TranslationMatrixNode () :
	TranslationMatrixNode (L"", NUIE::Point ())
{

}

TranslationMatrixNode::TranslationMatrixNode (const std::wstring& name, const NUIE::Point& position) :
	TransformationMatrixNode (name, position)
{

}

void TranslationMatrixNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("matrix"), L"Matrix")));
}

NE::ValuePtr TranslationMatrixNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr x = EvaluateSingleInputSlot (NE::SlotId ("x"), env);
	NE::ValuePtr y = EvaluateSingleInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({ x, y }, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new TransformationValue (
			Transformation::Translation (
				NE::NumberValue::ToDouble (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1))
			)
		)));
	});

	return result;
}

void TranslationMatrixNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	UINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueParameter<TranslationMatrixNode, NE::DoubleValue> (parameterList, L"X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueParameter<TranslationMatrixNode, NE::DoubleValue> (parameterList, L"Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
}

NE::Stream::Status TranslationMatrixNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	TransformationMatrixNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status TranslationMatrixNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	TransformationMatrixNode::Write (outputStream);
	return outputStream.GetStatus ();
}

}
