#include "NodeEditorControl.hpp"
#include "ContextFactory.hpp"

#include "InputUINodes.hpp"
#include "ArithmeticUINodes.hpp"
#include "ViewerUINodes.hpp"
#include "TestAppNodes.hpp"

#include "ParameterDialog.hpp"

#include <wx/menu.h>

static NUIE::CommandPtr SelectCommandFromContextMenu (wxPanel* panel, const NUIE::Point& position, const NUIE::CommandStructure& commands)
{
	if (commands.IsEmpty ()) {
		return nullptr;
	}

	wxMenu popupMenu;
	wxMenu* currentMenu = &popupMenu;

	size_t currentCommandId = 1000;
	std::unordered_map<size_t, NUIE::CommandPtr> commandTable;
	std::function<void (const NUIE::CommandPtr&)> addCommand = [&] (const NUIE::CommandPtr& command) {
		if (command->HasChildCommands ()) {
			wxMenu* oldMenu = currentMenu;
			currentMenu = new wxMenu ();
			oldMenu->AppendSubMenu (currentMenu, command->GetName ());
			command->EnumerateChildCommands (addCommand);
			currentMenu = oldMenu;
		} else {
			wxMenuItem* currentMenuItem = currentMenu->AppendCheckItem (currentCommandId, command->GetName ());
			currentMenuItem->Check (command->IsChecked ());
			commandTable.insert ({ currentCommandId, command });
			currentCommandId += 1;
		}
	};
	commands.EnumerateCommands (addCommand);

	wxPoint mousePos ((int) position.GetX (), (int) position.GetY ());
	int selectedItem = panel->GetPopupMenuSelectionFromUser (popupMenu, mousePos);
	if (selectedItem == wxID_NONE) {
		return nullptr;
	}

	return commandTable[selectedItem];
}

MyCreateNodeCommand::MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position) :
	NUIE::CreateNodeCommand (name, uiManager, uiEnvironment, position),
	nodeType (nodeType)
{

}

NUIE::UINodePtr MyCreateNodeCommand::CreateNode (const NUIE::Point& modelPosition)
{
	switch (nodeType) {
		case NodeType::Number:
			return NUIE::UINodePtr (new NUIE::NumericUpDownNode (L"Number", modelPosition, 0, 5));
		case NodeType::Range:
			return NUIE::UINodePtr (new NUIE::NumberRangeNode (L"Range", modelPosition));
		case NodeType::Addition:
			return NUIE::UINodePtr (new NUIE::AdditionNode (L"Addition", modelPosition));
		case NodeType::Subtraction:
			return NUIE::UINodePtr (new NUIE::SubtractionNode (L"Subtraction", modelPosition));
		case NodeType::Multiplication:
			return NUIE::UINodePtr (new NUIE::MultiplicationNode (L"Multiplication", modelPosition));
		case NodeType::Division:
			return NUIE::UINodePtr (new NUIE::DivisionNode (L"Division", modelPosition));
		case NodeType::Color:
			return NUIE::UINodePtr (new ColorNode (L"Color", modelPosition));
		case NodeType::Point:
			return NUIE::UINodePtr (new PointNode (L"Point", modelPosition));
		case NodeType::Line:
			return NUIE::UINodePtr (new LineNode (L"Line", modelPosition));
		case NodeType::Circle:
			return NUIE::UINodePtr (new CircleNode (L"Circle", modelPosition));
		case NodeType::Viewer:
			return NUIE::UINodePtr (new NUIE::MultiLineViewerNode (L"Viewer", modelPosition, 5));
	}
	return nullptr;
}

