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

class NodeUITextPanelBase : public NUIE::NodePanel
{
public:
	NodeUITextPanelBase (const std::wstring& nodeText);

	virtual NUIE::Size		GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void			Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

protected:
	virtual NUIE::Color		GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const = 0;
	virtual NUIE::Color		GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const = 0;

private:
	std::wstring nodeText;
};

class NodeUITextPanel : public NodeUITextPanelBase
{
public:
	NodeUITextPanel (const std::wstring& nodeText);

private:
	virtual NUIE::Color		GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual NUIE::Color		GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const override;
};

class NodeUIHeaderPanel : public NodeUITextPanelBase
{
public:
	NodeUIHeaderPanel (const std::wstring& headerText);

private:
	virtual NUIE::Color		GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual NUIE::Color		GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const override;
};

class NodeUIStatusHeaderPanel : public NUIE::NodePanel
{
public:
	enum class NodeStatus
	{
		HasValue,
		HasNoValue
	};

	NodeUIStatusHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	static const int StatusRectSize = 4;

	std::wstring	headerText;
	NodeStatus		nodeStatus;
};

class NodeUIMultiLineTextPanel : public NUIE::NodePanel
{
public:
	NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts,NUIE:: NodeUIDrawingEnvironment& env, size_t textsPerPage);

	virtual NUIE::Size		GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void			Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

protected:
	virtual NUIE::Color		GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const;
	virtual NUIE::Color		GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const;

private:
	std::vector<std::wstring>	nodeTexts;
	NUIE::Size					maxTextSize;
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
									const std::wstring& nodeText,
									NUIE::NodeUIDrawingEnvironment& env);

	virtual NUIE::Size	GetMinSize (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void		Draw (NUIE::NodeUIDrawingEnvironment& env, const NUIE::Rect& rect, NUIE::NodeDrawingImage& drawingImage) const override;

protected:
	static const NUIE::Pen		DefaultButtonBorderPen;
	static const NUIE::Color	DefaultButtonBackgroundColor;

	virtual NUIE::Color		GetTextColor (NUIE::NodeUIDrawingEnvironment& env) const;
	virtual NUIE::Color		GetBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const;

	virtual NUIE::Pen		GetButtonBorderPen (NUIE::NodeUIDrawingEnvironment& env) const;
	virtual NUIE::Color		GetButtonBackgroundColor (NUIE::NodeUIDrawingEnvironment& env) const;

	std::string		leftButtonId;
	std::wstring	leftButtonText;
	std::string		rightButtonId;
	std::wstring	rightButtonText;
	std::wstring	panelText;
	NUIE::Size		leftButtonSize;
	NUIE::Size		panelTextSize;
	NUIE::Size		rightButtonSize;
};

}

#endif
