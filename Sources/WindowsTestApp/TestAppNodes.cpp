#include "TestAppNodes.hpp"
#include "ResultImage.hpp"
#include "ValueCombination.hpp"
#include "UINodeParameters.hpp"
#include "UINodeCommonParameters.hpp"
#include "UINodeLayouts.hpp"
#include "TestAppValues.hpp"

NE::SerializationInfo			DrawableNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{E19AC155-90A7-43EA-9406-8E0876BAE05F}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineNode::serializationInfo (NE::ObjectId ("{3EEBD3D1-7D67-4513-9F29-60E2D7B5DE2B}"), NE::ObjectVersion (1), LineNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleNode::serializationInfo (NE::ObjectId ("{651FEFFD-4F77-4E31-8765-CAF542491261}"), NE::ObjectVersion (1), CircleNode::CreateSerializableInstance);

DrawableNode::DrawableNode () :
	DrawableNode (L"", NUIE::Point ())
{

}

DrawableNode::DrawableNode (const std::wstring& name, const NUIE::Point& position) :
	NUIE::UINode (name, position),
	BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest),
	BI::EnableDisableFeature (true)
{

}

void DrawableNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	BI::ValueCombinationFeature::RegisterFeatureCommands (commandRegistrator);
	BI::EnableDisableFeature::RegisterFeatureCommands (commandRegistrator);
}

void DrawableNode::CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	EnableDisableFeature::FeatureCalculationPostProcess (value, env);
}

void DrawableNode::OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (value, env);
}

void DrawableNode::OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (value, env);
}

void DrawableNode::OnDisabled (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

void DrawableNode::OnDeleted (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

NUIE::DrawingItemConstPtr DrawableNode::CreateDrawingItem (const NE::ValuePtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValuePtr& innerValue) {
		DrawableValue* drawableValue = NE::Value::Cast<DrawableValue> (innerValue.get ());
		if (DBGVERIFY (drawableValue != nullptr)) {
			result->AddItem (drawableValue->CreateDrawingItem ());
		}
	});
	return result;
}

void DrawableNode::AddItem (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
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

void DrawableNode::RemoveItem (NE::EvaluationEnv& env) const
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

NE::Stream::Status DrawableNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	BI::EnableDisableFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status DrawableNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	BI::EnableDisableFeature::Write (outputStream);
	return outputStream.GetStatus ();
}

void DrawableNode::DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const
{
	EnableDisableFeature::CreateDrawingEnvironment (env, [&] (NUIE::NodeUIDrawingEnvironment& newEnv) {
		UINode::DrawInplace (newEnv);
	});
}

void DrawableNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	BI::DrawStatusHeaderWithSlotsLayout (*this, env, drawingImage);
}

PointNode::PointNode () :
	DrawableNode ()
{

}

PointNode::PointNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
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
			BI::Point (
				NE::NumberValue::ToDouble (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1))
			)
		)));
	});

	return result;
}

void PointNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	DrawableNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueParameter<PointNode, NE::DoubleValue> (parameterList, L"Position X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueParameter<PointNode, NE::DoubleValue> (parameterList, L"Position Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
}

NE::Stream::Status PointNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	DrawableNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status PointNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	DrawableNode::Write (outputStream);
	return outputStream.GetStatus ();
}

LineNode::LineNode () :
	DrawableNode ()
{

}

LineNode::LineNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
{

}

void LineNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("beg"), L"Beg", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("end"), L"End", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new BI::ColorValue (BI::Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("line"), L"Line")));
}

NE::ValuePtr LineNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("beg"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("end"), env);
	NE::ValuePtr color = EvaluateSingleInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<PointValue> (end) || !NE::IsComplexType<BI::ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new LineValue (
			BI::Line (
				PointValue::Get (combination.GetValue (0)),
				PointValue::Get (combination.GetValue (1)),
				BI::ColorValue::Get (combination.GetValue (2))
			))));
	});

	return result;
}

NE::Stream::Status LineNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	DrawableNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status LineNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	DrawableNode::Write (outputStream);
	return outputStream.GetStatus ();
}

CircleNode::CircleNode () :
	DrawableNode ()
{

}

CircleNode::CircleNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
{

}

void CircleNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("center"), L"Center", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::DoubleValue (10.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new BI::ColorValue (BI::Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("circle"), L"Circle")));
}

NE::ValuePtr CircleNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr beg = EvaluateSingleInputSlot (NE::SlotId ("center"), env);
	NE::ValuePtr end = EvaluateSingleInputSlot (NE::SlotId ("radius"), env);
	NE::ValuePtr color = EvaluateSingleInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<NE::NumberValue> (end) || !NE::IsComplexType<BI::ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new CircleValue (
			BI::Circle (
				PointValue::Get (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1)),
				BI::ColorValue::Get (combination.GetValue (2))
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
			SlotDefaultValueParameter<CircleNode, NE::DoubleValue> (L"Radius", NUIE::ParameterType::Double, NE::SlotId ("radius"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodePtr&, const std::shared_ptr<NE::DoubleValue>& value) const override
		{
			return value->GetValue () >= 0.0;
		}
	};

	DrawableNode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new RadiusParameter ()));
}

NE::Stream::Status CircleNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	DrawableNode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status CircleNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	DrawableNode::Write (outputStream);
	return outputStream.GetStatus ();
}
