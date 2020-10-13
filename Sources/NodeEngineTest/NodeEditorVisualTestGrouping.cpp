#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestGrouping
{

TEST (GroupingVisualTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"Grouping_Initial.svg"));

	{
		env.Click (env.doubleInputHeaderPoint);
		env.ExecuteCommand (CommandCode::Group);
		ASSERT (env.CheckReference (L"Grouping_OneNodeGrouped.svg"));
	}

	{
		env.Click (env.rangeInputHeaderPoint);
		env.ExecuteCommand (CommandCode::Group);
		ASSERT (env.CheckReference (L"Grouping_TwoNodesGrouped.svg"));
	}

	{
		env.Click (env.viewer1HeaderPoint);
		env.CtrlClick (env.viewer2HeaderPoint);
		env.ExecuteCommand (CommandCode::Group);
		ASSERT (env.CheckReference (L"Grouping_AllNodesGrouped.svg"));
	}

	{
		Point groupPoint = env.viewer1HeaderPoint - Point (0.0, 5.5);
		env.DragDrop (EmptyModifierKeys, groupPoint, groupPoint + Point (50.0, 0.0));
		ASSERT (env.CheckReference (L"Grouping_GroupMoved.svg"));
		env.RecalcPositions ();
	}

	{
		env.Click (env.viewer1HeaderPoint);
		env.ExecuteCommand (CommandCode::Ungroup);
		ASSERT (env.CheckReference (L"Grouping_Viewer1Ungrouped.svg"));
	}
	
	{
		env.Click (env.viewer2HeaderPoint);
		env.ExecuteCommand (CommandCode::Ungroup);
		ASSERT (env.CheckReference (L"Grouping_Viewer2Ungrouped.svg"));
	}
}

}
