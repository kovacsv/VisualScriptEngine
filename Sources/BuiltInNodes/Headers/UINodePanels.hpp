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

	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const override;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

protected:
	virtual Color	GetTextColor (NodeUIDrawingEnvironment& env) const = 0;
	virtual Color	GetBackgroundColor (NodeUIDrawingEnvironment& env) const = 0;

private:
	std::wstring nodeText;
};

class NodeUITextPanel : public NodeUITextPanelBase
{
public:
	NodeUITextPanel (const std::wstring& nodeText);

private:
	virtual Color	GetTextColor (NodeUIDrawingEnvironment& env) const override;
	virtual Color	GetBackgroundColor (NodeUIDrawingEnvironment& env) const override;
};

class NodeUIHeaderPanel : public NodeUITextPanelBase
{
public:
	NodeUIHeaderPanel (const std::wstring& headerText);

private:
	virtual Color	GetTextColor (NodeUIDrawingEnvironment& env) const override;
	virtual Color	GetBackgroundColor (NodeUIDrawingEnvironment& env) const override;
};

class NodeUIStatusHeaderPanel : public NodeUIPanel
{
public:
	enum class NodeStatus
	{
		HasValue,
		HasNoValue
	};

	NodeUIStatusHeaderPanel (const std::wstring& headerText, NodeStatus nodeStatus);

	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const override;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

private:
	static const int StatusRectSize = 4;

	std::wstring	headerText;
	NodeStatus		nodeStatus;
};

class NodeUIMultiLineTextPanel : public NodeUIPanel
{
public:
	NodeUIMultiLineTextPanel (const std::vector<std::wstring>& nodeTexts, NodeUIDrawingEnvironment& env, size_t textsPerPage, size_t currentPage);

	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const override;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

protected:
	virtual Color	GetTextColor (NodeUIDrawingEnvironment& env) const;
	virtual Color	GetBackgroundColor (NodeUIDrawingEnvironment& env) const;

private:
	std::vector<std::wstring>	nodeTexts;
	Size						maxTextSize;
	size_t						textsPerPage;
	size_t						currentPage;
};

class NodeUISlotPanel : public NodeUIPanel
{
public:
	NodeUISlotPanel (const UINode& node, NodeUIDrawingEnvironment& env);

	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const override;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

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
									NodeUIDrawingEnvironment& env);

	virtual Size	GetMinSize (NodeUIDrawingEnvironment& env) const override;
	virtual void	Draw (NodeUIDrawingEnvironment& env, const Rect& rect, NodeDrawingImage& drawingImage) const override;

protected:
	static const Pen	DefaultButtonBorderPen;
	static const Color	DefaultButtonBackgroundColor;

	virtual Color	GetTextColor (NodeUIDrawingEnvironment& env) const;
	virtual Color	GetBackgroundColor (NodeUIDrawingEnvironment& env) const;

	virtual Pen		GetButtonBorderPen (NodeUIDrawingEnvironment& env) const;
	virtual Color	GetButtonBackgroundColor (NodeUIDrawingEnvironment& env) const;

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
