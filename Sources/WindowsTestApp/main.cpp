#include "DrawingControl.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ArithmeticUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "WXAS_ParameterDialog.hpp"
#include "WXAS_NodeEditorControl.hpp"

#include "TestAppNodes.hpp"

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

	MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position) :
		BI::CreateNodeCommand (name, uiManager, uiEnvironment, position),
		nodeType (nodeType)
	{

	}

	virtual NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition) override
	{
		switch (nodeType) {
			case NodeType::Integer:
				return NUIE::UINodePtr (new BI::IntegerUpDownNode (L"Integer", modelPosition, 0, 5));
			case NodeType::Number:
				return NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", modelPosition, 0.0, 5.0));
			case NodeType::IntegerRange:
				return NUIE::UINodePtr (new BI::IntegerRangeNode (L"Integer Range", modelPosition));
			case NodeType::NumberRange:
				return NUIE::UINodePtr (new BI::DoubleRangeNode (L"Number Range", modelPosition));
			case NodeType::Addition:
				return NUIE::UINodePtr (new BI::AdditionNode (L"Addition", modelPosition));
			case NodeType::Subtraction:
				return NUIE::UINodePtr (new BI::SubtractionNode (L"Subtraction", modelPosition));
			case NodeType::Multiplication:
				return NUIE::UINodePtr (new BI::MultiplicationNode (L"Multiplication", modelPosition));
			case NodeType::Division:
				return NUIE::UINodePtr (new BI::DivisionNode (L"Division", modelPosition));
			case NodeType::Color:
				return NUIE::UINodePtr (new ColorNode (L"Color", modelPosition));
			case NodeType::Point:
				return NUIE::UINodePtr (new PointNode (L"Point", modelPosition));
			case NodeType::Line:
				return NUIE::UINodePtr (new LineNode (L"Line", modelPosition));
			case NodeType::Circle:
				return NUIE::UINodePtr (new CircleNode (L"Circle", modelPosition));
			case NodeType::Offset:
				return NUIE::UINodePtr (new OffsetNode (L"Offset", modelPosition));
			case NodeType::Viewer:
				return NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", modelPosition, 5));
		}
		return nullptr;
	}

private:
	NodeType nodeType;
};

class MyNodeEditorEventHandlers : public WXAS::NodeEditorEventHandlers
{
public:
	MyNodeEditorEventHandlers (wxPanel* panel) :
		WXAS::NodeEditorEventHandlers (panel)
	{
	
	}

	virtual NUIE::UICommandPtr OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::UICommandStructure& commands) override
	{
		NUIE::UICommandStructure actualCommands = commands;
		NUIE::UIGroupCommandPtr createCommandGroup (new NUIE::UIGroupCommand (L"Add Node"));

		NUIE::UIGroupCommandPtr inputCommandGroup (new NUIE::UIGroupCommand (L"Input Nodes"));
		inputCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Integer, uiManager, uiEnvironment, L"Integer", position)));
		inputCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Number, uiManager, uiEnvironment, L"Number", position)));
		inputCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::IntegerRange, uiManager, uiEnvironment, L"Integer Range", position)));
		inputCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::NumberRange, uiManager, uiEnvironment, L"Number Range", position)));
		createCommandGroup->AddChildCommand (inputCommandGroup);

		NUIE::UIGroupCommandPtr arithmeticCommandGroup (new NUIE::UIGroupCommand (L"Arithmetic Nodes"));
		arithmeticCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Addition, uiManager, uiEnvironment, L"Addition", position)));
		arithmeticCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Subtraction, uiManager, uiEnvironment, L"Subtraction", position)));
		arithmeticCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Multiplication, uiManager, uiEnvironment, L"Multiplication", position)));
		arithmeticCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Division, uiManager, uiEnvironment, L"Division", position)));
		createCommandGroup->AddChildCommand (arithmeticCommandGroup);

		NUIE::UIGroupCommandPtr drawingCommandGroup (new NUIE::UIGroupCommand (L"Drawing Nodes"));
		drawingCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Color, uiManager, uiEnvironment, L"Color", position)));
		drawingCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Point, uiManager, uiEnvironment, L"Point", position)));
		drawingCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Line, uiManager, uiEnvironment, L"Line", position)));
		drawingCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Circle, uiManager, uiEnvironment, L"Circle", position)));
		createCommandGroup->AddChildCommand (drawingCommandGroup);

		NUIE::UIGroupCommandPtr transformationCommandGroup (new NUIE::UIGroupCommand (L"Transformation Nodes"));
		transformationCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Offset, uiManager, uiEnvironment, L"Offset", position)));
		createCommandGroup->AddChildCommand (transformationCommandGroup);

		NUIE::UIGroupCommandPtr otherCommandGroup (new NUIE::UIGroupCommand (L"Other Nodes"));
		otherCommandGroup->AddChildCommand (NUIE::UICommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Viewer, uiManager, uiEnvironment, L"Viewer", position)));
		createCommandGroup->AddChildCommand (otherCommandGroup);

		actualCommands.AddCommand (createCommandGroup);
		return WXAS::SelectCommandFromContextMenu (panel, position, actualCommands);
	}
};

