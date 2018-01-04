#include "TestAppNodes.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"

NE::SerializationInfo			GeometricNode::serializationInfo (NE::ObjectId ("{74700C2B-6587-4850-A2F6-9DAB38896F41}"), NE::ObjectVersion (1));
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{E19AC155-90A7-43EA-9406-8E0876BAE05F}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineNode::serializationInfo (NE::ObjectId ("{3EEBD3D1-7D67-4513-9F29-60E2D7B5DE2B}"), NE::ObjectVersion (1), LineNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleNode::serializationInfo (NE::ObjectId ("{651FEFFD-4F77-4E31-8765-CAF542491261}"), NE::ObjectVersion (1), CircleNode::CreateSerializableInstance);

Point::Point (int x, int y) :
	x (x),
	y (y)
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

Line::Line (Point beg, Point end) :
	beg (beg),
	end (end)
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

Circle::Circle (Point center, int radius) :
	center (center),
	radius (radius)
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

PointValue::PointValue (const Point& val) :
	NE::GenericValue<Point> (val)
{

}

std::wstring PointValue::ToString () const
{
	return GetValue ().ToString ();
}

LineValue::LineValue (const Line& val) :
	NE::GenericValue<Line> (val)
{

}

std::wstring LineValue::ToString () const
{
	return GetValue ().ToString ();
}

CircleValue::CircleValue (const Circle& val) :
	NE::GenericValue<Circle> (val)
{

}

std::wstring CircleValue::ToString () const
{
	return GetValue ().ToString ();
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
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValuePtr PointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr x = EvaluateSingleInputSlot (NE::SlotId ("x"), env);
	NE::ValuePtr y = EvaluateSingleInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<NE::IntValue> (x) || !NE::IsComplexType<NE::IntValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({x, y}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new PointValue (Point (NE::IntValue::Get (combination.GetValue (0)), NE::IntValue::Get (combination.GetValue (1))))));
	});

	return result;
}

void PointNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	class ExampleCommand : public NUIE::NodeCommand
	{
	public:
		ExampleCommand () :
			NodeCommand (L"Rename command", false)
		{

		}

		virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
		{
			return std::dynamic_pointer_cast<PointNode> (uiNode) != nullptr;
		}

		virtual void Do (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode) override
		{
			uiNode->SetNodeName (L"New Node Name");
			uiManager.RequestRedraw ();
		}
	};

	class ExampleCommand2 : public NUIE::NodeCommand
	{
	public:
		ExampleCommand2 () :
			NodeCommand (L"Set Default To (50, 50)", false)
		{

		}

		virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
		{
			return std::dynamic_pointer_cast<PointNode> (uiNode) != nullptr;
		}

		virtual void Do (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode) override
		{
			std::shared_ptr<PointNode> pointNode = std::dynamic_pointer_cast<PointNode> (uiNode);
			if (DBGERROR (pointNode == nullptr)) {
				return;
			}
			pointNode->GetUIInputSlot (NE::SlotId ("x"))->SetDefaultValue (NE::ValuePtr (new NE::IntValue (50)));
			pointNode->GetUIInputSlot (NE::SlotId ("y"))->SetDefaultValue (NE::ValuePtr (new NE::IntValue (50)));
			uiManager.RequestRecalculate ();
		}
	};

	GeometricNode::RegisterCommands (commandRegistrator);
	commandRegistrator.RegisterNodeCommand (NUIE::NodeCommandPtr (new ExampleCommand ()));
	commandRegistrator.RegisterNodeCommand (NUIE::NodeCommandPtr (new ExampleCommand2 ()));
}

NUIE::DrawingItemConstPtr PointNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		Point point = PointValue::Get (innerValue);
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingFillEllipse (NUIE::Rect (point.x - 5, point.y - 5, 10, 10), NUIE::Color (0, 0, 0))));
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
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("line"), L"Line")));
}

NE::ValuePtr LineNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("beg"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("end"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<PointValue> (end)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new LineValue (Line (PointValue::Get (combination.GetValue (0)), PointValue::Get (combination.GetValue (1))))));
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
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingLine (NUIE::Point (line.beg.x, line.beg.y), NUIE::Point (line.end.x, line.end.y), NUIE::Pen (NUIE::Color (0, 0, 0), 1.0))));
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
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("circle"), L"Circle")));
}

NE::ValuePtr CircleNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("center"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("radius"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<NE::IntValue> (end)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new CircleValue (Circle (PointValue::Get (combination.GetValue (0)), NE::IntValue::Get (combination.GetValue (1))))));
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
		result->AddItem (NUIE::DrawingItemConstPtr (new NUIE::DrawingEllipse (rect, NUIE::Pen (NUIE::Color (0, 0, 0), 1.0))));
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
