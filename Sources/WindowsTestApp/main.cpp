#include "DrawingControl.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"
#include "WXAS_ParameterDialog.hpp"
#include "NodeEditorControl.hpp"

#include <CommCtrl.h>
#include <iostream>
#include <fstream>

#include <wx/wx.h>
#include <wx/splitter.h>

class ApplicationState
{
public:
	ApplicationState () :
		currentFileName ()
	{

	}

	void ClearCurrentFileName ()
	{
		currentFileName.clear ();
	}

	void SetCurrentFileName (const std::wstring& newCurrentFileName)
	{
		currentFileName = newCurrentFileName;
	}

	bool HasCurrentFileName () const
	{
		return !currentFileName.empty ();
	}

	const std::wstring& GetCurrentFileName () const
	{
		return currentFileName;
	}

private:
	std::wstring currentFileName;
};

class MyNodeEditorEventHandler : public WXAS::NodeEditorEventHandler
{
public:
	MyNodeEditorEventHandler (DrawingControl* drawingControl) :
		drawingControl (drawingControl)
	{
		
	}

	virtual void OnValuesRecalculated () override
	{
		drawingControl->RedrawImage ();
	}

private:
	DrawingControl* drawingControl;
};

class MainFrame : public wxFrame
{
public:
	enum CommandId
	{
		File_New		= 1,
		File_Open		= 2,
		File_Save		= 3,
		File_SaveAs		= 4,
		File_Exit		= 5,
		Edit_Undo		= 6,
		Edit_Redo		= 7
	};

	MainFrame (const std::shared_ptr<ResultImage>& resultImage, NE::EvaluationEnv& evaluationEnv) :
		wxFrame (NULL, wxID_ANY, L"Node Engine Test App", wxDefaultPosition, wxSize (1000, 600)),
		menuBar (new wxMenuBar ()),
		fileMenu (new wxMenu ()),
		editMenu (new wxMenu ()),
		editorAndDrawingSplitter (new wxSplitterWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE)),
		drawingControl (new DrawingControl (editorAndDrawingSplitter, resultImage)),
		editorEventHandler (drawingControl),
		nodeEditorControl (new WXAS::NodeEditorControl (editorAndDrawingSplitter, evaluationEnv, editorEventHandler)),
		applicationState ()
	{
		fileMenu->Append (CommandId::File_New, "New");
		fileMenu->Append (CommandId::File_Open, "Open...");
		fileMenu->Append (CommandId::File_Save, "Save...");
		fileMenu->Append (CommandId::File_SaveAs, "Save As...");
		fileMenu->AppendSeparator ();
		fileMenu->Append (CommandId::File_Exit, L"Exit");
		menuBar->Append (fileMenu, L"&File");

		editMenu->Append (CommandId::Edit_Undo, "Undo");
		editMenu->Append (CommandId::Edit_Redo, "Redo");
		menuBar->Append (editMenu, L"&Edit");

		SetMenuBar (menuBar);

		CreateStatusBar ();
		UpdateStatusBar ();

		editorAndDrawingSplitter->SetSashGravity (0.5);
		editorAndDrawingSplitter->SetMinimumPaneSize (20);
		editorAndDrawingSplitter->SplitVertically (nodeEditorControl, drawingControl, 700);
	}

	~MainFrame ()
	{
	
	}

	void OnNew (wxCommandEvent& evt)
	{
		Reset ();
		UpdateStatusBar ();
	}

	void OnOpen (wxCommandEvent& evt)
	{
		wxFileDialog fileDialog (this, L"Open", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (fileDialog.ShowModal () == wxID_OK) {
			std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
			drawingControl->ClearImage ();
			// TODO: handle when open fails
			if (nodeEditorControl->Open (fileName)) {
				applicationState.SetCurrentFileName (fileName);
			} else {
				Reset ();
			}
		}
		UpdateStatusBar ();
	}

	void OnSave (wxCommandEvent& evt)
	{
		wxFileDialog fileDialog (this, L"Save", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
		if (applicationState.HasCurrentFileName ()) {
			nodeEditorControl->Save (applicationState.GetCurrentFileName ());
		} else if (fileDialog.ShowModal () == wxID_OK) {
			std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
			nodeEditorControl->Save (fileName);
			applicationState.SetCurrentFileName (fileName);
		}
		UpdateStatusBar ();
	}

	void OnSaveAs (wxCommandEvent& evt)
	{
		wxFileDialog fileDialog (this, L"Save As", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
		if (fileDialog.ShowModal () == wxID_OK) {
			std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
			nodeEditorControl->Save (fileName);
			applicationState.SetCurrentFileName (fileName);
		}
		UpdateStatusBar ();
	}

	void OnExit (wxCommandEvent& evt)
	{
		Close (true);
	}

	void OnUndo (wxCommandEvent& evt)
	{
		nodeEditorControl->Undo ();
	}

	void OnRedo (wxCommandEvent& evt)
	{
		nodeEditorControl->Redo ();
	}

	void UpdateStatusBar ()
	{
		std::wstring currentFileText = L"No File";
		if (applicationState.HasCurrentFileName ()) {
			currentFileText = applicationState.GetCurrentFileName ();
		}
		SetStatusText (currentFileText);
	}

private:
	void Reset ()
	{
		nodeEditorControl->New ();
		drawingControl->ClearImage ();
		applicationState.ClearCurrentFileName ();
	}

	wxMenuBar*					menuBar;
	wxMenu*						fileMenu;
	wxMenu*						editMenu;

	wxSplitterWindow*			editorAndDrawingSplitter;
	DrawingControl*				drawingControl;
	MyNodeEditorEventHandler	editorEventHandler;
	WXAS::NodeEditorControl*	nodeEditorControl;

	ApplicationState			applicationState;

	DECLARE_EVENT_TABLE ()
};

BEGIN_EVENT_TABLE (MainFrame, wxFrame)
EVT_MENU (MainFrame::CommandId::File_New, MainFrame::OnNew)
EVT_MENU (MainFrame::CommandId::File_Open, MainFrame::OnOpen)
EVT_MENU (MainFrame::CommandId::File_Save, MainFrame::OnSave)
EVT_MENU (MainFrame::CommandId::File_SaveAs, MainFrame::OnSaveAs)
EVT_MENU (MainFrame::CommandId::File_Exit, MainFrame::OnExit)
EVT_MENU (MainFrame::CommandId::Edit_Undo, MainFrame::OnUndo)
EVT_MENU (MainFrame::CommandId::Edit_Redo, MainFrame::OnRedo)
END_EVENT_TABLE ()

class NodeEngineTestApplication : public wxApp
{
public:
	NodeEngineTestApplication () :
		resultImage (new ResultImage ()),
		evaluationData (new ResultImageEvaluationData (resultImage)),
		evaluationEnv (evaluationData),
		mainFrame (nullptr)
	{
	
	}

	virtual bool OnInit ()
	{
		mainFrame = new MainFrame (resultImage, evaluationEnv);
		mainFrame->Show (true);
		SetTopWindow (mainFrame);
		return true;
	}

private:
	std::shared_ptr<ResultImage>				resultImage;
	std::shared_ptr<ResultImageEvaluationData>	evaluationData;
	NE::EvaluationEnv							evaluationEnv;

	MainFrame*									mainFrame;
};

IMPLEMENT_APP (NodeEngineTestApplication)
