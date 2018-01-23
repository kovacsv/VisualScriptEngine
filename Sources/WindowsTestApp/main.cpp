#include "BitmapContextGdi.hpp"
#include "BitmapContextGdiplus.hpp"
#include "WindowsAppUtilities.hpp"
#include "NodeEditorControl.hpp"
#include "DrawingControl.hpp"
#include "Debug.hpp"
#include "ParameterList.hpp"
#include "SingleValues.hpp"

#include <CommCtrl.h>
#include <iostream>
#include <fstream>

#include "wx/wx.h"
#include "wx/splitter.h"

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

class ParametersPanel : public wxPanel
{
public:
	ParametersPanel (wxWindow *parent) :
		wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
		parameterList (new ParameterList (this)),
		sizer (new wxBoxSizer (wxHORIZONTAL))
	{
		sizer->Add (parameterList, 0, wxEXPAND);
		SetSizer (sizer);
	}

	void FillParameters (ParameterAccessorPtr& paramAccessor)
	{
		parameterList->SetParameterAccessor (paramAccessor);
		parameterList->FillParameters ();
	}

private:
	ParameterList*		parameterList;
	wxBoxSizer*			sizer;
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
		File_Exit		= 5
	};

	MainFrame (const std::shared_ptr<ResultImage>& resultImage, NE::EvaluationEnv& evaluationEnv) :
		wxFrame (NULL, wxID_ANY, L"Node Engine Test App", wxDefaultPosition, wxSize (1000, 600)),
		menuBar (new wxMenuBar ()),
		fileMenu (new wxMenu ()),
		parametersPanel (new ParametersPanel (this)),
		editorAndDrawingSplitter (new wxSplitterWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE)),
		drawingControl (new DrawingControl (editorAndDrawingSplitter, resultImage)),
		updateInterface (parametersPanel, drawingControl),
		nodeEditorControl (new NodeEditorControl (editorAndDrawingSplitter, updateInterface, evaluationEnv)),
		mainSizer (new wxBoxSizer (wxHORIZONTAL)),
		applicationState ()
	{
		mainSizer->Add (editorAndDrawingSplitter, 1, wxEXPAND);
		mainSizer->Add (parametersPanel, 0, wxEXPAND);
		SetSizer (mainSizer);

		fileMenu->Append (CommandId::File_New, "New");
		fileMenu->Append (CommandId::File_Open, "Open...");
		fileMenu->Append (CommandId::File_Save, "Save...");
		fileMenu->Append (CommandId::File_SaveAs, "Save As...");
		fileMenu->AppendSeparator ();
		fileMenu->Append (CommandId::File_Exit, L"Exit");
		menuBar->Append (fileMenu, L"&File");
		SetMenuBar (menuBar);

		CreateStatusBar ();
		UpdateStatusBar ();

		editorAndDrawingSplitter->SetSashGravity (0.5);
		editorAndDrawingSplitter->SetMinimumPaneSize (20);
		editorAndDrawingSplitter->SplitHorizontally (nodeEditorControl, drawingControl, 300);
	}

	~MainFrame ()
	{
	
	}

	void OnNew (wxCommandEvent& event)
	{
		nodeEditorControl->New ();
		drawingControl->ClearImage ();
		applicationState.ClearCurrentFileName ();
		UpdateStatusBar ();
	}

	void OnOpen (wxCommandEvent& event)
	{
		wxFileDialog fileDialog (this, L"Open", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
		if (fileDialog.ShowModal () == wxID_OK) {
			std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
			drawingControl->ClearImage ();
			// TODO: handle when open fails
			if (nodeEditorControl->Open (fileName)) {
				applicationState.SetCurrentFileName (fileName);
			}
		}
		UpdateStatusBar ();
	}

	void OnSave (wxCommandEvent& event)
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

	void OnSaveAs (wxCommandEvent& event)
	{
		wxFileDialog fileDialog (this, L"Save As", L"", L"", L"Node Engine Files (*.ne)|*.ne", wxFD_SAVE);
		if (fileDialog.ShowModal () == wxID_OK) {
			std::wstring fileName = fileDialog.GetPath ().ToStdWstring ();
			nodeEditorControl->Save (fileName);
			applicationState.SetCurrentFileName (fileName);
		}
		UpdateStatusBar ();
	}

	void OnExit (wxCommandEvent& event)
	{
		Close (true);
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
	class NodeEditorUpdateInterface : public UpdateInterface
	{
	public:
		NodeEditorUpdateInterface (ParametersPanel* parametersPanel, DrawingControl* drawingControl) :
			parametersPanel (parametersPanel),
			drawingControl (drawingControl)
		{
		
		}

		virtual void RedrawResultImage () override
		{
			drawingControl->RedrawImage ();
		}

		virtual void UpdateParameters (NUIE::NodeParameterAccessorPtr& nodeParameterAccessor) override
		{
			class MyParamAccessor : public ParameterAccessor
			{
			public:
				MyParamAccessor (NUIE::NodeParameterAccessorPtr& nodeParameterAccessor) :
					ParameterAccessor (),
					nodeParameterAccessor (nodeParameterAccessor)
				{
				}

				virtual size_t GetParameterCount () const override
				{
					return nodeParameterAccessor->GetParameterCount ();
				}

				virtual std::wstring GetParameterName (int index) const override
				{
					return nodeParameterAccessor->GetParameterName (index);
				}

				virtual std::wstring GetParameterValue (int index) const override
				{
					NE::ValuePtr value = nodeParameterAccessor->GetParameterValue (index);
					if (DBGERROR (!NE::Value::IsType<NE::StringValue> (value))) {
						return L"";
					}
					return NE::StringValue::Get (value);
				}

				virtual bool SetParameterValue (int index, const std::wstring& value) override
				{
					NE::ValuePtr valuePtr (new NE::StringValue (value));
					return nodeParameterAccessor->SetParameterValue (index, valuePtr);
				}

			private:
				NUIE::NodeParameterAccessorPtr nodeParameterAccessor;
			};

			ParameterAccessorPtr accessor (new MyParamAccessor (nodeParameterAccessor));
			parametersPanel->FillParameters (accessor);
		}

	private:
		ParametersPanel*	parametersPanel;
		DrawingControl*		drawingControl;
	};

	wxMenuBar*					menuBar;
	wxMenu*						fileMenu;

	ParametersPanel*			parametersPanel;
	wxSplitterWindow*			editorAndDrawingSplitter;
	DrawingControl*				drawingControl;
	NodeEditorUpdateInterface	updateInterface;
	NodeEditorControl*			nodeEditorControl;
	wxBoxSizer*					mainSizer;

	ApplicationState			applicationState;

	DECLARE_EVENT_TABLE ()
};

BEGIN_EVENT_TABLE (MainFrame, wxFrame)
EVT_MENU (MainFrame::CommandId::File_New, MainFrame::OnNew)
EVT_MENU (MainFrame::CommandId::File_Open, MainFrame::OnOpen)
EVT_MENU (MainFrame::CommandId::File_Save, MainFrame::OnSave)
EVT_MENU (MainFrame::CommandId::File_SaveAs, MainFrame::OnSaveAs)
EVT_MENU (MainFrame::CommandId::File_Exit, MainFrame::OnExit)
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
