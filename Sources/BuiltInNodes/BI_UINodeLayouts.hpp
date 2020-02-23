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
											const std::wstring& middleText,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage& drawingImage);

void DrawHeaderWithSlotsAndSwitchLayout (	const NUIE::UINode& uiNode,
											const std::string& switchButtonId,
											const std::wstring& leftSwitchText,
											const std::wstring& rightSwitchText,
											short selectedIndex,
											NUIE::NodeUIDrawingEnvironment& env,
											NUIE::NodeDrawingImage& drawingImage);

class ButtonClickHandler
{
public:
	ButtonClickHandler ();
	virtual ~ButtonClickHandler ();

	virtual void LeftButtonClicked () = 0;
	virtual void RightButtonClicked () = 0;
};

class SwitchClickHandler
{
public:
	SwitchClickHandler ();
	virtual ~SwitchClickHandler ();

	virtual void SwitchClicked () = 0;
};

NUIE::EventHandlerResult HandleMouseClickOnButtonsLayout (	const NUIE::UINode& uiNode,
															const std::string& leftButtonId,
															const std::string& rightButtonId,
															NUIE::NodeUIEnvironment& env,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface,
															ButtonClickHandler& clickHandler);

NUIE::EventHandlerResult HandleMouseClickOnSwitchLayout (	const NUIE::UINode& uiNode,
															const std::string& switchButtonId,
															NUIE::NodeUIEnvironment& env,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface,
															SwitchClickHandler& clickHandler);

}

#endif
