#ifndef NODEEDITORCONTROL_HPP
#define NODEEDITORCONTROL_HPP

#include "BuiltInCommands.hpp"
#include "ControlUtilities.hpp"
#include "NodeEditor.hpp"
#include "ResultImage.hpp"
#include <wx/wx.h>
#include <memory>

class MyCreateNodeCommand : public NUIE::CreateNodeCommand
{
public:
	enum class NodeType
	{
		Number,
		IntegerRange,
		Double,
		Color,
		Point,
		Line,
		Circle,
		Viewer
	};

	MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position);

	virtual NUIE::UINodePtr		CreateNode (const NUIE::Point& modelPosition) override;

private:
	NodeType nodeType;
};

class AppEventHandlers : public NUIE::EventHandlers
{
public:
	AppEventHandlers (wxPanel* panel);

	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::CommandStructure& commands) override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::CommandStructure& commands) override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::OutputSlotPtr& outputSlot, const NUIE::CommandStructure& commands) override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::InputSlotPtr& inputSlot, const NUIE::CommandStructure& commands) override;
	virtual bool				OnParameterSettings (NUIE::NodeParameterAccessorPtr paramAccessor) override;

private:
	wxPanel* panel;
};

class UpdateInterface
{
public:
	virtual void RedrawResultImage () = 0;
};

class NodeEditorControl;

class NodeEditorUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	NodeEditorUIEnvironment (NodeEditorControl* nodeEditorControl, NE::EvaluationEnv& evaluationEnv);

	void							OnPaint (wxPanel* panel, wxPaintEvent& evt);
	void							OnResize (int width, int height);

	virtual NUIE::DrawingContext&	GetDrawingContext () override;
	virtual NUIE::SkinParams&		GetSkinParams () override;
	virtual NE::EvaluationEnv&		GetEvaluationEnv () override;
	virtual void					OnValuesRecalculated () override;
	virtual void					OnRedrawRequest () override;
	virtual NUIE::EventHandlers&	GetEventHandlers () override;
	virtual void					OnSelectionChanged () override;

private:
	NodeEditorControl*								nodeEditorControl;
	NE::EvaluationEnv&								evaluationEnv;

	std::shared_ptr<NUIE::NativeDrawingContext>		drawingContext;
	NUIE::SkinParams								skinParams;
	AppEventHandlers								eventHandlers;
};

class NodeEditorControl : public wxPanel
{
public:
	NodeEditorControl (wxWindow *parent, UpdateInterface& updateInterface, NE::EvaluationEnv& evaluationEnv);

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

	void							New ();
	bool							Open (const std::wstring& fileName);
	bool							Save (const std::wstring& fileName);

	void							RedrawResultImage ();

private:
	UpdateInterface&				updateInterface;
	MouseCaptureHandler				captureHandler;
	NodeEditorUIEnvironment			uiEnvironment;
	NUIE::NodeEditor				nodeEditor;

	DECLARE_EVENT_TABLE ()
};

#endif
