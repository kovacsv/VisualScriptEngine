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
	NUIE::MenuCommandStructure finalCommands = commands;
	AddNodeTreeAsCommands (control->GetNodeTree (), uiManager, uiEnvironment, position, finalCommands);
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
