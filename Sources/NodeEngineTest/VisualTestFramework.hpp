#ifndef VISUALTESTFRAMEWORK_HPP
#define VISUALTESTFRAMEWORK_HPP

#include "NE_StringSettings.hpp"
#include "NE_EvaluationEnv.hpp"
#include "NUIE_EventHandlers.hpp"
#include "NUIE_NodeEditor.hpp"
#include "NUIE_SvgDrawingContext.hpp"

using namespace NE;
using namespace NUIE;

class TestEventHandlers : public EventHandlers
{
public:
	TestEventHandlers ();

	virtual MenuCommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr	OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodeGroupPtr&, const MenuCommandStructure&) override;
	virtual bool			OnParameterSettings (ParameterInterfacePtr) override;

	void					SetNextCommandName (const std::wstring& nextCommandName);

private:
	MenuCommandPtr			SelectCommandByName (const MenuCommandStructure& commands);
	MenuCommandPtr			SelectCommandByName (const MenuCommandPtr& command);

	std::wstring		commandToSelect;
};

class TestNodeUIEnvironment : public NodeUIEnvironment
{
public:
	TestNodeUIEnvironment (NodeEditor& nodeEditor, const BasicSkinParams& skinParams);

	virtual const StringSettings&	GetStringSettings () override;
	virtual const SkinParams&		GetSkinParams () override;
	virtual DrawingContext&			GetDrawingContext () override;
	virtual EvaluationEnv&			GetEvaluationEnv () override;
	virtual void					OnEvaluationBegin () override;
	virtual void					OnEvaluationEnd () override;
	virtual void					OnValuesRecalculated () override;
	virtual void					OnRedrawRequested () override;
	virtual EventHandlers&			GetEventHandlers () override;
	virtual double					GetMouseMoveMinOffset () override;

	void							SetNextCommandName (const std::wstring& nextCommandName);
	const SvgDrawingContext&		GetSvgDrawingContext () const;

private:
	NodeEditor&				nodeEditor;

	BasicStringSettings		stringSettings;
	BasicSkinParams			skinParams;
	SvgDrawingContext		drawingContext;
	TestEventHandlers		eventHandlers;
	EvaluationEnv			evaluationEnv;
};

class NodeEditorTestEnv
{
public:
	NodeEditorTestEnv (const BasicSkinParams& skinParams);

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
