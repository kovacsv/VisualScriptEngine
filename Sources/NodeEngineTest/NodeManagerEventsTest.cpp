#include "SimpleTest.hpp"
#include "TestUtils.hpp"
#include "BI_BuiltInNodes.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeManagerEventsTest
{

class OnChangedCounterEnv : public TestUIEnvironment
{
public:
	OnChangedCounterEnv () :
		selectionChangedCount (0),
		undoStateChangedCount (0),
		clipboardStateChangedCount (0)
	{

	}

	virtual void OnSelectionChanged (const Selection&) override
	{
		selectionChangedCount++;
	}

	virtual void OnUndoStateChanged (const UndoState&) override
	{
		undoStateChangedCount++;
	}

	virtual void OnClipboardStateChanged (const ClipboardState&) override
	{
		clipboardStateChangedCount++;
	}

	int selectionChangedCount;
	int undoStateChangedCount;
	int clipboardStateChangedCount;
};

TEST (NodeEditorOnSelectionChangedEventTest)
{
	OnChangedCounterEnv env;
	NodeUIManager uiManager (env);
	DBGASSERT (env.selectionChangedCount == 0);

	UINodePtr val1 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (100, 100), 1.0, 1.0)));
	UINodePtr val2 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (300, 200), 2.0, 1.0)));
	DBGASSERT (env.selectionChangedCount == 0);

	Selection newSelection;
	newSelection.SetNodes (NE::NodeCollection ({ val1->GetId () }));
	uiManager.SetSelection (newSelection, env);
	DBGASSERT (env.selectionChangedCount == 1);

	uiManager.DeleteNode (val1->GetId (), env.GetEvaluationEnv (), env);
	DBGASSERT (env.selectionChangedCount == 2);

	newSelection.SetNodes (NE::NodeCollection ({ val2->GetId () }));
	uiManager.SetSelection (newSelection, env);
	DBGASSERT (env.selectionChangedCount == 3);

	uiManager.New (env);
	DBGASSERT (env.selectionChangedCount == 4);
}

TEST (NodeEditorOnUndoStateChangedEventTest)
{
	OnChangedCounterEnv env;
	NodeUIManager uiManager (env);
	DBGASSERT (env.undoStateChangedCount == 0);

	UINodePtr val1 (new DoubleUpDownNode (LocString (L"Value1"), Point (100, 100), 1.0, 1.0));
	AddNodeCommand addNode1 (val1);
	uiManager.ExecuteCommand (addNode1, env);
	DBGASSERT (env.undoStateChangedCount == 1);

	UINodePtr val2 (new DoubleUpDownNode (LocString (L"Value2"), Point (300, 200), 1.0, 1.0));
	AddNodeCommand addNode2 (val2);
	uiManager.ExecuteCommand (addNode2, env);
	DBGASSERT (env.undoStateChangedCount == 2);

	UndoCommand uc1 (env);
	uiManager.ExecuteCommand (uc1, env);
	DBGASSERT (env.undoStateChangedCount == 3);

	UndoCommand uc2 (env);
	uiManager.ExecuteCommand (uc2, env);
	DBGASSERT (env.undoStateChangedCount == 4);

	UndoCommand uc3 (env);
	uiManager.ExecuteCommand (uc3, env);
	DBGASSERT (env.undoStateChangedCount == 4);

	RedoCommand rc1 (env);
	uiManager.ExecuteCommand (rc1, env);
	DBGASSERT (env.undoStateChangedCount == 5);

	UndoCommand uc4 (env);
	uiManager.ExecuteCommand (uc4, env);
	DBGASSERT (env.undoStateChangedCount == 6);

	UndoCommand uc5 (env);
	uiManager.ExecuteCommand (uc5, env);
	DBGASSERT (env.undoStateChangedCount == 6);
}

TEST (NodeEditorOnClipboardStateChangedEventTest)
{
	OnChangedCounterEnv env;
	NodeUIManager uiManager (env);
	DBGASSERT (env.clipboardStateChangedCount == 0);

	UINodePtr val1 (new DoubleUpDownNode (LocString (L"Value1"), Point (100, 100), 1.0, 1.0));
	AddNodeCommand addNode1 (val1);
	uiManager.ExecuteCommand (addNode1, env);
	DBGASSERT (env.clipboardStateChangedCount == 0);

	UINodePtr val2 (new DoubleUpDownNode (LocString (L"Value2"), Point (300, 200), 1.0, 1.0));
	AddNodeCommand addNode2 (val2);
	uiManager.ExecuteCommand (addNode2, env);
	DBGASSERT (env.clipboardStateChangedCount == 0);

	NE::NodeCollection val1Coll ({ val1->GetId () });
	CopyNodesCommand copyCommand1 (env, val1Coll);
	uiManager.ExecuteCommand (copyCommand1, env);
	DBGASSERT (env.clipboardStateChangedCount == 1);

	NE::NodeCollection val2Coll ({ val2->GetId () });
	CopyNodesCommand copyCommand2 (env, val2Coll);
	uiManager.ExecuteCommand (copyCommand2, env);
	DBGASSERT (env.clipboardStateChangedCount == 1);
}

}
