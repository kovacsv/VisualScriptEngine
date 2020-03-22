#ifndef BI_UINODELAYOUTS_HPP
#define BI_UINODELAYOUTS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"
#include "BI_UINodeLayout.hpp"

namespace BI
{

class HeaderWithSlotsLayout : public UINodeLayout
{
public:
	HeaderWithSlotsLayout ();

	virtual void						AddPanels (	const NUIE::UINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const override;

	virtual NUIE::EventHandlerResult	HandleMouseClick (	NUIE::UINode& uiNode,
															NUIE::NodeUIEnvironment& env,
															const NUIE::ModifierKeys& modifierKeys,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface) const override;
};

class HeaderWithSlotsAndButtonsLayout : public UINodeLayout
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

	virtual void						AddPanels (	const NUIE::UINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const override;

	virtual NUIE::EventHandlerResult		HandleMouseClick (	NUIE::UINode& uiNode,
																NUIE::NodeUIEnvironment& env,
																const NUIE::ModifierKeys& modifierKeys,
																NUIE::MouseButton mouseButton,
																const NUIE::Point& position,
																NUIE::UINodeCommandInterface& commandInterface) const override;

	virtual std::wstring					GetMiddleText (const NUIE::UINode& uiNode, const NE::StringSettings& stringSettings) const = 0;
	virtual std::shared_ptr<ClickHandler>	GetClickHandler (NUIE::UINode& uiNode) const = 0;

private:
	std::string leftButtonId;
	std::wstring leftButtonText;
	std::string rightButtonId;
	std::wstring rightButtonText;
};

class HeaderWithSlotsAndSwitchLayout : public UINodeLayout
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

	virtual void							AddPanels (	const NUIE::UINode& uiNode,
														NUIE::NodeUIDrawingEnvironment& env,
														NUIE::NodePanelDrawer& drawer) const override;

	virtual NUIE::EventHandlerResult		HandleMouseClick (	NUIE::UINode& uiNode,
																NUIE::NodeUIEnvironment& env,
																const NUIE::ModifierKeys& modifierKeys,
																NUIE::MouseButton mouseButton,
																const NUIE::Point& position,
																NUIE::UINodeCommandInterface& commandInterface) const override;

	virtual SelectedItem					GetSelectedItem (const NUIE::UINode& uiNode) const = 0;
	virtual std::shared_ptr<ClickHandler>	GetClickHandler (NUIE::UINode& uiNode) const = 0;

private:
	std::string switchButtonId;
	std::wstring firstSwitchText;
	std::wstring secondSwitchText;
};

}

#endif
