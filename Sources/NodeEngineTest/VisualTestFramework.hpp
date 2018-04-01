#ifndef VISUALTESTFRAMEWORK_HPP
#define VISUALTESTFRAMEWORK_HPP

#include "EvaluationEnv.hpp"
#include "EventHandlers.hpp"
#include "NodeEditor.hpp"
#include "SvgDrawingContext.hpp"

using namespace NE;
using namespace NUIE;

class TestEventHandlers : public EventHandlers
{
public:
	TestEventHandlers ();

	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const CommandStructure&) override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const CommandStructure&) override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::OutputSlotPtr&, const CommandStructure&) override;
	virtual CommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::InputSlotPtr&, const CommandStructure&) override;
	virtual bool		OnParameterSettings (NodeParameterAccessorPtr) override;

	void				SetNextCommandName (const std::wstring& nextCommandName);

private:
	CommandPtr			SelectCommandByName (const CommandStructure& commands);
	CommandPtr			SelectCommandByName (const CommandPtr& command);

	std::wstring		commandToSelect;
};

class TestNodeUIEnvironment : public NodeUIEnvironment
{
public:
	TestNodeUIEnvironment (NodeEditor& nodeEditor);

	virtual DrawingContext&		GetDrawingContext () override;
	virtual SkinParams&			GetSkinParams () override;
	virtual EvaluationEnv&		GetEvaluationEnv () override;
	virtual void				OnValuesRecalculated () override;
	virtual void				OnRedrawRequest () override;
	virtual EventHandlers&		GetEventHandlers () override;

	void						SetNextCommandName (const std::wstring& nextCommandName);
	const SvgDrawingContext&	GetSvgDrawingContext () const;

private:
	NodeEditor&			nodeEditor;

	SvgDrawingContext	drawingContext;
	DefaultSkinParams	skinParams;
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
	void	RightClick (const Point& point);
	void	Wheel (MouseWheelRotation rotation, const Point& point);
	void	DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr);
	void	SetNextCommandName (const std::wstring& nextCommandName);

	TestNodeUIEnvironment	uiEnvironment;
	NodeEditor				nodeEditor;
};

#endif
