#include "WAS_HwndEventHandler.hpp"
#include "WAS_ParameterDialog.hpp"
#include "WAS_WindowsAppUtils.hpp"

namespace WAS
{

HwndEventHandler::HwndEventHandler () :
	NUIE::EventHandler (),
	hwnd (nullptr)
{

}

HwndEventHandler::~HwndEventHandler ()
{

}

void HwndEventHandler::Init (HWND windowHandle)
{
	hwnd = windowHandle;
}

NUIE::MenuCommandPtr HwndEventHandler::OnContextMenu (NUIE::EventHandler::ContextMenuType, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	return SelectCommandFromContextMenu (hwnd, position, commands);
}

bool HwndEventHandler::OnParameterSettings (NUIE::EventHandler::ParameterSettingsType, NUIE::ParameterInterfacePtr paramAccessor)
{
	ParameterDialog paramDialog (paramAccessor);
	return paramDialog.Show (hwnd, 50, 50);
}

void HwndEventHandler::OnDoubleClick (const NUIE::Point&, NUIE::MouseButton)
{

}

}
