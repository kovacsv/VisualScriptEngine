#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInNodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestWorkflow
{

class WorkflowTestEnv : public NodeEditorTestEnv
{
public:
	WorkflowTestEnv (const BasicSkinParams& skinParams) :
		NodeEditorTestEnv (skinParams)
	{

	}

	bool CheckStepReference (int step)
	{
		std::wstring stepString = std::to_wstring (step);
		while (stepString.length () < 2) {
			stepString = L"0" + stepString;
		}
		return CheckReference (L"WorkflowTest_" + stepString + L".svg");
	}
};

TEST (WorkflowTest)
{
	WorkflowTestEnv env (GetDefaultSkinParams ());

	Point padding (10.0, 10.0);
	UINodePtr doubleIncrementNode = nullptr;
	UINodePtr doubleNode = nullptr;
	UINodePtr divisionNode = nullptr;
	UINodePtr viewerNode1 = nullptr;
	UINodePtr viewerNode2 = nullptr;

	{ // start with an empty canvas
		ASSERT (env.CheckStepReference (1));
	}

	{ // add double incremented node
		doubleIncrementNode.reset (new DoubleIncrementedNode (LocString (L"Numbers"), Point (100.0, 150.0)));
		env.nodeEditor.AddNode (doubleIncrementNode);
		ASSERT (env.CheckStepReference (2));
	}

	{ // add double node
		doubleNode.reset (new DoubleUpDownNode (LocString (L"Number"), Point (150.0, 300.0), 0, 1));
		env.nodeEditor.AddNode (doubleNode);
		ASSERT (env.CheckStepReference (3));
	}

	{ // add division node
		divisionNode.reset (new DivisionNode (LocString (L"Division"), Point (350.0, 150.0)));
		env.nodeEditor.AddNode (divisionNode);
		ASSERT (env.CheckStepReference (4));
	}

	{ // connect double incremented to division
		env.DragDrop (
			env.GetOutputSlotConnPosition (doubleIncrementNode, "out"),
			env.GetInputSlotConnPosition (divisionNode, "a")
		);
		ASSERT (env.CheckStepReference (5));
	}

	{ // connect double to division
		env.DragDrop (
			env.GetOutputSlotConnPosition (doubleNode, "out"),
			env.GetInputSlotConnPosition (divisionNode, "b")
		);
		ASSERT (env.CheckStepReference (6));
	}

	{ // increase number twice
		Point plusButtonCenter = doubleNode->GetSpecialRect (env.uiEnvironment, "plus").GetCenter ();
		env.Click (plusButtonCenter);
		env.Click (plusButtonCenter);
		ASSERT (env.CheckStepReference (7));
	}

	{ // add viewer node
		viewerNode1.reset (new MultiLineViewerNode (LocString (L"Viewer 1"), Point (600.0, 150.0), 5));
		env.nodeEditor.AddNode (viewerNode1);
		ASSERT (env.CheckStepReference (8));
	}

	{ // add another viewer node
		viewerNode2.reset (new MultiLineViewerNode (LocString (L"Viewer 2"), Point (600.0, 450.0), 5));
		env.nodeEditor.AddNode (viewerNode2);
		ASSERT (env.CheckStepReference (9));
	}

	{ // connect division to viewer 1
		env.DragDrop (
			env.GetOutputSlotConnPosition (divisionNode, "result"),
			env.GetInputSlotConnPosition (viewerNode1, "in")
		);
		ASSERT (env.CheckStepReference (10));
	}

	{ // connect division to viewer 2
		env.DragDrop (
			env.GetOutputSlotConnPosition (divisionNode, "result"),
			env.GetInputSlotConnPosition (viewerNode2, "in")
		);
		ASSERT (env.CheckStepReference (11));
	}

	{ // increase number again
		Point plusButtonCenter = doubleNode->GetSpecialRect (env.uiEnvironment, "plus").GetCenter ();
		env.Click (plusButtonCenter);
		ASSERT (env.CheckStepReference (12));
	}

	{ // select number nodes
		env.DragDrop (
			env.GetNodeRect (doubleIncrementNode).GetTopLeft () - padding,
			env.GetNodeRect (doubleNode).GetBottomRight () + padding
		);
		ASSERT (env.CheckStepReference (13));
	}

	{ // align number nodes to the right
		env.SetNextCommandName (L"Horizontal Right");
		env.RightClick (env.GetNodeRect (doubleIncrementNode).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (14));
	}

	{ // group selected nodes
		env.SetNextCommandName (L"Create New Group");
		env.RightClick (env.GetNodeRect (doubleIncrementNode).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (15));
	}

	{ // select viewer node 2
		env.DragDrop (
			env.GetNodeRect (viewerNode2).GetTopLeft () - padding,
			env.GetNodeRect (viewerNode2).GetBottomRight () + padding
		);
		ASSERT (env.CheckStepReference (16));
	}

	{ // group viewer node 2
		env.SetNextCommandName (L"Create New Group");
		env.RightClick (env.GetNodeRect (viewerNode2).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (17));
	}

	{ // select double incremented node next to viewer 2
		env.CtrlClick (env.GetNodeRect (doubleIncrementNode).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (18));
	}

	{ // delete selected nodes
		env.SetNextCommandName (L"Delete Nodes");
		env.RightClick (env.GetNodeRect (doubleIncrementNode).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (19));
	}

	{ // undo delete
		env.ExecuteCommand (CommandCode::Undo);
		ASSERT (env.CheckStepReference (20));
	}

	{ // redo delete
		env.ExecuteCommand (CommandCode::Redo);
		ASSERT (env.CheckStepReference (21));
	}

	{ // undo delete
		env.ExecuteCommand (CommandCode::Undo);
		ASSERT (env.CheckStepReference (22));
	}
}

}
