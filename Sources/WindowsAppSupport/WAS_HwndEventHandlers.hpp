#ifndef WAS_HWNDEVENTHANDLERS_HPP
#define WAS_HWNDEVENTHANDLERS_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include "NUIE_EventHandlers.hpp"
#include "WAS_NodeEditorHwndControl.hpp"

namespace WAS
{

class HwndEventHandlers : public NUIE::EventHandlers
{
public:
	HwndEventHandlers ();

	void								Init (const NodeEditorHwndBasedControl*	nodeEditorControl);

	virtual NUIE::MenuCommandPtr		OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr		OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr		OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UIOutputSlotPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr		OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UIInputSlotPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr		OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodeGroupPtr& group, const NUIE::MenuCommandStructure& commands) override;
	virtual bool						OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor) override;

protected:
	const NodeEditorHwndBasedControl*	control;
};

}

#endif
