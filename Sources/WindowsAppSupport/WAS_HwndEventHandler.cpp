#include "WAS_HwndEventHandler.hpp"
#include "WAS_ParameterDialog.hpp"
#include "WAS_WindowsAppUtils.hpp"

namespace WAS
{

HwndEventHandler::HwndEventHandler () :
	NUIE::EventHandler (),
	control (nullptr)
{

}

HwndEventHandler::~HwndEventHandler ()
{

}

void HwndEventHandler::Init (const NUIE::NativeNodeEditorControl* nodeEditorControl)
{
	control = nodeEditorControl;
}

NUIE::MenuCommandPtr HwndEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

NUIE::MenuCommandPtr HwndEventHandler::OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, commands);
}

void HwndEventHandler::OnDoubleClick (NUIE::MouseButton, const NUIE::Point&)
{

}

bool HwndEventHandler::OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodePtr&)
{
	WAS::ParameterDialog paramDialog (paramAccessor);
	return paramDialog.Show ((HWND) control->GetEditorNativeHandle (), 50, 50);
}

bool HwndEventHandler::OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodeGroupPtr&)
{
	WAS::ParameterDialog paramDialog (paramAccessor);
	return paramDialog.Show ((HWND) control->GetEditorNativeHandle (), 50, 50);
}

}
