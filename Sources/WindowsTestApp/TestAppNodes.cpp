#include "TestAppNodes.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "CommonUINodeParameters.hpp"

NE::SerializationInfo			GeometricNode::serializationInfo (NE::ObjectId ("{74700C2B-6587-4850-A2F6-9DAB38896F41}"), NE::ObjectVersion (1));
NE::DynamicSerializationInfo	ColorValue::serializationInfo (NE::ObjectId ("{E6D2DBDC-6311-4BA5-9B1A-A0FFF8CA2444}"), NE::ObjectVersion (1), ColorValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointValue::serializationInfo (NE::ObjectId ("{D10E20B6-856A-4AAC-A806-FC60E6D1E82F}"), NE::ObjectVersion (1), PointValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineValue::serializationInfo (NE::ObjectId ("{E899A12E-F87F-4B6B-ACD8-5C86573C382F}"), NE::ObjectVersion (1), LineValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleValue::serializationInfo (NE::ObjectId ("{82190020-867B-4260-94BA-49D8FE94418E}"), NE::ObjectVersion (1), CircleValue::CreateSerializableInstance);
NE::DynamicSerializationInfo	ColorNode::serializationInfo (NE::ObjectId ("{CBB0BCBD-488B-4A35-A796-9A7FED2E9420}"), NE::ObjectVersion (1), ColorNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{E19AC155-90A7-43EA-9406-8E0876BAE05F}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineNode::serializationInfo (NE::ObjectId ("{3EEBD3D1-7D67-4513-9F29-60E2D7B5DE2B}"), NE::ObjectVersion (1), LineNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleNode::serializationInfo (NE::ObjectId ("{651FEFFD-4F77-4E31-8765-CAF542491261}"), NE::ObjectVersion (1), CircleNode::CreateSerializableInstance);

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
	inputStream.Read (val.r);
	inputStream.Read (val.g);
	inputStream.Read (val.b);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (val.r);
	outputStream.Write (val.g);
	outputStream.Write (val.b);
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
	DBGBREAK (); // TODO: Implement this
	return inputStream.GetStatus ();
}

NE::Stream::Status PointValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	DBGBREAK (); // TODO: Implement this
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
	DBGBREAK (); // TODO: Implement this
	return inputStream.GetStatus ();
}

NE::Stream::Status LineValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	DBGBREAK (); // TODO: Implement this
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
	DBGBREAK (); // TODO: Implement this
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleValue::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	DBGBREAK (); // TODO: Implement this
	return outputStream.GetStatus ();
}

GeometricNode::GeometricNode () :
	NUIE::CombinedValueUINode ()
{

}

GeometricNode::GeometricNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::CombinedValueUINode (name, position)
{

}

void GeometricNode::OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (value, env);
}

void GeometricNode::OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (value, env);
}

void GeometricNode::OnDisabled (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

void GeometricNode::OnDeleted (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

void GeometricNode::AddItem (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	if (DBGERROR (!env.IsDataType<ResultImageEvaluationData> ())) {
		return;
	}
	drawingItem = CreateDrawingItem (value);
	if (drawingItem != nullptr) {
		std::shared_ptr<ResultImageEvaluationData> evalData = env.GetData<ResultImageEvaluationData> ();
		evalData->AddItem (drawingItem);
	}
}

void GeometricNode::RemoveItem (NE::EvaluationEnv& env) const
{
	if (DBGERROR (!env.IsDataType<ResultImageEvaluationData> ())) {
		return;
	}
	if (drawingItem != nullptr) {
		std::shared_ptr<ResultImageEvaluationData> evalData = env.GetData<ResultImageEvaluationData> ();
		evalData->RemoveItem (drawingItem);
		drawingItem = nullptr;
	}
}

NE::Stream::Status GeometricNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::CombinedValueUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status GeometricNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::CombinedValueUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

ColorNode::ColorNode () :
	NUIE::CombinedValueUINode ()
{

}

ColorNode::ColorNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::CombinedValueUINode (name, position)
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
	if (!NE::IsComplexType<NE::IntValue> (r) || !NE::IsComplexType<NE::IntValue> (g) || !NE::IsComplexType<NE::IntValue> (b)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({r, g, b}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new ColorValue (Color (NE::IntValue::Get (combination.GetValue (0)), NE::IntValue::Get (combination.GetValue (1)), NE::IntValue::Get (combination.GetValue (2))))));
	});

	return result;
}

void ColorNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class RedParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		RedParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> ("RedParameter", L"Red", NUIE::ParameterType::Integer, NE::SlotId ("r"))
		{

		}
	};

	class GreenParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		GreenParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> ("GreenParameter", L"Green", NUIE::ParameterType::Integer, NE::SlotId ("g"))
		{

		}
	};

	class BlueParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		BlueParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> ("BlueParameter", L"Blue", NUIE::ParameterType::Integer, NE::SlotId ("b"))
		{

		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new RedParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new GreenParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new BlueParameter ()));
}

NE::Stream::Status ColorNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::CombinedValueUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::CombinedValueUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

PointNode::PointNode () :
	GeometricNode ()
{

}

