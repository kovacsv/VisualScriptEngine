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

bool HwndEventHandler::OnParameterSettings (NUIE::EventHandler::ParameterSettingsType type, NUIE::ParameterInterfacePtr paramAccessor)
{
	ParameterDialog paramDialog (paramAccessor, hwnd);
	std::wstring title;
	if (type == NUIE::EventHandler::ParameterSettingsType::Node) {
		title = NE::LocalizeString (L"Node Parameters");
	} else if (type == NUIE::EventHandler::ParameterSettingsType::Group) {
		title = NE::LocalizeString (L"Group Parameters");
	} else {
		DBGBREAK ();
	}
	return paramDialog.Show (title, 0, 0);
}

void HwndEventHandler::OnDoubleClick (const NUIE::Point&, NUIE::MouseButton)
{

}

}
