#ifndef BI_UINODELAYOUTS_HPP
#define BI_UINODELAYOUTS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"

namespace BI
{

class StatusHeaderWithSlotsLayout
{
public:
	StatusHeaderWithSlotsLayout ();

	void Draw (	const NUIE::UINode& uiNode,
				NUIE::NodeUIDrawingEnvironment& env,
				NUIE::NodeDrawingImage& drawingImage) const;
};

class HeaderWithSlotsAndButtonsLayout
{
public:
	class ClickHandler
	{
	public:
		ClickHandler ();
		virtual ~ClickHandler ();

		virtual void LeftButtonClicked () = 0;
		virtual void RightButtonClicked () = 0;
	};

	HeaderWithSlotsAndButtonsLayout (	const std::string& leftButtonId,
										const std::wstring& leftButtonText,
										const std::string& rightButtonId,
										const std::wstring& rightButtonText);

	void						Draw (	const NUIE::UINode& uiNode,
										const std::wstring& middleText,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodeDrawingImage& drawingImage) const;


	NUIE::EventHandlerResult	HandleMouseClick (	const NUIE::UINode& uiNode,
													NUIE::NodeUIEnvironment& env,
													NUIE::MouseButton mouseButton,
													const NUIE::Point& position,
													NUIE::UINodeCommandInterface& commandInterface,
													ClickHandler& clickHandler);

private:
	const std::string leftButtonId;
	const std::wstring leftButtonText;
	const std::string rightButtonId;
	const std::wstring rightButtonText;
};

class HeaderWithSlotsAndSwitchLayout
{
public:
	enum class SelectedItem
	{
		First,
		Second
	};

	class ClickHandler
	{
	public:
		ClickHandler ();
		virtual ~ClickHandler ();

		virtual void SwitchClicked () = 0;
	};

	HeaderWithSlotsAndSwitchLayout (const std::string& switchButtonId,
									const std::wstring& firstSwitchText,
									const std::wstring& secondSwitchText);

	void						Draw (	const NUIE::UINode& uiNode,
										SelectedItem selectedItem,
										NUIE::NodeUIDrawingEnvironment& env,
										NUIE::NodeDrawingImage& drawingImage) const;


	NUIE::EventHandlerResult	HandleMouseClick (	const NUIE::UINode& uiNode,
													NUIE::NodeUIEnvironment& env,
													NUIE::MouseButton mouseButton,
													const NUIE::Point& position,
													NUIE::UINodeCommandInterface& commandInterface,
													ClickHandler& clickHandler);

private:
	const std::string switchButtonId;
	const std::wstring firstSwitchText;
	const std::wstring secondSwitchText;
};

}

#endif
