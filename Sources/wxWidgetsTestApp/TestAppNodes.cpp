#include "TestAppNodes.hpp"
#include "NE_ValueCombination.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "TestAppValues.hpp"
#include "ResultImage.hpp"

SERIALIZATION_INFO (DrawableNode, 1);
DYNAMIC_SERIALIZATION_INFO (ColorNode, 1, "{CBB0BCBD-488B-4A35-A796-9A7FED2E9420}");
DYNAMIC_SERIALIZATION_INFO (PointNode, 1, "{E19AC155-90A7-43EA-9406-8E0876BAE05F}");
DYNAMIC_SERIALIZATION_INFO (LineNode, 1, "{3EEBD3D1-7D67-4513-9F29-60E2D7B5DE2B}");
DYNAMIC_SERIALIZATION_INFO (CircleNode, 1, "{651FEFFD-4F77-4E31-8765-CAF542491261}");
DYNAMIC_SERIALIZATION_INFO (OffsetNode, 1, "{76B41F97-8819-4F7E-8377-BD0FC0491C1A}");

ColorNode::ColorNode () :
	ColorNode (L"", NUIE::Point ())
{

}

ColorNode::ColorNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void ColorNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("r"), L"Red", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("g"), L"Green", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("b"), L"Blue", NE::ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("color"), L"Color")));
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest)));
}

NE::ValueConstPtr ColorNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr r = EvaluateInputSlot (NE::SlotId ("r"), env);
	NE::ValueConstPtr g = EvaluateInputSlot (NE::SlotId ("g"), env);
	NE::ValueConstPtr b = EvaluateInputSlot (NE::SlotId ("b"), env);
	if (!NE::IsComplexType<NE::NumberValue> (r) || !NE::IsComplexType<NE::NumberValue> (g) || !NE::IsComplexType<NE::NumberValue> (b)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, { r, g, b }, [&] (const NE::ValueCombination& combination) {
		unsigned char rColor = (unsigned char) NE::NumberValue::ToInteger (combination.GetValue (0));
		unsigned char gColor = (unsigned char) NE::NumberValue::ToInteger (combination.GetValue (1));
		unsigned char bColor = (unsigned char) NE::NumberValue::ToInteger (combination.GetValue (2));
		result->Push (NE::ValuePtr (new ColorValue (Color (rColor, gColor, bColor))));
		return true;
	});

	return result;
}

void ColorNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class RedParameter : public NUIE::SlotDefaultValueNodeParameter<ColorNode, NE::IntValue>
	{
	public:
		RedParameter () :
			SlotDefaultValueNodeParameter<ColorNode, NE::IntValue> (L"Red", NUIE::ParameterType::Integer, NE::SlotId ("r"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	class GreenParameter : public NUIE::SlotDefaultValueNodeParameter<ColorNode, NE::IntValue>
	{
	public:
		GreenParameter () :
			SlotDefaultValueNodeParameter<ColorNode, NE::IntValue> (L"Green", NUIE::ParameterType::Integer, NE::SlotId ("g"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	class BlueParameter : public NUIE::SlotDefaultValueNodeParameter<ColorNode, NE::IntValue>
	{
	public:
		BlueParameter () :
			SlotDefaultValueNodeParameter<ColorNode, NE::IntValue> (L"Blue", NUIE::ParameterType::Integer, NE::SlotId ("b"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
		{
			return value->GetValue () >= 0 && value->GetValue () <= 255;
		}
	};

	BI::BasicUINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new RedParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new GreenParameter ()));
	parameterList.AddParameter (NUIE::NodeParameterPtr (new BlueParameter ()));
}

NE::Stream::Status ColorNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ColorNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

DrawableNode::DrawableNode () :
	DrawableNode (L"", NUIE::Point ())
{

}

DrawableNode::DrawableNode (const std::wstring& name, const NUIE::Point& position) :
	BI::BasicUINode (name, position)
{

}

void DrawableNode::Initialize ()
{
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::EnableDisableFeature (BI::EnableDisableFeature::State::Enabled, BI::EnableDisableFeature::Mode::DoNotInvalidate)));
	RegisterFeature (NUIE::NodeFeaturePtr (new BI::ValueCombinationFeature (NE::ValueCombinationMode::Longest)));
}

void DrawableNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
}

void DrawableNode::ProcessCalculatedValue (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const
{
	std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
	if (enableDisable->GetState () == BI::EnableDisableFeature::State::Enabled) {
		OnCalculated (value, env);
	}
}

void DrawableNode::OnCalculated (const NE::ValueConstPtr&, NE::EvaluationEnv& env) const
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

void DrawableNode::OnFeatureChange (const NUIE::FeatureId& featureId, NE::EvaluationEnv& env) const
{
	if (featureId == BI::EnableDisableFeatureId) {
		std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
		if (enableDisable->GetState () == BI::EnableDisableFeature::State::Enabled) {
			OnEnabled (env);
		} else {
			OnDisabled (env);
		}
	}
}

void DrawableNode::OnDelete (NE::EvaluationEnv& env) const
{
	RemoveItem (env);
}

NUIE::DrawingItemConstPtr DrawableNode::CreateDrawingItem (const NE::ValueConstPtr& value) const
{
	if (!NE::Value::IsType<NE::ListValue> (value)) {
		return nullptr;
	}
	std::shared_ptr<NUIE::MultiDrawingItem> result (new NUIE::MultiDrawingItem ());
	NE::Value::Cast<NE::ListValue> (value)->Enumerate ([&] (const NE::ValueConstPtr& innerValue) {
		const DrawableValue* drawableValue = NE::Value::Cast<DrawableValue> (innerValue.get ());
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
	NE::ValueConstPtr value = GetCalculatedValue ();
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
	return inputStream.GetStatus ();
}

NE::Stream::Status DrawableNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

void DrawableNode::DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const
{
	std::shared_ptr<BI::EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
	enableDisable->DrawInplace (env, [&] (NUIE::NodeUIDrawingEnvironment& newEnv) {
		BI::BasicUINode::DrawInplace (newEnv);
	});
}

PointNode::PointNode () :
	PointNode (L"", NUIE::Point ())
{

}

PointNode::PointNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
{

}

void PointNode::Initialize ()
{
	DrawableNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("point"), L"Point")));
}

NE::ValueConstPtr PointNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr x = EvaluateInputSlot (NE::SlotId ("x"), env);
	NE::ValueConstPtr y = EvaluateInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {x, y}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new PointValue (
			Point (
				NE::NumberValue::ToDouble (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1))
			)
		)));
		return true;
	});

	return result;
}

void PointNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	DrawableNode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<PointNode, NE::DoubleValue> (parameterList, L"Position X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueNodeParameter<PointNode, NE::DoubleValue> (parameterList, L"Position Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
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
	LineNode (L"", NUIE::Point ())
{

}

LineNode::LineNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
{

}

void LineNode::Initialize ()
{
	DrawableNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("beg"), L"Beg", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("end"), L"End", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new ColorValue (Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("line"), L"Line")));
}

NE::ValueConstPtr LineNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr beg = EvaluateInputSlot (NE::SlotId ("beg"), env);
	NE::ValueConstPtr end = EvaluateInputSlot (NE::SlotId ("end"), env);
	NE::ValueConstPtr color = EvaluateInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<PointValue> (end) || !NE::IsComplexType<ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new LineValue (
			Line (
				PointValue::Get (combination.GetValue (0)),
				PointValue::Get (combination.GetValue (1)),
				ColorValue::Get (combination.GetValue (2))
			)
		)));
		return true;
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
	CircleNode (L"", NUIE::Point ())
{

}

CircleNode::CircleNode (const std::wstring& name, const NUIE::Point& position) :
	DrawableNode (name, position)
{

}

void CircleNode::Initialize ()
{
	DrawableNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("center"), L"Center", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("radius"), L"Radius", NE::ValuePtr (new NE::DoubleValue (10.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("color"), L"Color", NE::ValuePtr (new ColorValue (Color (0, 0, 0))), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("circle"), L"Circle")));
}

NE::ValueConstPtr CircleNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr beg = EvaluateInputSlot (NE::SlotId ("center"), env);
	NE::ValueConstPtr end = EvaluateInputSlot (NE::SlotId ("radius"), env);
	NE::ValueConstPtr color = EvaluateInputSlot (NE::SlotId ("color"), env);
	if (!NE::IsComplexType<PointValue> (beg) || !NE::IsComplexType<NE::NumberValue> (end) || !NE::IsComplexType<ColorValue> (color)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, {beg, end, color}, [&] (const NE::ValueCombination& combination) {
		result->Push (NE::ValuePtr (new CircleValue (
			Circle (
				PointValue::Get (combination.GetValue (0)),
				NE::NumberValue::ToDouble (combination.GetValue (1)),
				ColorValue::Get (combination.GetValue (2))
			)
		)));
		return true;
	});

	return result;
}

void CircleNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class RadiusParameter : public NUIE::SlotDefaultValueNodeParameter<CircleNode, NE::DoubleValue>
	{
	public:
		RadiusParameter () :
			SlotDefaultValueNodeParameter<CircleNode, NE::DoubleValue> (L"Radius", NUIE::ParameterType::Double, NE::SlotId ("radius"))
		{

		}

		virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<const NE::DoubleValue>& value) const override
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

void OffsetNode::Initialize ()
{
	DrawableNode::Initialize ();
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("geometry"), L"Geometry", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("x"), L"X", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("y"), L"Y", NE::ValuePtr (new NE::DoubleValue (0.0)), NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("geometry"), L"Geometry")));
}

NE::ValueConstPtr OffsetNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr geometry = EvaluateInputSlot (NE::SlotId ("geometry"), env);
	NE::ValueConstPtr x = EvaluateInputSlot (NE::SlotId ("x"), env);
	NE::ValueConstPtr y = EvaluateInputSlot (NE::SlotId ("y"), env);
	if (!NE::IsComplexType<GeometricValue> (geometry) || !NE::IsComplexType<NE::NumberValue> (x) || !NE::IsComplexType<NE::NumberValue> (y)) {
		return nullptr;
	}

	NE::ListValuePtr result (new NE::ListValue ());
	BI::CombineValues (this, { geometry, x, y }, [&] (const NE::ValueCombination& combination) {
		const GeometricValue* geomValue = NE::Value::Cast<GeometricValue> (combination.GetValue (0).get ());
		Transformation transformation = Transformation::Translation (
			NE::NumberValue::ToDouble (combination.GetValue (1)),
			NE::NumberValue::ToDouble (combination.GetValue (2))
		);
		if (DBGVERIFY (geomValue != nullptr)) {
			result->Push (geomValue->Transform (transformation));
		}
		return true;
	});

	return result;
}

void OffsetNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	BI::BasicUINode::RegisterParameters (parameterList);
	NUIE::RegisterSlotDefaultValueNodeParameter<OffsetNode, NE::DoubleValue> (parameterList, L"X", NUIE::ParameterType::Double, NE::SlotId ("x"));
	NUIE::RegisterSlotDefaultValueNodeParameter<OffsetNode, NE::DoubleValue> (parameterList, L"Y", NUIE::ParameterType::Double, NE::SlotId ("y"));
}

NE::Stream::Status OffsetNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BI::BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status OffsetNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BI::BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}
