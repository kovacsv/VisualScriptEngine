#include "WAS_HwndEventHandlers.hpp"
#include "WAS_ParameterDialog.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "BI_BuiltInCommands.hpp"

namespace WAS
{

HwndEventHandlers::HwndEventHandlers () :
	NUIE::EventHandlers (),
	control (nullptr)
{

}

void HwndEventHandlers::Init (const NodeEditorHwndBasedControl*	nodeEditorControl)
{
	control = nodeEditorControl;
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	class MyCreateNodeCommand : public BI::CreateNodeCommand
	{
	public:
		MyCreateNodeCommand (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position, const CreatorFunction& creator) :
			BI::CreateNodeCommand (name, uiManager, uiEnvironment, position),
			creator (creator)
		{
		}

		virtual NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition) override
		{
			return creator (modelPosition);
		}

	private:
		CreatorFunction creator;
	};

	const WAS::NodeTree& nodeTree = control->GetNodeTree ();

	NUIE::MenuCommandStructure finalCommands = commands;
	for (const NodeTree::Group& group : nodeTree.GetGroups ()) {
		NUIE::GroupMenuCommandPtr groupCommand (new NUIE::GroupMenuCommand (group.name));
		for (const NodeTree::Item& item : group.items) {
			groupCommand->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (uiManager, uiEnvironment, item.name, position, item.creator)));
		}
		finalCommands.AddCommand (groupCommand);
	}	

	return WAS::SelectCommandFromContextMenu (control->GetEditorHandle (), position, finalCommands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (control->GetEditorHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UIOutputSlotPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (control->GetEditorHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UIInputSlotPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (control->GetEditorHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (control->GetEditorHandle (), position, commands);
}

bool HwndEventHandlers::OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor)
{
	WAS::ParameterDialog paramDialog (paramAccessor);
	return paramDialog.Show (control->GetEditorHandle (), 50, 50);
}

}
