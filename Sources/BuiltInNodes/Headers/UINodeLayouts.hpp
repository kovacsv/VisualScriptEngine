#ifndef UINODELAYOUTS_HPP
#define UINODELAYOUTS_HPP

#include "UINode.hpp"
#include "NodeUIEnvironment.hpp"
#include "NodeDrawingImage.hpp"

namespace NUIE
{

void DrawStatusHeaderWithSlotsLayout (	const UINode& uiNode,
										NodeUIDrawingEnvironment& env,
										NodeDrawingImage& drawingImage);

void DrawHeaderWithSlotsAndButtonsLayout (	const UINode& uiNode,
											const std::string& leftButtonId,
											const std::wstring& leftButtonText,
											const std::string& rightButtonId,
											const std::wstring& rightButtonText,
											NodeUIDrawingEnvironment& env,
											NodeDrawingImage& drawingImage);

}

#endif
