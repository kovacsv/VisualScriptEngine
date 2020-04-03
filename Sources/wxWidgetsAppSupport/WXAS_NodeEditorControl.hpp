#ifndef WXAS_NODEEDITORCONTROL_HPP
#define WXAS_NODEEDITORCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "BI_BuiltInCommands.hpp"
#include "WXAS_ControlUtilities.hpp"

#include <wx/wx.h>
#include <memory>

namespace WXAS
{

class NodeEditorControl;

class NodeEditorEventHandlers : public NUIE::EventHandlers
{
public:
	NodeEditorEventHandlers (wxPanel* panel);
	virtual ~NodeEditorEventHandlers ();

	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& outputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodeGroupPtr& group, const NUIE::MenuCommandStructure& commands) override;
	virtual bool OnParameterSettings (NUIE::ParameterInterfacePtr paramInterface);

protected:
	wxPanel* panel;
};

class NodeEditorUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	NodeEditorUIEnvironment	(	NodeEditorControl* nodeEditorControl,
								NE::StringSettingsPtr& stringSettings,
								NUIE::SkinParamsPtr& skinParams,
								NUIE::EventHandlersPtr& eventHandlers,
								NE::EvaluationEnv& evaluationEnv);
	virtual ~NodeEditorUIEnvironment ();

	void								OnPaint (wxPanel* panel, wxPaintEvent& evt);
	void								OnResize (int width, int height);

	virtual const NE::StringSettings&	GetStringSettings () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;
	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandlers&		GetEventHandlers () override;
	virtual double						GetMouseMoveMinOffset () override;

private:
	NodeEditorControl*								nodeEditorControl;
	NE::EvaluationEnv&								evaluationEnv;

	NE::StringSettingsPtr							stringSettings;
	NUIE::SkinParamsPtr								skinParams;
	NUIE::EventHandlersPtr							eventHandlers;
	std::shared_ptr<NUIE::NativeDrawingContext>		drawingContext;
};

class NodeEditorControl : public wxPanel
{
public:
	enum class UpdateMode
	{
		Automatic,
		Manual
	};

	NodeEditorControl (wxWindow *parent);
	virtual ~NodeEditorControl ();

	void						Init (std::shared_ptr<NodeEditorUIEnvironment>& editorUIEnvironment);
	virtual void				OnInit ();

	void						OnPaint (wxPaintEvent& evt);
	void						OnResize (wxSizeEvent& evt);
	void						OnMouseCaptureLost (wxMouseCaptureLostEvent& evt);

	void						OnLeftButtonDown (wxMouseEvent& evt);
	void						OnLeftButtonUp (wxMouseEvent& evt);
	void						OnLeftButtonDoubleClick (wxMouseEvent& evt);

	void						OnRightButtonDown (wxMouseEvent& evt);
	void						OnRightButtonUp (wxMouseEvent& evt);
	void						OnRightButtonDoubleClick (wxMouseEvent& evt);

	void						OnMiddleButtonDown (wxMouseEvent& evt);
	void						OnMiddleButtonUp (wxMouseEvent& evt);
	void						OnMiddleButtonDoubleClick (wxMouseEvent& evt);

	void						OnMouseMove (wxMouseEvent& evt);
	void						OnMouseWheel (wxMouseEvent& evt);

	void						OnKeyDown (wxKeyEvent& evt);

	UpdateMode					GetUpdateMode () const;
	void						SetUpdateMode (UpdateMode mode) const;
	void						ManualUpdate ();

	void						AddNode (const NUIE::UINodePtr& uiNode);
	NUIE::Point					ViewToModel (const NUIE::Point& viewPoint) const;
	void						AlignToWindow ();
	void						FitToWindow ();

	const NE::NodeCollection&	GetSelectedNodes () const;
	void						SetSelectedNodes (const NE::NodeCollection& newSelectedNodes);

	void						New ();
	bool						Open (const std::wstring& fileName, const NUIE::ExternalFileIO* externalFileIO, const NUIE::ExternalHeaderIO* externalHeader);
	bool						Open (NE::InputStream& inputStream, const NUIE::ExternalHeaderIO* externalHeader);
	bool						Save (const std::wstring& fileName, const NUIE::ExternalFileIO* externalFileIO, const NUIE::ExternalHeaderIO* externalHeader);
	bool						Save (NE::OutputStream& outputStream, const NUIE::ExternalHeaderIO* externalHeader) const;
	bool						NeedToSave () const;
	void						Undo ();
	void						Redo ();

protected:
	MouseCaptureHandler							captureHandler;
	std::shared_ptr<NodeEditorUIEnvironment>	uiEnvironment;
	std::shared_ptr<NUIE::NodeEditor>			nodeEditor;

	DECLARE_EVENT_TABLE ()
};

}

#endif