AppEventHandlers::AppEventHandlers (wxPanel* panel) :
	panel (panel)
{

}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::CommandStructure& commands)
{
	NUIE::CommandStructure actualCommands = commands;
	NUIE::GroupCommandPtr createCommandGroup (new NUIE::GroupCommand (L"Add Node"));

	NUIE::GroupCommandPtr inputCommandGroup (new NUIE::GroupCommand (L"Input Nodes"));
	inputCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Number, uiManager, uiEnvironment, L"Number", position)));
	inputCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Range, uiManager, uiEnvironment, L"Range", position)));
	createCommandGroup->AddChildCommand (inputCommandGroup);

	NUIE::GroupCommandPtr arithmeticCommandGroup (new NUIE::GroupCommand (L"Arithmetic Nodes"));
	arithmeticCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Addition, uiManager, uiEnvironment, L"Addition", position)));
	arithmeticCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Subtraction, uiManager, uiEnvironment, L"Subtraction", position)));
	arithmeticCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Multiplication, uiManager, uiEnvironment, L"Multiplication", position)));
	arithmeticCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Division, uiManager, uiEnvironment, L"Division", position)));
	createCommandGroup->AddChildCommand (arithmeticCommandGroup);

	NUIE::GroupCommandPtr drawingCommandGroup (new NUIE::GroupCommand (L"Drawing Nodes"));
	drawingCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Color, uiManager, uiEnvironment, L"Color", position)));
	drawingCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Point, uiManager, uiEnvironment, L"Point", position)));
	drawingCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Line, uiManager, uiEnvironment, L"Line", position)));
	drawingCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Circle, uiManager, uiEnvironment, L"Circle", position)));
	createCommandGroup->AddChildCommand (drawingCommandGroup);

	NUIE::GroupCommandPtr otherCommandGroup (new NUIE::GroupCommand (L"Other Nodes"));
	otherCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Viewer, uiManager, uiEnvironment, L"Viewer", position)));
	createCommandGroup->AddChildCommand (otherCommandGroup);

	actualCommands.AddCommand (createCommandGroup);
	return SelectCommandFromContextMenu (panel, position, actualCommands);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::CommandStructure& commands)
{
	return SelectCommandFromContextMenu (panel, position, commands);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::OutputSlotPtr& outputSlot, const NUIE::CommandStructure& commands)
{
	return SelectCommandFromContextMenu (panel, position, commands);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::InputSlotPtr& inputSlot, const NUIE::CommandStructure& commands)
{
	return SelectCommandFromContextMenu (panel, position, commands);
}

bool AppEventHandlers::OnParameterSettings (NUIE::NodeParameterAccessorPtr paramAccessor)
{
	ParameterDialog paramDialog (panel, paramAccessor);
	if (paramDialog.ShowModal () == ParameterDialog::DialogIds::OkButtonId) {
		return true;
	}
	return false;
}

NodeEditorUIEnvironment::NodeEditorUIEnvironment (NodeEditorControl* nodeEditorControl, NE::EvaluationEnv& evaluationEnv) :
	nodeEditorControl (nodeEditorControl),
	evaluationEnv (evaluationEnv),
	drawingContext (CreateNativeDrawingContext ()),
	skinParams (),
	eventHandlers (nodeEditorControl)
{
	drawingContext->Init (GetNativeHandle (nodeEditorControl));
}

void NodeEditorUIEnvironment::OnPaint (wxPanel* panel, wxPaintEvent& evt)
{
	drawingContext->Blit (GetNativeHandle (nodeEditorControl));
}

void NodeEditorUIEnvironment::OnResize (int width, int height)
{
	drawingContext->Resize (width, height);
}

NUIE::DrawingContext& NodeEditorUIEnvironment::GetDrawingContext ()
{
	return *drawingContext;
}

NUIE::SkinParams& NodeEditorUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NE::EvaluationEnv& NodeEditorUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void NodeEditorUIEnvironment::OnValuesRecalculated ()
{
	nodeEditorControl->RedrawResultImage ();
}

void NodeEditorUIEnvironment::OnRedrawRequest ()
{
	nodeEditorControl->Refresh (false);
}

NUIE::EventHandlers& NodeEditorUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

void NodeEditorUIEnvironment::OnSelectionChanged ()
{

}

