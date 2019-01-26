#include "WAS_HwndEventHandlers.hpp"
#include "WAS_ParameterDialog.hpp"
#include "WAS_WindowsAppUtils.hpp"

namespace WAS
{

HwndEventHandlers::HwndEventHandlers () :
	NUIE::EventHandlers (),
	hwnd (NULL)
{

}

void HwndEventHandlers::Init (HWND newHwnd)
{
	hwnd = newHwnd;
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UIOutputSlotPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UIInputSlotPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu (hwnd, position, commands);
}

bool HwndEventHandlers::OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor)
{
	WAS::ParameterDialog paramDialog (paramAccessor);
	return paramDialog.Show (hwnd, 50, 50);
}


}