class MyNodeEditorUIEnvironment : public WXAS::NodeEditorUIEnvironment
{
public:
	MyNodeEditorUIEnvironment (	WXAS::NodeEditorControl* nodeEditorControl,
								DrawingControl* drawingControl,
								std::shared_ptr<NE::StringSettings>& stringSettings,
								std::shared_ptr<NUIE::SkinParams>& skinParams,
								std::shared_ptr<NUIE::EventHandlers>& eventHandlers,
								NE::EvaluationEnv& evaluationEnv) :
		WXAS::NodeEditorUIEnvironment (nodeEditorControl, stringSettings, skinParams, eventHandlers, evaluationEnv),
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

class MyNodeEditorControl : public WXAS::NodeEditorControl
{
public:
	 MyNodeEditorControl (wxWindow *parent) :
		 WXAS::NodeEditorControl (parent)
	 {
	 
	 }

	 virtual void OnInit () override
	 {
		NUIE::NodeUIManager& uiManager = nodeEditor->GetNodeUIManager ();

		static const bool isStressTest = false;
		if (isStressTest) {
			static int count = 10;
			for (int i = 0; i < count; i++) {
				for (int j = 0; j < count; j++) {
					uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleRangeNode (L"Range", NUIE::Point (i * 150, j * 150))), uiEnvironment->GetEvaluationEnv ());
				}
			}
			nodeEditor->Update ();
		} else {
			NUIE::UINodePtr startInputNode = uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (70, 70), 20, 5)), uiEnvironment->GetEvaluationEnv ());
			NUIE::UINodePtr stepInputNode = uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (70, 180), 20, 5)), uiEnvironment->GetEvaluationEnv ());
			NUIE::UINodePtr intRangeNodeX = uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleRangeNode (L"Range", NUIE::Point (220, 100))), uiEnvironment->GetEvaluationEnv ());
			NUIE::UINodePtr inputNodeY = uiManager.AddNode (NUIE::UINodePtr (new BI::DoubleUpDownNode (L"Number", NUIE::Point (220, 220), 20, 5)), uiEnvironment->GetEvaluationEnv ());
			std::shared_ptr<PointNode> pointNode (new PointNode (L"Point", NUIE::Point (400, 150)));
			uiManager.AddNode (pointNode, uiEnvironment->GetEvaluationEnv ());
			pointNode->SetValueCombinationMode (NE::ValueCombinationMode::CrossProduct);
			NUIE::UINodePtr viewerNode = uiManager.AddNode (NUIE::UINodePtr (new BI::MultiLineViewerNode (L"Viewer", NUIE::Point (600, 150), 5)), uiEnvironment->GetEvaluationEnv ());

			uiManager.ConnectOutputSlotToInputSlot (startInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("start")));
			uiManager.ConnectOutputSlotToInputSlot (stepInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("step")));
			uiManager.ConnectOutputSlotToInputSlot (intRangeNodeX->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("x")));
			uiManager.ConnectOutputSlotToInputSlot (inputNodeY->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("y")));
			uiManager.ConnectOutputSlotToInputSlot (pointNode->GetUIOutputSlot (NE::SlotId ("point")), viewerNode->GetUIInputSlot (NE::SlotId ("in")));
			nodeEditor->Update ();
		}
	 }
};

