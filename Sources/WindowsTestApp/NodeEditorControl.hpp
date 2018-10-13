#ifndef NODEEDITORCONTROL_HPP
#define NODEEDITORCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInCommands.hpp"
#include "WXAS_ControlUtilities.hpp"

#include <wx/wx.h>
#include <memory>

namespace WXAS
{

class MyCreateNodeCommand : public BI::CreateNodeCommand
{
public:
	enum class NodeType
	{
		Integer,
		Number,
		IntegerRange,
		NumberRange,
		Addition,
		Subtraction,
		Multiplication,
		Division,
		Color,
		Point,
		Line,
		Circle,
		Offset,
		Viewer
	};

	MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position);

	virtual NUIE::UINodePtr		CreateNode (const NUIE::Point& modelPosition) override;

private:
	NodeType nodeType;
};

class NodeEditorEventHandler
{
public:
	NodeEditorEventHandler ();
	virtual ~NodeEditorEventHandler ();

	virtual void OnValuesRecalculated () = 0;
};

class NodeEditorControl;

class NodeEditorUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	NodeEditorUIEnvironment (NodeEditorControl* nodeEditorControl, NE::EvaluationEnv& evaluationEnv);

	void							OnPaint (wxPanel* panel, wxPaintEvent& evt);
	void							OnResize (int width, int height);

	virtual const NE::StringSettings&	GetStringSettings () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandlers&		GetEventHandlers () override;
	virtual double						GetMouseMoveMinOffset () override;

private:
	NodeEditorControl*								nodeEditorControl;
	NE::EvaluationEnv&								evaluationEnv;

	NE::BasicStringSettings							stringSettings;
	NUIE::DefaultSkinParams							skinParams;
	std::shared_ptr<NUIE::NativeDrawingContext>		drawingContext;
	std::shared_ptr<NUIE::EventHandlers>			eventHandlers;
};

class NodeEditorControl : public wxPanel
{
public:
	NodeEditorControl (wxWindow *parent, NE::EvaluationEnv& evaluationEnv, NodeEditorEventHandler& editorEventHandler);

	void							OnPaint (wxPaintEvent& evt);
	void							OnResize (wxSizeEvent& evt);
	void							OnMouseCaptureLost (wxMouseCaptureLostEvent& evt);

	void							OnLeftButtonDown (wxMouseEvent& evt);
	void							OnLeftButtonUp (wxMouseEvent& evt);
	void							OnLeftButtonDoubleClick (wxMouseEvent& evt);

	void							OnRightButtonDown (wxMouseEvent& evt);
	void							OnRightButtonUp (wxMouseEvent& evt);
	void							OnRightButtonDoubleClick (wxMouseEvent& evt);

	void							OnMiddleButtonDown (wxMouseEvent& evt);
	void							OnMiddleButtonUp (wxMouseEvent& evt);
	void							OnMiddleButtonDoubleClick (wxMouseEvent& evt);

	void							OnMouseMove (wxMouseEvent& evt);
	void							OnMouseWheel (wxMouseEvent& evt);

	void							OnKeyDown (wxKeyEvent& evt);

	void							New ();
	bool							Open (const std::wstring& fileName);
	bool							Save (const std::wstring& fileName);
	void							Undo ();
	void							Redo ();

	void							OnValuesRecalculated ();

private:
	MouseCaptureHandler				captureHandler;
	NodeEditorUIEnvironment			uiEnvironment;
	NUIE::NodeEditor				nodeEditor;
	NodeEditorEventHandler&			editorEventHandler;

	DECLARE_EVENT_TABLE ()
};

}

#endif