PointNode::PointNode (const std::wstring& name, const NUIE::Point& position) :
	GeometricNode (name, position)
{

}

void PointNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("size"), L"Size", NE::ValuePtr (new NE::IntValue (10)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new ColorValue (Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValuePtr PointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr x = EvaluateSingleInputSlot (NE::SlotId ("x"), env);
	NE::ValuePtr y = EvaluateSingleInputSlot (NE::SlotId ("y"), env);
	NE::ValuePtr size = EvaluateSingleInputSlot (NE::SlotId ("size"), env);
	NE::ValuePtr color = EvaluateSingleInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<NE::IntValue> (x) || !NE::IsComplexType<NE::IntValue> (y) || !NE::IsComplexType<NE::IntValue> (size) || !NE::IsComplexType<ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({x, y, size, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new PointValue (
			Point (
				NE::IntValue::Get (combination.GetValue (0)),
				NE::IntValue::Get (combination.GetValue (1)),
				NE::IntValue::Get (combination.GetValue (2)),
				ColorValue::Get (combination.GetValue (3))
			)
		)));
	});

	return result;
}

void PointNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class PositionXParameter : public NUIE::SlotDefaultValueParameter<PointNode, NE::IntValue>
	{
	public:
		PositionXParameter () :
			SlotDefaultValueParameter<PointNode, NE::IntValue> ("PositionXParameter", L"Position X", NUIE::ParameterType::Integer, NE::SlotId ("x"))
		{

		}
	};

	class PositionYParameter : public NUIE::SlotDefaultValueParameter<PointNode, NE::IntValue>
	{
	public:
		PositionYParameter () :
			SlotDefaultValueParameter<PointNode, NE::IntValue> ("PositionYParameter", L"Position Y", NUIE::ParameterType::Integer, NE::SlotId ("y"))
		{

		}
	};

	class SizeParameter : public NUIE::SlotDefaultValueParameter<PointNode, NE::IntValue>
	{
	public:
		SizeParameter () :
			SlotDefaultValueParameter<PointNode, NE::IntValue> ("SizeParameter", L"Size", NUIE::ParameterType::Integer, NE::SlotId ("size"))
		{

		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new PositionXParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new PositionYParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new SizeParameter ()));
}

NUIE::DrawingItemConstPtr PointNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		Point point = PointValue::Get (innerValue);
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillEllipse (NUIE::Rect (point.x - point.size / 2, point.y - point.size / 2, point.size, point.size), NUIE::Color (point.color.r, point.color.g, point.color.b))));
	});
	return result;
}

NE::Stream::Status PointNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	GeometricNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	GeometricNode::Write (outputStream);
	return outputStream.GetStatus ();
}

LineNode::LineNode () :
	GeometricNode ()
{

}

LineNode::LineNode (const std::wstring& name, const NUIE::Point& position) :
	GeometricNode (name, position)
{

}

void LineNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("beg"), L"Beg", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("end"), L"End", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new ColorValue (Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("line"), L"Line")));
}

NE::ValuePtr LineNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("beg"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("end"), env);
	NE::ValuePtr color = EvaluateSingleInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<PointValue> (end) || !NE::IsComplexType<ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new LineValue (
			Line (
				PointValue::Get (combination.GetValue (0)),
				PointValue::Get (combination.GetValue (1)),
				ColorValue::Get (combination.GetValue (2))
			))));
	});

	return result;
}

NUIE::DrawingItemConstPtr LineNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		Line line = LineValue::Get (innerValue);
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingLine (NUIE::Point (line.beg.x, line.beg.y), NUIE::Point (line.end.x, line.end.y), NUIE::Pen (NUIE::Color (line.color.r, line.color.g, line.color.b), 1.0))));
	});
	return result;
}

NE::Stream::Status LineNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	GeometricNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	GeometricNode::Write (outputStream);
	return outputStream.GetStatus ();
}

CircleNode::CircleNode () :
	GeometricNode ()
{

}

CircleNode::CircleNode (const std::wstring& name, const NUIE::Point& position) :
	GeometricNode (name, position)
{

}

void CircleNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("center"), L"Center", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new ColorValue (Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("circle"), L"Circle")));
}

NE::ValuePtr CircleNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("center"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("radius"), env);
	NE::ValuePtr color = EvaluateSingleInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<NE::IntValue> (end) || !NE::IsComplexType<ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new CircleValue (
			Circle (
				PointValue::Get (combination.GetValue (0)),
				NE::IntValue::Get (combination.GetValue (1)),
				ColorValue::Get (combination.GetValue (2))
			))));
	});

	return result;
}

NUIE::DrawingItemConstPtr CircleNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		Circle circle = CircleValue::Get (innerValue);
		NUIE::Rect rect = NUIE::Rect::FromCenterAndSize (NUIE::Point (circle.center.x, circle.center.y), NUIE::Size (circle.radius / 2.0, circle.radius / 2.0));
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (rect, NUIE::Pen (NUIE::Color (circle.color.r, circle.color.g, circle.color.b), 1.0))));
	});
	return result;
}

NE::Stream::Status CircleNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	GeometricNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	GeometricNode::Write (outputStream);
	return outputStream.GetStatus ();
}
