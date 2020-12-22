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

class NodeFinderByName : public NodeFinder
{
public:
	NodeFinderByName (const std::wstring& nodeName) :
		NodeFinder (),
		nodeName (nodeName)
	{

	}

	virtual bool IsMatch (const UINodeConstPtr& uiNode) const override
	{
		return uiNode->GetName ().GetLocalized () == nodeName;
	}

private:
	std::wstring nodeName;
};

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

	UINodeConstPtr GetNode (const std::wstring& name)
	{
		NodeFinderByName finder (name);
		std::vector<UINodeConstPtr> nodes = nodeEditor.FindNodes (finder);
		if (DBGERROR (nodes.size () != 1)) {
			return nullptr;
		}
		return nodes[0];
	}

	void SetParameter (ParameterInterfacePtr paramInterface, std::wstring name, ValueConstPtr val)
	{
		for (size_t i = 0; i < paramInterface->GetParameterCount (); i++) {
			if (paramInterface->GetParameterName (i) == name) {
				paramInterface->SetParameterValue (i, val);
				break;
			}
		}
	}
};

TEST (WorkflowTest)
{
	WorkflowTestEnv env (GetDefaultSkinParams ());

	Point padding (10.0, 10.0);

	{ // start with an empty canvas
		ASSERT (env.CheckStepReference (1));
	}

	{ // add double incremented node
		env.nodeEditor.AddNode (UINodePtr (new DoubleIncrementedNode (LocString (L"Numbers"), Point (100.0, 150.0))));
		ASSERT (env.CheckStepReference (2));
	}

	{ // add double node
		env.nodeEditor.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Number"), Point (150.0, 300.0), 0, 1)));
		ASSERT (env.CheckStepReference (3));
	}

	{ // add division node
		env.nodeEditor.AddNode (UINodePtr (new DivisionNode (LocString (L"Division"), Point (350.0, 150.0))));
		ASSERT (env.CheckStepReference (4));
	}

	{ // connect double incremented to division
		env.DragDrop (
			env.GetOutputSlotConnPosition (env.GetNode (L"Numbers"), "out"),
			env.GetInputSlotConnPosition (env.GetNode (L"Division"), "a")
		);
		ASSERT (env.CheckStepReference (5));
	}

	{ // connect double to division
		env.DragDrop (
			env.GetOutputSlotConnPosition (env.GetNode (L"Number"), "out"),
			env.GetInputSlotConnPosition (env.GetNode (L"Division"), "b")
		);
		ASSERT (env.CheckStepReference (6));
	}

	{ // increase number twice
		Point plusButtonCenter = env.GetNode (L"Number")->GetSpecialRect (env.uiEnvironment, "plus").GetCenter ();
		env.Click (plusButtonCenter);
		env.Click (plusButtonCenter);
		ASSERT (env.CheckStepReference (7));
	}

	{ // add viewer node
		env.nodeEditor.AddNode (UINodePtr (new MultiLineViewerNode (LocString (L"Viewer 1"), Point (600.0, 150.0), 5)));
		ASSERT (env.CheckStepReference (8));
	}

	{ // add another viewer node
		env.nodeEditor.AddNode (UINodePtr (new MultiLineViewerNode (LocString (L"Viewer 2"), Point (600.0, 450.0), 5)));
		ASSERT (env.CheckStepReference (9));
	}

	{ // connect division to viewer 1
		env.DragDrop (
			env.GetOutputSlotConnPosition (env.GetNode (L"Division"), "result"),
			env.GetInputSlotConnPosition (env.GetNode (L"Viewer 1"), "in")
		);
		ASSERT (env.CheckStepReference (10));
	}

	{ // connect division to viewer 2
		env.DragDrop (
			env.GetOutputSlotConnPosition (env.GetNode (L"Division"), "result"),
			env.GetInputSlotConnPosition (env.GetNode (L"Viewer 2"), "in")
		);
		ASSERT (env.CheckStepReference (11));
	}

	{ // increase number again
		Point plusButtonCenter = env.GetNode (L"Number")->GetSpecialRect (env.uiEnvironment, "plus").GetCenter ();
		env.Click (plusButtonCenter);
		ASSERT (env.CheckStepReference (12));
	}

	{ // select number nodes
		env.DragDrop (
			env.GetNodeRect (env.GetNode (L"Numbers")).GetTopLeft () - padding,
			env.GetNodeRect (env.GetNode (L"Number")).GetBottomRight () + padding
		);
		ASSERT (env.CheckStepReference (13));
	}

	{ // align number nodes to the right
		env.SetNextCommandName (L"Horizontal Right");
		env.RightClick (env.GetNodeRect (env.GetNode (L"Numbers")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (14));
	}

	{ // group selected nodes
		env.SetNextCommandName (L"Create New Group");
		env.RightClick (env.GetNodeRect (env.GetNode (L"Numbers")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (15));
	}

	{ // select viewer node 2
		env.DragDrop (
			env.GetNodeRect (env.GetNode (L"Viewer 2")).GetTopLeft () - padding,
			env.GetNodeRect (env.GetNode (L"Viewer 2")).GetBottomRight () + padding
		);
		ASSERT (env.CheckStepReference (16));
	}

	{ // group viewer node 2
		env.SetNextCommandName (L"Create New Group");
		env.RightClick (env.GetNodeRect (env.GetNode (L"Viewer 2")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (17));
	}

	{ // select double incremented node next to viewer 2
		env.CtrlClick (env.GetNodeRect (env.GetNode (L"Numbers")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (18));
	}

	{ // delete selected nodes
		env.SetNextCommandName (L"Delete Nodes");
		env.RightClick (env.GetNodeRect (env.GetNode (L"Numbers")).GetTopLeft () + padding);
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

	{ // select viewer node 1 parameters
		env.SetNextCommandNodeParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Node Name", ValueConstPtr (new StringValue (L"Renamed 1")));
			return true;
		});
		env.RightClick (env.GetNodeRect (env.GetNode (L"Viewer 1")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (23));
	}

	{ // select viewer node 2 parameters
		env.SetNextCommandNodeParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Node Name", ValueConstPtr (new StringValue (L"Renamed 2")));
			return true;
		});
		env.RightClick (env.GetNodeRect (env.GetNode (L"Viewer 2")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (24));
	}

	{ // select viewer nodes
		env.DragDrop (
			env.GetNodeRect (env.GetNode (L"Renamed 1")).GetTopLeft () - padding,
			env.GetNodeRect (env.GetNode (L"Renamed 2")).GetBottomRight () + padding
		);
		ASSERT (env.CheckStepReference (25));
	}

	{ // modify texts per page
		env.SetNextCommandNodeParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Texts per page", ValueConstPtr (new IntValue (3)));
			return true;
		});
		env.RightClick (env.GetNodeRect (env.GetNode (L"Renamed 1")).GetTopLeft () + padding);
		ASSERT (env.CheckStepReference (26));
	}

	{ // clear selection
		env.ExecuteCommand (CommandCode::Escape);
		ASSERT (env.CheckStepReference (27));
	}

	{ // modify group name
		env.SetNextCommandGroupParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Group Name", ValueConstPtr (new StringValue (L"Group 1")));
			return true;
		});
		env.RightClick (env.GetNodeRect (env.GetNode (L"Numbers")).GetTopLeft () - Point (10.0, 0.0));
		ASSERT (env.CheckStepReference (28));
	}
}

}
