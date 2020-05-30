#include "NE_SingleValues.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_BasicUINode.hpp"

class MyEventHandler : public NUIE::EventHandler
{
public:
	MyEventHandler () :
		NUIE::EventHandler ()
	{

	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UINodePtr& uiNode,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UIOutputSlotConstPtr& inputSlot,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UIInputSlotConstPtr& inputSlot,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UINodeGroupPtr& group,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual void OnDoubleClick (
		const NUIE::Point& position) override
	{

	}

	virtual bool OnParameterSettings (
		NUIE::ParameterInterfacePtr paramAccessor,
		const NUIE::UINodePtr& uiNode) override
	{
		return false;
	}

	virtual bool OnParameterSettings (
		NUIE::ParameterInterfacePtr paramAccessor,
		const NUIE::UINodeGroupPtr& uiGroup) override
	{
		return false;
	}
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::GetDefaultStringConverter ()),
		skinParams (NUIE::GetDefaultSkinParams ()),
		drawingContext (),
		eventHandler (),
		clipboardHandler (),
		evaluationEnv (nullptr)
	{

	}

	virtual const NE::StringConverter& GetStringConverter () override
	{
		return stringConverter;
	}

	virtual const NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return drawingContext;
	}

	virtual double GetWindowScale () override
	{
		return 1.0;
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	virtual void OnEvaluationBegin () override
	{

	}

	virtual void OnEvaluationEnd () override
	{

	}

	virtual void OnValuesRecalculated () override
	{

	}

	virtual void OnRedrawRequested () override
	{

	}

	virtual NUIE::EventHandler& GetEventHandler () override
	{
		return eventHandler;
	}

	virtual NUIE::ClipboardHandler& GetClipboardHandler () override
	{
		return clipboardHandler;
	}

private:
	NE::BasicStringConverter		stringConverter;
	NUIE::BasicSkinParams			skinParams;
	NUIE::NullDrawingContext		drawingContext;
	MyEventHandler					eventHandler;
	NUIE::InMemoryClipboardHandler	clipboardHandler;
	NE::EvaluationEnv				evaluationEnv;
};

class MyNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE (MyNode);

public:
	MyNode ()
	{
	}

	MyNode (const NE::String& name, const NUIE::Point& position) :
		BI::BasicUINode (name, position)
	{
	}

	virtual ~MyNode ()
	{
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (
			NE::SlotId ("a"), NE::String (L"A"), NE::ValuePtr (new NE::DoubleValue (0.0)),
			NE::OutputSlotConnectionMode::Single
		)));
		RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (
			NE::SlotId ("b"), NE::String (L"B"), NE::ValuePtr (new NE::DoubleValue (0.0)),
			NE::OutputSlotConnectionMode::Single
		)));
		RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (
			NE::SlotId ("result"), NE::String (L"Result")
		)));
	}

	virtual NE::ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		NE::ValueConstPtr aValue = EvaluateInputSlot (NE::SlotId ("a"), env);
		NE::ValueConstPtr bValue = EvaluateInputSlot (NE::SlotId ("b"), env);
		if (!NE::IsSingleType<NE::NumberValue> (aValue) ||
			!NE::IsSingleType<NE::NumberValue> (bValue))
		{
			return nullptr;
		}

		double aDouble = NE::NumberValue::ToDouble (aValue);
		double bDouble = NE::NumberValue::ToDouble (bValue);
		return NE::ValueConstPtr (new NE::DoubleValue (aDouble + bDouble));
	}

	virtual NE::Stream::Status Read (NE::InputStream& inputStream) override
	{
		NE::ObjectHeader header (inputStream);
		BasicUINode::Read (inputStream);
		return inputStream.GetStatus ();
	}

	virtual NE::Stream::Status Write (NE::OutputStream& outputStream) const override
	{
		NE::ObjectHeader header (outputStream, serializationInfo);
		BasicUINode::Write (outputStream);
		return outputStream.GetStatus ();
	}
};

DYNAMIC_SERIALIZATION_INFO (MyNode, 1, "{8DC2C2F6-C1D1-451A-96C5-D64B3DEDB764}");

int main (int argc, char* argv[])
{
	MyNodeUIEnvironment uiEnvironment;
	NUIE::NodeEditor nodeEditor (uiEnvironment);

	nodeEditor.AddNode (NUIE::UINodePtr (new MyNode (NE::String (L"My Node"), NUIE::Point (0.0, 0.0))));

	nodeEditor.Update ();
	nodeEditor.Draw ();

	return 0;
}
