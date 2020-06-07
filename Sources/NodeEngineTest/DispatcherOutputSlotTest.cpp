#include "SimpleTest.hpp"
#include "TestUtils.hpp"
#include "TestNodes.hpp"

#include "NE_SingleValues.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UIDispatcherOutputSlot.hpp"

using namespace NE;
using namespace NUIE;

namespace DispatcherOutputSlotTest
{

class DispatcherNode : public SerializableTestUINode
{
public:
	DispatcherNode (const LocString& name, const Point& position) :
		SerializableTestUINode (name, position)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIDispatcherOutputSlot (SlotId ("out1"), NE::LocString (L"Out 1"), 0)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIDispatcherOutputSlot (SlotId ("out2"), NE::LocString (L"Out 2"), 1)));
	}

	virtual ValueConstPtr Calculate (EvaluationEnv&) const override
	{
		ValueConstPtr val1 (new IntValue (5));
		ValueConstPtr val2 (new StringValue (L"test"));
		ListValuePtr listVal (new ListValue ());
		listVal->Push (val1);
		listVal->Push (val2);
		return listVal;
	}
};

class ValueNode : public SerializableTestUINode
{
public:
	ValueNode (const LocString& name, const Point& position) :
		SerializableTestUINode (name, position)
	{

	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in"), LocString (L"In"), nullptr, OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), LocString (L"Out"))));
	}

	virtual ValueConstPtr Calculate (EvaluationEnv& env) const override
	{
		return EvaluateInputSlot (SlotId ("in"), env);
	}
};

class TestCalcEnvironment : public NodeUICalculationEnvironment
{
public:
	TestCalcEnvironment () :
		NodeUICalculationEnvironment ()
	{

	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return EmptyEvaluationEnv;
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
};

TEST (DispatcherOutputSlotEvaluationTest)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr dispatcherNode (new DispatcherNode (LocString (L"Node"), Point (0.0, 0.0)));
	UINodePtr valueNode1 (new ValueNode (LocString (L"Node"), Point (0.0, 0.0)));
	UINodePtr valueNode2 (new ValueNode (LocString (L"Node"), Point (0.0, 0.0)));

	ASSERT (uiManager.AddNode (dispatcherNode, NE::EmptyEvaluationEnv) != nullptr);
	ASSERT (uiManager.AddNode (valueNode1, NE::EmptyEvaluationEnv) != nullptr);
	ASSERT (uiManager.AddNode (valueNode2, NE::EmptyEvaluationEnv) != nullptr);

	ASSERT (uiManager.ConnectOutputSlotToInputSlot (dispatcherNode->GetUIOutputSlot (SlotId ("out1")), valueNode1->GetUIInputSlot (SlotId ("in"))));
	ASSERT (uiManager.ConnectOutputSlotToInputSlot (dispatcherNode->GetUIOutputSlot (SlotId ("out2")), valueNode2->GetUIInputSlot (SlotId ("in"))));

	TestCalcEnvironment calcEnv;
	uiManager.Update (calcEnv);

	ASSERT (valueNode1->HasCalculatedValue ());
	ASSERT (valueNode2->HasCalculatedValue ());
	NE::ValueConstPtr val1 = valueNode1->GetCalculatedValue ();
	NE::ValueConstPtr val2 = valueNode2->GetCalculatedValue ();
	ASSERT (Value::IsType<IntValue> (val1) && IntValue::Get (val1) == 5);
	ASSERT (Value::IsType<StringValue> (val2) && StringValue::Get (val2) == L"test");
}

}
