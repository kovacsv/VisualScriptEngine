#ifndef WAS_HWNDEVENTHANDLER_HPP
#define WAS_HWNDEVENTHANDLER_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include "NUIE_EventHandler.hpp"
#include "WAS_NodeEditorHwndControl.hpp"

namespace WAS
{

class HwndEventHandler : public NUIE::EventHandler
{
public:
	HwndEventHandler ();
	virtual ~HwndEventHandler ();

	void							Init (HWND windowHandle);

	virtual NUIE::MenuCommandPtr	OnContextMenu (NUIE::EventHandler::ContextMenuType type, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual bool					OnParameterSettings (NUIE::EventHandler::ParameterSettingsType, NUIE::ParameterInterfacePtr paramAccessor) override;
	virtual void					OnDoubleClick (const NUIE::Point& position, NUIE::MouseButton mouseButton) override;

protected:
	HWND							hwnd;
};

using HwndEventHandlerPtr = std::shared_ptr<HwndEventHandler>;
using HwndEventHandlerConstPtr = std::shared_ptr<const HwndEventHandler>;

}

#endif
