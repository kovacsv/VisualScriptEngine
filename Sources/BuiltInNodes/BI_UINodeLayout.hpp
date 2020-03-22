#ifndef BI_UINODELAYOUT_HPP
#define BI_UINODELAYOUT_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodePanelDrawer.hpp"
#include "NUIE_NodeDrawingImage.hpp"

namespace BI
{

class BasicUINode;

class UINodeLayout
{
public:
	UINodeLayout ();
	virtual ~UINodeLayout ();

	virtual void						Draw (	const BasicUINode& uiNode,
												NUIE::NodeUIDrawingEnvironment& env,
												NUIE::NodeDrawingImage& drawingImage) const;

	virtual void						AddPanels (	const BasicUINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const = 0;

	virtual NUIE::EventHandlerResult	HandleMouseClick (	BasicUINode& uiNode,
															NUIE::NodeUIEnvironment& env,
															const NUIE::ModifierKeys& modifierKeys,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface) const = 0;
};

using UINodeLayoutPtr = std::shared_ptr<UINodeLayout>;
using UINodeLayoutConstPtr = std::shared_ptr<const UINodeLayout>;

}

#endif
