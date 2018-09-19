#include "TestAppNodes.hpp"
#include "NE_ValueCombination.hpp"
#include "NUIE_UINodeParameters.hpp"
#include "NUIE_UINodeCommonParameters.hpp"
#include "TestAppValues.hpp"
#include "ResultImage.hpp"

NE::DynamicSerializationInfo	ColorNode::serializationInfo (NE::ObjectId ("{CBB0BCBD-488B-4A35-A796-9A7FED2E9420}"), NE::ObjectVersion (1), ColorNode::CreateSerializableInstance);

NE::SerializationInfo			DrawableNode::serializationInfo (NE::ObjectVersion (1));
NE::DynamicSerializationInfo	PointNode::serializationInfo (NE::ObjectId ("{E19AC155-90A7-43EA-9406-8E0876BAE05F}"), NE::ObjectVersion (1), PointNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	LineNode::serializationInfo (NE::ObjectId ("{3EEBD3D1-7D67-4513-9F29-60E2D7B5DE2B}"), NE::ObjectVersion (1), LineNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	CircleNode::serializationInfo (NE::ObjectId ("{651FEFFD-4F77-4E31-8765-CAF542491261}"), NE::ObjectVersion (1), CircleNode::CreateSerializableInstance);
NE::DynamicSerializationInfo	OffsetNode::serializationInfo (NE::ObjectId ("{76B41F97-8819-4F7E-8377-BD0FC0491C1A}"), NE::ObjectVersion (1), OffsetNode::CreateSerializableInstance);

ColorNode::ColorNode () :
	ColorNode (L"", NUIE::Point ())
{

}

ColorNode::ColorNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position),
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

	BI::BasicUINode::RegisterParameters (parameterList);
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
	BI::BasicUINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	return outputStream.GetStatus ();
}

DrawableNode::DrawableNode () :
	DrawableNode (L"", NUIE::Point ())
{

}

DrawableNode::DrawableNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position),
	BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest),
	BI::EnableDisableFeature (true)
{

}

void DrawableNode::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	BI::EnableDisableFeature::RegisterFeatureCommands (commandRegistrator);
	BI::ValueCombinationFeature::RegisterFeatureCommands (commandRegistrator);
}

void DrawableNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
	BI::EnableDisableFeature::RegisterFeatureParameters (parameterList);
	BI::ValueCombinationFeature::RegisterFeatureParameters (parameterList);
}

void DrawableNode::ProcessValue (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	EnableDisableFeature::FeatureProcessValue (value, env);
}

void DrawableNode::OnCalculated (const NE::ValuePtr&, NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (env);
}

void DrawableNode::OnEnabled (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
	AddItem (env);
}

void DrawableNode::OnDisabled (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

void DrawableNode::OnDelete (NE::EvaluationEnv& env) const
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

void DrawableNode::AddItem (NE::EvaluationEnv& env) const
{
	if (DBGERROR (!env.IsDataType<ResultImageEvaluationData> ())) {
		return;
	}
	NE::ValuePtr value = GetCalculatedValue ();
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
	BI::BasicUINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	BI::EnableDisableFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status DrawableNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	BI::EnableDisableFeature::Write (outputStream);
	return outputStream.GetStatus ();
}

void DrawableNode::DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const
{
	EnableDisableFeature::CreateDrawingEnvironment (env, [&] (NUIE::NodeUIDrawingEnvironment& newEnv) {
		BI::BasicUINode::DrawInplace (newEnv);
	});
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

OffsetNode::OffsetNode () :
	OffsetNode (L"", NUIE::Point ())
{

}

OffsetNode::OffsetNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
{

}

void OffsetNode::RegisterSlots ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("geometry"), L"Geometry", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("geometry"), L"Geometry")));
}

NE::ValuePtr OffsetNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValuePtr geometry = EvaluateSingleInputSlot (NE::SlotId ("geometry"), env);
	NE::ValuePtr x = EvaluateSingleInputSlot (NE::SlotId ("x"), env);
	NE::ValuePtr y = EvaluateSingleInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<GeometricValue> (geometry) || !NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	CombineValues ({ geometry, x, y }, [&] (const NE::ValueCombination& combination) {
		GeometricValue* geomValue = NE::Value::Cast<GeometricValue> (combination.GetValue (0).get ());
		Transformation transformation = Transformation::Translation (
			NE::NumberValue::ToDouble (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2))
		);
		if (DBGVERIFY (geomValue != nullptr)) {
			result->Push (geomValue->Transform (transformation));
		}
	});

	return result;
}

void OffsetNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueParameter<OffsetNode, NE::DoubleValue> (parameterList, L"X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueParameter<OffsetNode, NE::DoubleValue> (parameterList, L"Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
}

NE::Stream::Status OffsetNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	BI::ValueCombinationFeature::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status OffsetNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	BI::ValueCombinationFeature::Write (outputStream);
	return outputStream.GetStatus ();
}
