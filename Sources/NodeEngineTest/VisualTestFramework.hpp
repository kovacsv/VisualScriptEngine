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

	virtual MenuCommandPtr		OnContextMenu (ContextMenuType, const Point&, const MenuCommandStructure&) override;
	virtual bool				OnParameterSettings (ParameterSettingsType, ParameterInterfacePtr) override;
	virtual void				OnDoubleClick (const Point&, MouseButton) override;

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
	virtual void					OnSelectionChanged (const Selection& selection) override;
	virtual void					OnUndoStateChanged (const UndoState& undoState) override;
	virtual void					OnClipboardStateChanged (const ClipboardState& clipboardState) override;
	virtual void					OnIncompatibleVersionPasted (const Version& version) override;

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
	virtual ~NodeEditorTestEnv ();

	bool	CheckReference (const std::wstring& referenceFileName);
	void	Click (const Point& point);
	void	CtrlClick (const Point& point);
	void	RightClick (const Point& point);
	void	Wheel (MouseWheelRotation rotation, const Point& point);
	void	DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr);
	void	DragDrop (const ModifierKeys& keys, const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp = nullptr); 
	void	ExecuteCommand (const NUIE::CommandCode& commandCode);
	void	SetNextCommandName (const std::wstring& nextCommandName);
	void	SetNextCommandParameterSettings (const ParameterSettingsHandler& handler);
	Rect	GetNodeRect (const UINodePtr& node);
	Point	GetOutputSlotConnPosition (const UINodePtr& node, const std::string& slotId);
	Point	GetInputSlotConnPosition (const UINodePtr& node, const std::string& slotId);

	TestNodeUIEnvironment	uiEnvironment;
	NodeEditor				nodeEditor;
};

class SimpleNodeEditorTestEnv : public NodeEditorTestEnv
{
public:
	SimpleNodeEditorTestEnv (const BasicSkinParams& skinParams);
	virtual ~SimpleNodeEditorTestEnv ();

	void RecalcPositions ();

	UINodePtr	doubleUpDownNode;
	UINodePtr	rangeInputNode;
	UINodePtr	viewerUINode1;
	UINodePtr	viewerUINode2;

	Point		pointInBackground;
	Rect		doubleInputRect;
	Rect		rangeInputRect;
	Rect		viewer1InputRect;
	Rect		viewer2InputRect;
	Rect		viewer1InputSlotRect;
	Rect		viewer2InputSlotRect;
	Rect		doubleUpDownOutputSlotRect;
	Rect		rangeOutputSlotSRect;

	Point		doubleInputHeaderPoint;
	Point		rangeInputHeaderPoint;
	Point		viewer1HeaderPoint;
	Point		viewer2HeaderPoint;
};

class SimpleNodeEditorTestEnvWithConnections : public SimpleNodeEditorTestEnv
{
public:
	SimpleNodeEditorTestEnvWithConnections (const BasicSkinParams& skinParams);
};

#endif
