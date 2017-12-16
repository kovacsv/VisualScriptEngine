#ifndef VISUALTESTFRAMEWORK_HPP
#define VISUALTESTFRAMEWORK_HPP

#include "EvaluationEnv.hpp"
#include "EventHandlers.hpp"
#include "NodeEditor.hpp"

#include <string>
#include <vector>
#include <sstream>

using namespace NE;
using namespace NUIE;

class SVGBuilder
{
public:
	SVGBuilder ();

	void AddTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes);
	void AddTag (const std::wstring& tag, const std::wstring& content, const std::vector<std::pair<std::wstring, std::wstring>>& attributes);

	void AddOpenTag (const std::wstring& tag, const std::vector<std::pair<std::wstring, std::wstring>>& attributes);
	void AddCloseTag (const std::wstring& tag);

	void Clear ();
	void WriteToFile (const std::string& fileName) const;

	std::wstring GetAsString () const;

	static std::wstring ToString (double val);
	static std::wstring BegToString (double val);
	static std::wstring EndToString (double val);
	static std::wstring PenToStrokeStyle (const Pen& pen);
	static std::wstring ColorToFillStyle (const Color& color);
	static std::wstring FontToFontStyle (const Font& font);
	static std::wstring PointToPath (const Point& point);

private:
	void AddAttributes (const std::vector<std::pair<std::wstring, std::wstring>>& attributes);

	std::wostringstream svgContent;
};

class SVGDrawingContext : public DrawingContext
{
public:
	SVGDrawingContext (double width, double height);

	void				WriteToFile (const std::string& fileName) const;
	std::wstring		GetAsString () const;

	virtual void		Resize (int, int) override;
	virtual double		GetWidth () const override;
	virtual double		GetHeight () const override;
	virtual void		BeginDraw () override;
	virtual void		EndDraw () override;
	virtual void		DrawLine (const Point&, const Point&, const Pen&) override;
	virtual void		DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;
	virtual void		DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void		FillRect (const Rect& rect, const Color& color) override;
	virtual void		DrawEllipse (const Rect&, const Pen&) override;
	virtual void		FillEllipse (const Rect&, const Color&) override;
	virtual void		DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const NUIE::Color& backgroundColor, const NUIE::Color& textColor) override;
	virtual Size		MeasureText (const Font& font, const std::wstring& text) override;

private:
	SVGBuilder	svgBuilder;
	double		width;
	double		height;
};


class TestEventHandlers : public EventHandlers
{
public:
	TestEventHandlers ();

	virtual void		OnRecalculate () override;
	virtual void		OnRedraw () override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const CommandStructure&) override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const CommandStructure&) override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::OutputSlotPtr&, const CommandStructure&) override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::InputSlotPtr&, const CommandStructure&) override;
};

class TestNodeEditorInterface : public NodeEditorInterface
{
public:
	TestNodeEditorInterface ();
	const SVGDrawingContext&	GetSVGDrawingContext () const;
	virtual DrawingContext&		GetDrawingContext () override;
	virtual EventHandlers&		GetEventHandlers () override;
	virtual EvaluationEnv&		GetEvaluationEnv () override;

private:
	SVGDrawingContext	drawingContext;
	TestEventHandlers	eventHandlers;
	EvaluationEnv		evaluationEnv;
};

class NodeEditorTestEnv
{
public:
	NodeEditorTestEnv ();

	bool	CheckReference (const std::string& referenceFileName);
	void	Click (const Point& point);
	void	CtrlClick (const Point& point);
	void	Wheel (MouseWheelRotation rotation, const Point& point);
	void	DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr);

	TestNodeEditorInterface		nodeEditorInterface;
	NodeEditor					nodeEditor;
};

#endif