NodeEditorControl::NodeEditorControl (wxWindow *parent, UpdateInterface& updateInterface, NE::EvaluationEnv& evaluationEnv) :
	wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
	updateInterface (updateInterface),
	captureHandler (this),
	uiEnvironment (this, evaluationEnv),
	nodeEditor (uiEnvironment)
{
	NUIE::NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();

	static const bool isStressTest = false;
	if (isStressTest) {
		static int count = 10;
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < count; j++) {
				uiManager.AddNode (NUIE::UINodePtr (new NUIE::NumberRangeNode (L"Range", NUIE::Point (i * 150, j * 150))), uiEnvironment.GetEvaluationEnv ());
			}
		}
		nodeEditor.Update ();
	} else {
		NUIE::UINodePtr startInputNode = uiManager.AddNode (NUIE::UINodePtr (new NUIE::NumericUpDownNode (L"Number", NUIE::Point (70, 70), 20, 5)), uiEnvironment.GetEvaluationEnv ());
		NUIE::UINodePtr stepInputNode = uiManager.AddNode (NUIE::UINodePtr (new NUIE::NumericUpDownNode (L"Number", NUIE::Point (70, 180), 20, 5)), uiEnvironment.GetEvaluationEnv ());
		NUIE::UINodePtr intRangeNodeX = uiManager.AddNode (NUIE::UINodePtr (new NUIE::NumberRangeNode (L"Range", NUIE::Point (220, 100))), uiEnvironment.GetEvaluationEnv ());
		NUIE::UINodePtr inputNodeY = uiManager.AddNode (NUIE::UINodePtr (new NUIE::NumericUpDownNode (L"Number", NUIE::Point (220, 220), 20, 5)), uiEnvironment.GetEvaluationEnv ());
		std::shared_ptr<PointNode> pointNode (new PointNode (L"Point", NUIE::Point (400, 150)));
		uiManager.AddNode (pointNode, uiEnvironment.GetEvaluationEnv ());
		pointNode->SetValueCombinationMode (NE::ValueCombinationMode::CrossProduct);
		NUIE::UINodePtr viewerNode = uiManager.AddNode (NUIE::UINodePtr (new NUIE::MultiLineViewerNode (L"Viewer", NUIE::Point (600, 150), 5)), uiEnvironment.GetEvaluationEnv ());

		uiManager.ConnectOutputSlotToInputSlot (startInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("start")));
		uiManager.ConnectOutputSlotToInputSlot (stepInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("step")));
		uiManager.ConnectOutputSlotToInputSlot (intRangeNodeX->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("x")));
		uiManager.ConnectOutputSlotToInputSlot (inputNodeY->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("y")));
		uiManager.ConnectOutputSlotToInputSlot (pointNode->GetUIOutputSlot (NE::SlotId ("point")), viewerNode->GetUIInputSlot (NE::SlotId ("in")));
		nodeEditor.Update ();
	}
}

void NodeEditorControl::OnPaint (wxPaintEvent& evt)
{
	nodeEditor.Draw ();
	uiEnvironment.OnPaint (this, evt);
}

void NodeEditorControl::OnResize (wxSizeEvent& evt)
{
	wxSize size = evt.GetSize ();
	uiEnvironment.OnResize (size.GetWidth (), size.GetHeight ());
}

void NodeEditorControl::OnMouseCaptureLost (wxMouseCaptureLostEvent& evt)
{
	captureHandler.OnCaptureLost ();
}

