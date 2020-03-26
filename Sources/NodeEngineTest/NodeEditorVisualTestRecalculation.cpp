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
		TestNode (L"", Point (0.0, 0.0))
	{
	}

	TestNode (const std::wstring& name, const Point& position) :
		BasicUINode (name, position),
		recalcCounter (0)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in"), L"Input", ValuePtr (new NE::IntValue (0)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), L"Output")));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		NE::ValueConstPtr inputVal = EvaluateInputSlot (NE::SlotId ("in"), env);
		recalcCounter++;
		return inputVal;
	}

	size_t GetRecalcCounter () const
	{
		return recalcCounter;
	}

private:
	mutable size_t recalcCounter;
};

DynamicSerializationInfo TestNode::serializationInfo (ObjectId ("{ED945CAC-27C5-460E-B3FF-1B783C43C3DB}"), ObjectVersion (1), TestNode::CreateSerializableInstance);

class RecalcNodeEditorTestEnv : public NodeEditorTestEnv
{
public:
	RecalcNodeEditorTestEnv (const BasicSkinParams& skinParams) :
		NodeEditorTestEnv (skinParams)
	{
		testNode1.reset (new TestNode (L"Test 1", NUIE::Point (200, 200)));
		testNode2.reset (new TestNode (L"Test 2", NUIE::Point (500, 400)));

		nodeEditor.AddNode (testNode1);
		nodeEditor.AddNode (testNode2);
		nodeEditor.ConnectOutputSlotToInputSlot (testNode1->GetUIOutputSlot (SlotId ("out")), testNode2->GetUIInputSlot (SlotId ("in")));

		nodeEditor.Update ();
		RecalcPositions ();
	}

	void RecalcPositions ()
	{
		//doubleInputRect = doubleUpDownNode->GetNodeRect (uiEnvironment);
		//rangeInputRect = rangeInputNode->GetNodeRect (uiEnvironment);
		//viewer1InputRect = viewerUINode1->GetNodeRect (uiEnvironment);
		//viewer2InputRect = viewerUINode2->GetNodeRect (uiEnvironment);
		//
		//doubleInputHeaderPoint = doubleInputRect.GetTopCenter () + Point (5.0, 5.0);
		//rangeInputHeaderPoint = rangeInputRect.GetTopCenter () + Point (5.0, 5.0);	
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
	ASSERT (env.CheckReference ("Recalculation_Initial.svg"));
	ASSERT (env.CheckRecalcCounters (1, 2));
}

}
