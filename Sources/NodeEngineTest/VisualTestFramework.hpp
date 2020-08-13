#ifndef VISUALTESTFRAMEWORK_HPP
#define VISUALTESTFRAMEWORK_HPP

#include "NE_StringConverter.hpp"
#include "NE_EvaluationEnv.hpp"
#include "NUIE_EventHandler.hpp"
#include "NUIE_NodeEditor.hpp"
#include "NUIE_SvgDrawingContext.hpp"

using namespace NE;
using namespace NUIE;

using ParameterSettingsHandler = std::function<bool (ParameterInterfacePtr)>;

class TestEventHandler : public EventHandler
{
public:
	TestEventHandler (NodeEditor* nodeEditor);

	virtual MenuCommandPtr		OnContextMenu (const Point&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr		OnContextMenu (const Point&, const UINodePtr&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr		OnContextMenu (const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr		OnContextMenu (const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure&) override;
	virtual MenuCommandPtr		OnContextMenu (const Point&, const UINodeGroupPtr&, const MenuCommandStructure&) override;
	virtual void				OnDoubleClick (const Point&, MouseButton) override;
	virtual bool				OnParameterSettings (ParameterInterfacePtr, const UINodePtr&) override;
	virtual bool				OnParameterSettings (ParameterInterfacePtr, const UINodeGroupPtr&) override;

	void						SetNextCommandName (const std::wstring& nextCommandName);
	void						SetNextCommandParameterSettings (const ParameterSettingsHandler& handler);

private:
	MenuCommandPtr				SelectCommandByName (const MenuCommandStructure& commands);
	MenuCommandPtr				SelectCommandByName (const MenuCommandPtr& command);

	NodeEditor*					nodeEditor;
	std::wstring				commandToSelect;
	ParameterSettingsHandler	paramSettingsHandler;
};

class TestNodeUIEnvironment : public NodeUIEnvironment
{
public:
	TestNodeUIEnvironment (NodeEditor& nodeEditor, const BasicSkinParams& skinParams);

	virtual const StringConverter&	GetStringConverter () override;
	virtual const SkinParams&		GetSkinParams () override;
	virtual DrawingContext&			GetDrawingContext () override;
	virtual double					GetWindowScale () override;
	virtual EvaluationEnv&			GetEvaluationEnv () override;
	virtual void					OnEvaluationBegin () override;
	virtual void					OnEvaluationEnd () override;
	virtual void					OnValuesRecalculated () override;
	virtual void					OnRedrawRequested () override;
	virtual EventHandler&			GetEventHandler () override;
	virtual ClipboardHandler&		GetClipboardHandler () override;
	virtual double					GetMouseMoveMinOffset () override;

	void							SetNextCommandName (const std::wstring& nextCommandName);
	void							SetNextCommandParameterSettings (const ParameterSettingsHandler& handler);
	const SvgDrawingContext&		GetSvgDrawingContext () const;
	void							SetWindowScale (double newWindowScale);

private:
	NodeEditor&				nodeEditor;

	BasicStringConverter	stringConverter;
	BasicSkinParams			skinParams;
	SvgDrawingContext		drawingContext;
	TestEventHandler		eventHandler;
	MemoryClipboardHandler	clipboardHandler;
	EvaluationEnv			evaluationEnv;
	double					windowScale;
};

class NodeEditorTestEnv
{
public:
	NodeEditorTestEnv (const BasicSkinParams& skinParams);

	bool	CheckReference (const std::wstring& referenceFileName);
	void	Click (const Point& point);
	void	CtrlClick (const Point& point);
	void	RightClick (const Point& point);
	void	Wheel (MouseWheelRotation rotation, const Point& point);
	void	DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr);
	void	DragDrop (const ModifierKeys& keys, const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr); 
	void	KeyPress (const NUIE::Key& key);
	void	SetNextCommandName (const std::wstring& nextCommandName);
	void	SetNextCommandParameterSettings (const ParameterSettingsHandler& handler);

	TestNodeUIEnvironment	uiEnvironment;
	NodeEditor				nodeEditor;
};

#endif
