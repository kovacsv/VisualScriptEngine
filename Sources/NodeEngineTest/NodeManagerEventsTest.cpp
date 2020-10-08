#include "SimpleTest.hpp"
#include "TestUtils.hpp"
#include "BI_BuiltInNodes.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeManagerEventsTest
{

class SelectionChangedCounterEnv : public TestUIEnvironment
{
public:
	SelectionChangedCounterEnv () :
		selectionChangedCount (0)
	{

	}

	virtual void OnSelectionChanged (const Selection&) override
	{
		selectionChangedCount++;
	}

	int selectionChangedCount;
};

TEST (NodeEditorSelectionChangedEventTest)
{
	SelectionChangedCounterEnv env;
	NodeUIManager uiManager (env);
	DBGASSERT (env.selectionChangedCount == 0);

	UINodePtr val1 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (100, 100), 1.0, 1.0)), EmptyEvaluationEnv);
	UINodePtr val2 = uiManager.AddNode (UINodePtr (new DoubleUpDownNode (LocString (L"Value1"), Point (300, 200), 2.0, 1.0)), EmptyEvaluationEnv);
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

}
