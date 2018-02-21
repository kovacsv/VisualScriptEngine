#include "TestAppNodes.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "UINodeLayouts.hpp"
#include "TestAppValues.hpp"

NE::SerializationInfo			GeometricNode::serializationInfo (NE::ObjectId ("{74700C2B-6587-4850-A2F6-9DAB38896F41}"), NE::ObjectVersion (1));
NE::DynamicSerializationInfo	ColorNode::serializationInfo (NE::ObjectId ("{CBB0BCBD-488B-4A35-A796-9A7FED2E9420}"), NE::ObjectVersion (1), ColorNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{E19AC155-90A7-43EA-9406-8E0876BAE05F}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineNode::serializationInfo (NE::ObjectId ("{3EEBD3D1-7D67-4513-9F29-60E2D7B5DE2B}"), NE::ObjectVersion (1), LineNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleNode::serializationInfo (NE::ObjectId ("{651FEFFD-4F77-4E31-8765-CAF542491261}"), NE::ObjectVersion (1), CircleNode::CreateSerializableInstance);

GeometricNode::GeometricNode () :
	NUIE::CalculationObserverNode ()
{

}

GeometricNode::GeometricNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::CalculationObserverNode (name, position)
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
	NUIE::CalculationObserverNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status GeometricNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::CalculationObserverNode::Write (outputStream);
	return outputStream.GetStatus ();
}

void GeometricNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	NUIE::DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

ColorNode::ColorNode () :
	NUIE::CombinedValueNode ()
{

}

ColorNode::ColorNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::CombinedValueNode (name, position)
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
	CombineValues ({r, g, b}, [&] (const NE::ValueCombination& combination) {
		unsigned char rColor = NE::NumberValue::ToInteger (combination.GetValue (0));
		unsigned char gColor = NE::NumberValue::ToInteger (combination.GetValue (1));
		unsigned char bColor = NE::NumberValue::ToInteger (combination.GetValue (2));
		result->Push (NE::ValuePtr (new ColorValue (Color (rColor, gColor, bColor))));
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

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	class GreenParameter : public NUIE::SlotDefaultValueParameter<ColorNode, NE::IntValue>
	{
	public:
		GreenParameter () :
			SlotDefaultValueParameter<ColorNode, NE::IntValue> ("GreenParameter", L"Green", NUIE::ParameterType::Integer, NE::SlotId ("g"))
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
			SlotDefaultValueParameter<ColorNode, NE::IntValue> ("BlueParameter", L"Blue", NUIE::ParameterType::Integer, NE::SlotId ("b"))
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

NE::Stream::Status ColorNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::CombinedValueNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::CombinedValueNode::Write (outputStream);
	return outputStream.GetStatus ();
}

void ColorNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	NUIE::DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
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
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValuePtr PointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr x = EvaluateSingleInputSlot (NE::SlotId ("x"), env);
	NE::ValuePtr y = EvaluateSingleInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({x, y}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new PointValue (
			Point (
				NE::NumberValue::ToDouble (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1))
			)
		)));
	});

	return result;
}

void PointNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	GeometricNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueParameter<PointNode, NE::DoubleValue> (parameterList, "PositionXParameter", L"Position X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueParameter<PointNode, NE::DoubleValue> (parameterList, "PositionYParameter", L"Position Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
}

NUIE::DrawingItemConstPtr PointNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		Point point = PointValue::Get (innerValue);
		int pointSize = 10;
		NUIE::Rect pointRect (point.x - pointSize / 2, point.y - pointSize / 2, pointSize, pointSize);
		NUIE::Pen pointPen (NUIE::Color (80, 80, 80), 1.0);
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingLine (pointRect.GetTopLeft (), pointRect.GetBottomRight (), pointPen)));
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingLine (pointRect.GetBottomLeft (), pointRect.GetTopRight (), pointPen)));
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
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::DoubleValue (10.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new ColorValue (Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("circle"), L"Circle")));
}

NE::ValuePtr CircleNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("center"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("radius"), env);
	NE::ValuePtr color = EvaluateSingleInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<NE::NumberValue> (end) || !NE::IsComplexType<ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new CircleValue (
			Circle (
				PointValue::Get (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1)),
				ColorValue::Get (combination.GetValue (2))
			))));
	});

	return result;
}

void CircleNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class RadiusParameter : public NUIE::SlotDefaultValueParameter<CircleNode, NE::DoubleValue>
	{
	public:
		RadiusParameter () :
			SlotDefaultValueParameter<CircleNode, NE::DoubleValue> ("RadiusParameter", L"Radius", NUIE::ParameterType::Double, NE::SlotId ("radius"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::DoubleValue>& value) const override
		{
			return value->GetValue () >= 0.0;
		}
	};

	GeometricNode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new RadiusParameter ()));
}

NUIE::DrawingItemConstPtr CircleNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		Circle circle = CircleValue::Get (innerValue);
		NUIE::Rect rect = NUIE::Rect::FromCenterAndSize (NUIE::Point (circle.center.x, circle.center.y), NUIE::Size (circle.radius * 2.0, circle.radius * 2.0));
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
