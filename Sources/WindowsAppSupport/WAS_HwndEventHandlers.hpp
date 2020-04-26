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
	virtual ~HwndEventHandlers ();

	void									Init (const NUIE::NativeNodeEditorControl* nodeEditorControl);

	virtual NUIE::MenuCommandPtr			OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr			OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr			OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr			OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr			OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr& group, const NUIE::MenuCommandStructure& commands) override;
	virtual void							OnDoubleClick (const NUIE::Point& position) override;
	virtual bool							OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor) override;

protected:
	const NUIE::NativeNodeEditorControl*	control;
};

using HwndEventHandlersPtr = std::shared_ptr<HwndEventHandlers>;
using HwndEventHandlersConstPtr = std::shared_ptr<const HwndEventHandlers>;

}

#endif
