#ifndef UINODEPANELS_HPP
#define UINODEPANELS_HPP

#include "NodeUIEnvironment.hpp"
#include "NodeDrawingImage.hpp"
#include "NodeUIPanelDrawer.hpp"
#include "UINode.hpp"
#include <memory>

namespace NUIE
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

	void		AddSlotSize (const NE::SlotId& slotId, const Size& size);
	double		GetWidth () const;
	double		GetHeight (double padding) const;
	void		Enumerate (RefPointMode refPointMode, const Point& refPoint, double padding, const std::function<void (const NE::SlotId&, const Rect&)>& processor) const;

private:
	std::vector<std::pair<NE::SlotId, Size>> slotSizes;
};

class NodeUITextPanelBase : public NodeUIPanel
{
public:
	NodeUITextPanelBase (const std::wstring& nodeText);

	virtual Size	GetMinSize (NodeUIEnvironment& env) const override;
	virtual void	Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

protected:
	virtual Color	GetTextColor (NodeUIEnvironment& env) const = 0;
	virtual Color	GetBackgroundColor (NodeUIEnvironment& env) const = 0;

private:
	std::wstring nodeText;
};

class NodeUITextPanel : public NodeUITextPanelBase
{
public:
	NodeUITextPanel (const std::wstring& nodeText);

private:
	virtual Color	GetTextColor (NodeUIEnvironment& env) const override;
	virtual Color	GetBackgroundColor (NodeUIEnvironment& env) const override;
};

class NodeUIHeaderPanel : public NodeUITextPanelBase
{
public:
	NodeUIHeaderPanel (const std::wstring& headerText);

private:
	virtual Color	GetTextColor (NodeUIEnvironment& env) const override;
	virtual Color	GetBackgroundColor (NodeUIEnvironment& env) const override;
};

class NodeUIStatusHeaderPanel : public NodeUIPanel
{
public:
	NodeUIStatusHeaderPanel (const std::wstring& headerText, UINode::Status nodeStatus);

	virtual Size	GetMinSize (NodeUIEnvironment& env) const override;
	virtual void	Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

private:
	static const int StatusRectSize = 4;

	std::wstring	headerText;
	UINode::Status	nodeStatus;
};

class NodeUIMultiLineTextPanel : public NodeUIPanel
{
public:
	NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts, NodeUIEnvironment& env, size_t textsPerPage, size_t currentPage);

	virtual Size	GetMinSize (NodeUIEnvironment& env) const override;
	virtual void	Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

protected:
	virtual Color	GetTextColor (NodeUIEnvironment& env) const;
	virtual Color	GetBackgroundColor (NodeUIEnvironment& env) const;

private:
	std::vector<std::wstring>	nodeTexts;
	Size						maxTextSize;
	size_t						textsPerPage;
	size_t						currentPage;
};

class NodeUISlotPanel : public NodeUIPanel
{
public:
	NodeUISlotPanel (const UINode& node, NodeUIEnvironment& env);

	virtual Size	GetMinSize (NodeUIEnvironment& env) const override;
	virtual void	Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

private:
	const UINode&		node;
	SlotRectCollection	inputSlots;
	SlotRectCollection	outputSlots;
};

class NodeUILeftRightButtonsPanel : public NodeUIPanel
{
public:
	NodeUILeftRightButtonsPanel (	const std::string& leftButtonId,
									const std::wstring& leftButtonText,
									const std::string& rightButtonId,
									const std::wstring& rightButtonText,
									const std::wstring& nodeText,
									NodeUIEnvironment& env);

	virtual Size	GetMinSize (NodeUIEnvironment& env) const override;
	virtual void	Draw (NodeUIEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

protected:
	static const Pen	DefaultButtonBorderPen;
	static const Color	DefaultButtonBackgroundColor;

	virtual Color	GetTextColor (NodeUIEnvironment& env) const;
	virtual Color	GetBackgroundColor (NodeUIEnvironment& env) const;

	virtual Pen		GetButtonBorderPen (NodeUIEnvironment& env) const;
	virtual Color	GetButtonBackgroundColor (NodeUIEnvironment& env) const;

	std::string		leftButtonId;
	std::wstring	leftButtonText;
	std::string		rightButtonId;
	std::wstring	rightButtonText;
	std::wstring	nodeText;
	Size			leftButtonSize;
	Size			nodeTextSize;
	Size			rightButtonSize;
};

}

#endif