class MenuBar : public wxMenuBar
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

	MenuBar () :
		wxMenuBar ()
	{
		wxMenu* fileMenu = new wxMenu ();
		fileMenu->Append (CommandId::File_New, "New");
		fileMenu->Append (CommandId::File_Open, "Open...");
		fileMenu->Append (CommandId::File_Save, "Save...");
		fileMenu->Append (CommandId::File_SaveAs, "Save As...");
		fileMenu->AppendSeparator ();
		fileMenu->Append (CommandId::File_Exit, L"Exit");
		Append (fileMenu, L"&File");

		wxMenu* editMenu = new wxMenu ();
		editMenu->Append (CommandId::Edit_Undo, "Undo");
		editMenu->Append (CommandId::Edit_Redo, "Redo");
		Append (editMenu, L"&Edit");
	}
};

class MainFrame : public wxFrame
{
public:
	MainFrame (const std::shared_ptr<ResultImage>& resultImage, NE::EvaluationEnv& evaluationEnv) :
		wxFrame (NULL, wxID_ANY, L"Node Engine Test App", wxDefaultPosition, wxSize (1000, 600)),
		mainWindow (new wxSplitterWindow (this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH | wxSP_LIVE_UPDATE)),
		drawingControl (new DrawingControl (mainWindow, resultImage)),
		nodeEditorControl (new MyNodeEditorControl (mainWindow)),
		applicationState ()
	{
		std::shared_ptr<WXAS::NodeEditorUIEnvironment> uiEnvironment = std::shared_ptr<WXAS::NodeEditorUIEnvironment> (
			new MyNodeEditorUIEnvironment (
				nodeEditorControl,
				drawingControl,
				std::shared_ptr<NE::StringSettings> (new NE::BasicStringSettings (L'.', L',', 2)),
				std::shared_ptr<NUIE::SkinParams> (new NUIE::DefaultSkinParams ()),
				std::shared_ptr<NUIE::EventHandlers> (new MyNodeEditorEventHandlers (nodeEditorControl)),
				evaluationEnv
			)
		);

		nodeEditorControl->Init (uiEnvironment);

		SetMenuBar (new MenuBar ());

		CreateStatusBar ();
		UpdateStatusBar ();

		mainWindow->SetSashGravity (0.5);
		mainWindow->SetMinimumPaneSize (20);
		mainWindow->SplitVertically (nodeEditorControl, drawingControl, 700);
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

	wxSplitterWindow*			mainWindow;
	DrawingControl*				drawingControl;
	WXAS::NodeEditorControl*	nodeEditorControl;

	ApplicationState			applicationState;

	DECLARE_EVENT_TABLE ()
};

BEGIN_EVENT_TABLE (MainFrame, wxFrame)
EVT_MENU (MenuBar::CommandId::File_New, MainFrame::OnNew)
EVT_MENU (MenuBar::CommandId::File_Open, MainFrame::OnOpen)
EVT_MENU (MenuBar::CommandId::File_Save, MainFrame::OnSave)
EVT_MENU (MenuBar::CommandId::File_SaveAs, MainFrame::OnSaveAs)
EVT_MENU (MenuBar::CommandId::File_Exit, MainFrame::OnExit)
EVT_MENU (MenuBar::CommandId::Edit_Undo, MainFrame::OnUndo)
EVT_MENU (MenuBar::CommandId::Edit_Redo, MainFrame::OnRedo)
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
