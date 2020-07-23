#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorisualTestRecalculation
{

class TestNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (TestNode);

public:
	TestNode () :
		TestNode (LocString (), Point (0.0, 0.0))
	{
	}

	TestNode (const LocString& name, const Point& position) :
		BasicUINode (name, position),
		recalcCounter (0)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in"), LocString (L"Input"), ValuePtr (new IntValue (0)), OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), LocString (L"Output"))));
	}

	virtual ValueConstPtr Calculate (EvaluationEnv& env) const override
	{
		ValueConstPtr inputVal = EvaluateInputSlot (SlotId ("in"), env);
		recalcCounter++;
		return inputVal;
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		BasicUINode::Read (inputStream);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		BasicUINode::Write (outputStream);
		return outputStream.GetStatus ();
	}

	size_t GetRecalcCounter () const
	{
		return recalcCounter;
	}

private:
	mutable size_t recalcCounter;
};

DYNAMIC_SERIALIZATION_INFO (TestNode, 1, "{ED945CAC-27C5-460E-B3FF-1B783C43C3DB}");

class RecalcNodeEditorTestEnv : public NodeEditorTestEnv
{
public:
	RecalcNodeEditorTestEnv (const BasicSkinParams& skinParams) :
		NodeEditorTestEnv (skinParams)
	{
		testNode1.reset (new TestNode (LocString (L"Test 1"), NUIE::Point (200, 200)));
		testNode2.reset (new TestNode (LocString (L"Test 2"), NUIE::Point (500, 400)));

		nodeEditor.AddNode (testNode1);
		nodeEditor.AddNode (testNode2);
		nodeEditor.ConnectOutputSlotToInputSlot (testNode1->GetUIOutputSlot (SlotId ("out")), testNode2->GetUIInputSlot (SlotId ("in")));

		nodeEditor.Update ();
	}

	bool CheckNodeValues (int nodeVal1, int nodeVal2)
	{
		if (!testNode1->HasCalculatedValue () || !testNode2->HasCalculatedValue ()) {
			return false;
		}
		ValueConstPtr val1 = testNode1->GetCalculatedValue ();
		ValueConstPtr val2 = testNode1->GetCalculatedValue ();
		if (IntValue::Get (val1) != nodeVal1 || IntValue::Get (val2) != nodeVal2) {
			return false;
		}
		return true;
	}

	bool CheckRecalcCounters (size_t counter1, size_t counter2)
	{
		return testNode1->GetRecalcCounter () == counter1 && testNode2->GetRecalcCounter () == counter2;
	}

	std::shared_ptr<TestNode>	testNode1;
	std::shared_ptr<TestNode>	testNode2;
};

TEST (RecalculationVisualTest)
{
	RecalcNodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"Recalculation_Initial.svg"));
	ASSERT (env.CheckNodeValues (0, 0));
	ASSERT (env.CheckRecalcCounters (1, 2));
	
	{
		Rect test2NodeRect = env.testNode2->GetNodeRect (env.uiEnvironment);
		Point dragStart = test2NodeRect.GetTopLeft () + Point (5.0, 5.0);
		Point dragEnd (dragStart + Point (20.0, -20.0));
		env.DragDrop (dragStart, dragEnd);
		ASSERT (env.CheckReference (L"Recalculation_Node2Moved.svg"));
		ASSERT (env.CheckNodeValues (0, 0));
		ASSERT (env.CheckRecalcCounters (1, 2));
	}

	{
		Rect test1NodeRect = env.testNode1->GetNodeRect (env.uiEnvironment);
		Point test1HeaderPoint = test1NodeRect.GetTopLeft () + Point (5.0, 5.0);
		env.SetNextCommandParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			paramInterface->SetParameterValue (0, ValueConstPtr (new StringValue (L"Test 1 New")));
			return true;
		});
		env.RightClick (test1HeaderPoint);
		ASSERT (env.CheckReference (L"Recalculation_Node1Renamed.svg"));
		ASSERT (env.CheckNodeValues (0, 0));
		ASSERT (env.CheckRecalcCounters (1, 2));
	}
}

}
