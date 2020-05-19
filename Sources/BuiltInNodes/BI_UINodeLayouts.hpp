#ifndef BI_UINODELAYOUTS_HPP
#define BI_UINODELAYOUTS_HPP

#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"
#include "BI_UINodeLayout.hpp"
#include "BI_BasicUINode.hpp"

namespace BI
{

class HeaderBasedLayout : public UINodeLayout
{
public:
	HeaderBasedLayout ();

	virtual void	AddPanels (	const BasicUINode& uiNode,
								NUIE::NodeUIDrawingEnvironment& env,
								NUIE::NodePanelDrawer& drawer) const override;
};

class HeaderWithSlotsLayout : public HeaderBasedLayout
{
public:
	HeaderWithSlotsLayout ();

	virtual void						AddPanels (	const BasicUINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const override;

	virtual NUIE::EventHandlerResult	HandleMouseClick (	BasicUINode& uiNode,
															NUIE::NodeUIEnvironment& env,
															const NUIE::ModifierKeys& modifierKeys,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface) const override;
};

class HeaderWithSlotsAndTextLayout : public HeaderBasedLayout
{
public:
	HeaderWithSlotsAndTextLayout ();

	virtual void						AddPanels (	const BasicUINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const override;


	virtual NUIE::EventHandlerResult	HandleMouseClick (	BasicUINode& uiNode,
															NUIE::NodeUIEnvironment& env,
															const NUIE::ModifierKeys& modifierKeys,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface) const override;

	virtual std::wstring				GetText (const BasicUINode& uiNode, const NE::StringConverter& stringConverter) const = 0;
};

class HeaderWithSlotsAndButtonsLayout : public HeaderBasedLayout
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

	virtual void						AddPanels (	const BasicUINode& uiNode,
													NUIE::NodeUIDrawingEnvironment& env,
													NUIE::NodePanelDrawer& drawer) const override;

	virtual NUIE::EventHandlerResult	HandleMouseClick (	BasicUINode& uiNode,
															NUIE::NodeUIEnvironment& env,
															const NUIE::ModifierKeys& modifierKeys,
															NUIE::MouseButton mouseButton,
															const NUIE::Point& position,
															NUIE::UINodeCommandInterface& commandInterface) const override;

	virtual std::wstring					GetMiddleText (const BasicUINode& uiNode, const NE::StringConverter& stringConverter) const = 0;
	virtual std::shared_ptr<ClickHandler>	GetClickHandler (BasicUINode& uiNode) const = 0;

private:
	std::string		leftButtonId;
	std::wstring	leftButtonText;
	std::string		rightButtonId;
	std::wstring	rightButtonText;
};

class HeaderWithSlotsAndSwitchLayout : public HeaderBasedLayout
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

	virtual void							AddPanels (	const BasicUINode& uiNode,
														NUIE::NodeUIDrawingEnvironment& env,
														NUIE::NodePanelDrawer& drawer) const override;

	virtual NUIE::EventHandlerResult		HandleMouseClick (	BasicUINode& uiNode,
																NUIE::NodeUIEnvironment& env,
																const NUIE::ModifierKeys& modifierKeys,
																NUIE::MouseButton mouseButton,
																const NUIE::Point& position,
																NUIE::UINodeCommandInterface& commandInterface) const override;

	virtual SelectedItem					GetSelectedItem (const BasicUINode& uiNode) const = 0;
	virtual std::shared_ptr<ClickHandler>	GetClickHandler (BasicUINode& uiNode) const = 0;

private:
	std::string switchButtonId;
	std::wstring firstSwitchText;
	std::wstring secondSwitchText;
};

class HeaderWithSlotsAndMultilineTextLayout : public HeaderBasedLayout
{
public:
	HeaderWithSlotsAndMultilineTextLayout (	const std::string& leftButtonId,
											const std::wstring& leftButtonText,
											const std::string& rightButtonId,
											const std::wstring& rightButtonText);

	virtual void							AddPanels (	const BasicUINode& uiNode,
														NUIE::NodeUIDrawingEnvironment& env,
														NUIE::NodePanelDrawer& drawer) const override;


	virtual NUIE::EventHandlerResult		HandleMouseClick (	BasicUINode& uiNode,
																NUIE::NodeUIEnvironment& env,
																const NUIE::ModifierKeys& modifierKeys,
																NUIE::MouseButton mouseButton,
																const NUIE::Point& position,
																NUIE::UINodeCommandInterface& commandInterface) const override;

	virtual void							GetTextInfo (	const BasicUINode& uiNode,
															const NE::StringConverter& stringConverter,
															std::vector<std::wstring>& texts,
															size_t& textsPerPage) const = 0;

private:
	std::string		leftButtonId;
	std::wstring	leftButtonText;
	std::string		rightButtonId;
	std::wstring	rightButtonText;

protected:
	mutable size_t	pageCount;
	mutable size_t	currentPage;
};

}

#endif