void NodeEditorControl::OnLeftButtonDown (wxMouseEvent& evt)
{
	captureHandler.OnMouseDown ();
	nodeEditor.OnMouseDown (GetKeysFromEvent (evt), NUIE::MouseButton::Left, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnLeftButtonUp (wxMouseEvent& evt)
{
	nodeEditor.OnMouseUp (GetKeysFromEvent (evt), NUIE::MouseButton::Left, evt.GetX (), evt.GetY ());
	captureHandler.OnMouseUp ();
}

void NodeEditorControl::OnLeftButtonDoubleClick (wxMouseEvent& evt)
{
	nodeEditor.OnMouseDoubleClick (GetKeysFromEvent (evt), NUIE::MouseButton::Left, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnRightButtonDown (wxMouseEvent& evt)
{
	captureHandler.OnMouseDown ();
	nodeEditor.OnMouseDown (GetKeysFromEvent (evt), NUIE::MouseButton::Right, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnRightButtonUp (wxMouseEvent& evt)
{
	nodeEditor.OnMouseUp (GetKeysFromEvent (evt), NUIE::MouseButton::Right, evt.GetX (), evt.GetY ());
	captureHandler.OnMouseUp ();
}

void NodeEditorControl::OnRightButtonDoubleClick (wxMouseEvent& evt)
{
	nodeEditor.OnMouseDoubleClick (GetKeysFromEvent (evt), NUIE::MouseButton::Right, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnMiddleButtonDown (wxMouseEvent& evt)
{
	captureHandler.OnMouseDown ();
	nodeEditor.OnMouseDown (GetKeysFromEvent (evt), NUIE::MouseButton::Middle, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnMiddleButtonUp (wxMouseEvent& evt)
{
	nodeEditor.OnMouseUp (GetKeysFromEvent (evt), NUIE::MouseButton::Middle, evt.GetX (), evt.GetY ());
	captureHandler.OnMouseUp ();
}

void NodeEditorControl::OnMiddleButtonDoubleClick (wxMouseEvent& evt)
{
	nodeEditor.OnMouseDoubleClick (GetKeysFromEvent (evt), NUIE::MouseButton::Middle, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnMouseMove (wxMouseEvent& evt)
{
	nodeEditor.OnMouseMove (GetKeysFromEvent (evt), evt.GetX (), evt.GetY ());
}

void NodeEditorControl::OnMouseWheel (wxMouseEvent& evt)
{
	NUIE::MouseWheelRotation rotation = evt.GetWheelRotation () > 0 ? NUIE::MouseWheelRotation::Forward : NUIE::MouseWheelRotation::Backward;
	nodeEditor.OnMouseWheel (GetKeysFromEvent (evt), rotation, evt.GetX (), evt.GetY ());
}

void NodeEditorControl::New ()
{
	nodeEditor.Clear ();
}
 
bool NodeEditorControl::Open (const std::wstring& fileName)
{
	if (!nodeEditor.Load (fileName)) {
		return false;
	}
	return true;
}
 
bool NodeEditorControl::Save (const std::wstring& fileName)
{
	if (!nodeEditor.Save (fileName)) {
		return false;
	}
	return true;
}

void NodeEditorControl::RedrawResultImage ()
{
	updateInterface.RedrawResultImage ();
}

BEGIN_EVENT_TABLE(NodeEditorControl, wxPanel)

EVT_PAINT (NodeEditorControl::OnPaint)
EVT_SIZE (NodeEditorControl::OnResize)
EVT_MOUSE_CAPTURE_LOST (NodeEditorControl::OnMouseCaptureLost)

EVT_LEFT_DOWN (NodeEditorControl::OnLeftButtonDown)
EVT_LEFT_UP (NodeEditorControl::OnLeftButtonUp)
EVT_LEFT_DCLICK (NodeEditorControl::OnLeftButtonDoubleClick)

EVT_RIGHT_DOWN (NodeEditorControl::OnRightButtonDown)
EVT_RIGHT_UP (NodeEditorControl::OnRightButtonUp)
EVT_RIGHT_DCLICK (NodeEditorControl::OnRightButtonDoubleClick)

EVT_MIDDLE_DOWN (NodeEditorControl::OnMiddleButtonDown)
EVT_MIDDLE_UP (NodeEditorControl::OnMiddleButtonUp)
EVT_MIDDLE_DCLICK (NodeEditorControl::OnMiddleButtonDoubleClick)

EVT_MOUSEWHEEL (NodeEditorControl::OnMouseWheel)
EVT_MOTION (NodeEditorControl::OnMouseMove)

END_EVENT_TABLE ()
