#ifndef BI_UINODELAYOUTS_HPP
#define BI_UINODELAYOUTS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"

namespace BI
{

void DrawStatusHeaderWithSlotsLayout (	const NUIE::UINode& uiNode,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodeDrawingImage& drawingImage);

void DrawHeaderWithSlotsAndButtonsLayout (	const NUIE::UINode& uiNode,
											const std::string& leftButtonId,
											const std::wstring& leftButtonText,
											const std::string& rightButtonId,
											const std::wstring& rightButtonText,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage& drawingImage);

}

#endif
