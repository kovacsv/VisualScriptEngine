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

	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr& group, const NUIE::MenuCommandStructure& commands) override;
	virtual void					OnDoubleClick (NUIE::MouseButton mouseButton, const NUIE::Point& position) override;
	virtual bool					OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessorn, const NUIE::UINodePtr& uiNode) override;
	virtual bool					OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessorn, const NUIE::UINodeGroupPtr& uiGroup) override;

protected:
	HWND							hwnd;
};

using HwndEventHandlerPtr = std::shared_ptr<HwndEventHandler>;
using HwndEventHandlerConstPtr = std::shared_ptr<const HwndEventHandler>;

}

#endif
