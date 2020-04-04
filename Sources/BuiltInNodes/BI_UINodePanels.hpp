#ifndef BI_UINODEPANELS_HPP
#define BI_UINODEPANELS_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeDrawingImage.hpp"
#include "NUIE_NodePanelDrawer.hpp"
#include "NUIE_UINode.hpp"
#include <memory>

namespace BI
{

class SlotRectCollection
{
public:
	enum class RefPointMode
	{
		TopLeft,
		TopRight
	};

	SlotRectCollection ();

	void		AddSlotSize (const NE::SlotId& slotId, const NUIE::Size& size);
	double		GetWidth () const;
	double		GetHeight (double padding) const;
	void		Enumerate (RefPointMode refPointMode, const NUIE::Point& refPoint, double padding, const std::function<void (const NE::SlotId&, const NUIE::Rect&)>& processor) const;

private:
	std::vector<std::pair<NE::SlotId, NUIE::Size>> slotSizes;
};

class NodeUIHeaderPanel : public NUIE::NodePanel
{
public:
	enum class NodeStatus
	{
		HasValue,
		HasNoValue
	};

	NodeUIHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus);

	virtual NUIE::Size			GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void				Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

protected:
	const NUIE::Font&			GetTextFont (NUIE::NodeUIDrawingEnvironment& env) const;
	const NUIE::Color&			GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const;
	const NUIE::Color&			GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const;

	std::wstring	headerText;
	NodeStatus		nodeStatus;
};

class NodeUITextPanel : public NUIE::NodePanel
{
public:
	NodeUITextPanel (const std::wstring& text);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

protected:
	std::wstring	text;
};

class NodeUIIconHeaderPanel : public NodeUIHeaderPanel
{
public:
	NodeUIIconHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus, const NUIE::IconId& iconId, NUIE::NodeUIDrawingEnvironment& env);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	NUIE::IconId	iconId;
	NUIE::Size		textSize;
	double			iconSize;
	double			minWidth;
};

class NodeUIMultiLineTextPanel : public NUIE::NodePanel
{
public:
	NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts, size_t allTextCount, size_t textsPerPage, NUIE::NodeUIDrawingEnvironment& env);

	virtual NUIE::Size		GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void			Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	const NUIE::Color&		GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const;
	const NUIE::Color&		GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const;

	std::vector<std::wstring>	nodeTexts;
	NUIE::Size					maxTextSize;
	size_t						allTextCount;
	size_t						textsPerPage;
};

class NodeUISlotPanel : public NUIE::NodePanel
{
public:
	NodeUISlotPanel (const NUIE::UINode& node, NUIE::NodeUIDrawingEnvironment& env);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	const NUIE::UINode&		node;
	SlotRectCollection		inputSlots;
	SlotRectCollection		outputSlots;
};

class NodeUILeftRightButtonsPanel : public NUIE::NodePanel
{
public:
	NodeUILeftRightButtonsPanel (	const std::string& leftButtonId,
									const std::wstring& leftButtonText,
									const std::string& rightButtonId,
									const std::wstring& rightButtonText,
									const std::wstring& panelText,
									NUIE::NodeUIDrawingEnvironment& env);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	std::string		leftButtonId;
	std::wstring	leftButtonText;
	std::string		rightButtonId;
	std::wstring	rightButtonText;
	std::wstring	panelText;
	NUIE::Size		leftButtonSize;
	NUIE::Size		panelTextSize;
	NUIE::Size		rightButtonSize;
};

class NodeUIButtonPanel : public NUIE::NodePanel
{
public:
	NodeUIButtonPanel (	const std::string& buttonRectId,
						const std::wstring& buttonText,
						NUIE::NodeUIDrawingEnvironment& env);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	std::string		buttonRectId;
	std::wstring	buttonText;
	NUIE::Size		buttonSize;
};

}

#endif
