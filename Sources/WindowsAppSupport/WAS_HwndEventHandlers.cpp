#include "WAS_HwndEventHandlers.hpp"
#include "WAS_ParameterDialog.hpp"
#include "WAS_WindowsAppUtils.hpp"

namespace WAS
{

HwndEventHandlers::HwndEventHandlers () :
	NUIE::EventHandlers (),
	control (nullptr)
{

}

HwndEventHandlers::~HwndEventHandlers ()
{

}

void HwndEventHandlers::Init (const NUIE::NativeNodeEditorControl* nodeEditorControl)
{
	control = nodeEditorControl;
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UIInputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandlers::OnContextMenu (NUIE::NodeUIManager&, NUIE::NodeUIEnvironment&, const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

bool HwndEventHandlers::OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor)
{
	WAS::ParameterDialog paramDialog (paramAccessor);
	return paramDialog.Show ((HWND) control->GetEditorNativeHandle (), 50, 50);
}

}
