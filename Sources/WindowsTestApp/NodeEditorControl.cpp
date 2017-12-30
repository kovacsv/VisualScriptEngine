#include "NodeEditorControl.hpp"
#include "BitmapContextGdi.hpp"
#include "BitmapContextGdiplus.hpp"
#include "Direct2DContext.hpp"

#include "InputUINodes.hpp"
#include "ViewerUINodes.hpp"
#include "TestAppNodes.hpp"

MyCreateNodeCommand::MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position) :
	NUIE::CreateNodeCommand (name, uiManager, uiEnvironment, position),
	nodeType (nodeType)
{

}

NUIE::UINodePtr MyCreateNodeCommand::CreateNode (const NUIE::Point& modelPosition)
{
	switch (nodeType) {
	case NodeType::Integer:
		return NUIE::UINodePtr (new NUIE::IntegerUpDownUINode (L"Integer", modelPosition, 0, 10));
	case NodeType::IntegerRange:
		return NUIE::UINodePtr (new NUIE::IntegerRangeNode (L"Range", modelPosition));
	case NodeType::Point:
		return NUIE::UINodePtr (new PointNode (L"Point", modelPosition));
	case NodeType::Line:
		return NUIE::UINodePtr (new LineNode (L"Line", modelPosition));
	case NodeType::Circle:
		return NUIE::UINodePtr (new CircleNode (L"Circle", modelPosition));
	case NodeType::Viewer:
		return NUIE::UINodePtr (new NUIE::MultiLineViewerUINode (L"Viewer", modelPosition, 5));
	}
	return nullptr;
}

AppEventHandlers::AppEventHandlers () :
	hwnd (NULL)
{

}

void AppEventHandlers::SetWindowHandle (HWND newHwnd)
{
	hwnd = newHwnd;
}

void AppEventHandlers::RedrawRequested ()
{
	InvalidateRect (hwnd, NULL, FALSE);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::CommandStructure& commands)
{
	NUIE::CommandStructure actualCommands = commands;
	NUIE::GroupCommandPtr createCommandGroup (new NUIE::GroupCommand (L"Add Node"));
	createCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Integer, uiManager, uiEnvironment, L"Integer", position)));
	createCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::IntegerRange, uiManager, uiEnvironment, L"Range", position)));
	createCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Point, uiManager, uiEnvironment, L"Point", position)));
	createCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Line, uiManager, uiEnvironment, L"Line", position)));
	createCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Circle, uiManager, uiEnvironment, L"Circle", position)));
	createCommandGroup->AddChildCommand (NUIE::CommandPtr (new MyCreateNodeCommand (MyCreateNodeCommand::NodeType::Viewer, uiManager, uiEnvironment, L"Viewer", position)));
	actualCommands.AddCommand (createCommandGroup);
	return UI::SelectCommandFromContextMenu (hwnd, position, actualCommands);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::CommandStructure& commands)
{
	return UI::SelectCommandFromContextMenu (hwnd, position, commands);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::OutputSlotPtr& outputSlot, const NUIE::CommandStructure& commands)
{
	return UI::SelectCommandFromContextMenu (hwnd, position, commands);
}

NUIE::CommandPtr AppEventHandlers::OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::InputSlotPtr& inputSlot, const NUIE::CommandStructure& commands)
{
	return UI::SelectCommandFromContextMenu (hwnd, position, commands);
}

MyNodeUIEnvironment::MyNodeUIEnvironment (const std::shared_ptr<ResultImageEvaluationData>& evaluationData) :
	NUIE::NodeUIEnvironment (),
	drawingContext (nullptr),
	skinParams (),
	eventHandlers (),
	evaluationEnv (evaluationData)
{
	drawingContext.reset (new Direct2DContext ());
}

NUIE::DrawingContext& MyNodeUIEnvironment::GetDrawingContext ()
{
	return *drawingContext.get ();
}

NUIE::SkinParams& MyNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NUIE::EventHandlers& MyNodeUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

NE::EvaluationEnv& MyNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void MyNodeUIEnvironment::Init (HWND hwnd)
{
	drawingContext->Init (hwnd);
	eventHandlers.SetWindowHandle (hwnd);
}

void MyNodeUIEnvironment::DrawToHDC (HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	drawingContext->DrawToHDC (hdc);
	EndPaint (hwnd, &ps);
}

void MyNodeUIEnvironment::ChangeContext (HWND hwnd, short contextType)
{
	// TODO: Drawing invalidate needed
	switch (contextType) {
		case 1:
			drawingContext.reset (new BitmapContextGdi ());
			break;
		case 2:
			drawingContext.reset (new BitmapContextGdiplus ());
			break;
		case 3:
			drawingContext.reset (new Direct2DContext ());
			break;
		default:
			DBGBREAK ();
			break;
	}
	drawingContext->Init (hwnd);
}

NodeEditorControl::NodeEditorControl (const std::shared_ptr<ResultImageEvaluationData>& evaluationData) :
	CustomControl (),
	uiEnvironment (evaluationData),
	nodeEditor (uiEnvironment)
{

}

void NodeEditorControl::OnCreate (HWND hwnd)
{
	uiEnvironment.Init (hwnd);

	NUIE::NodeUIManager& uiManager = nodeEditor.GetNodeUIManager ();

	static const bool isStressTest = false;
	if (isStressTest) {
		static int count = 10;
		for (int i = 0; i < count; i++) {
			for (int j = 0; j < count; j++) {
				uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerRangeNode (L"Range", NUIE::Point (i * 150, j * 150))), uiEnvironment.GetEvaluationEnv ());
			}
		}
		nodeEditor.Update ();
	} else {
		NUIE::UINodePtr startInputNode = uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerUpDownUINode (L"Integer", NUIE::Point (70, 70), 20, 10)), uiEnvironment.GetEvaluationEnv ());
		NUIE::UINodePtr stepInputNode = uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerUpDownUINode (L"Integer", NUIE::Point (70, 180), 20, 10)), uiEnvironment.GetEvaluationEnv ());
		NUIE::UINodePtr intRangeNodeX = uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerRangeNode (L"Range", NUIE::Point (200, 100))), uiEnvironment.GetEvaluationEnv ());
		NUIE::UINodePtr inputNodeY = uiManager.AddNode (NUIE::UINodePtr (new NUIE::IntegerUpDownUINode (L"Integer", NUIE::Point (200, 220), 20, 10)), uiEnvironment.GetEvaluationEnv ());
		std::shared_ptr<PointNode> pointNode (new PointNode (L"Point", NUIE::Point (400, 150)));
		uiManager.AddNode (pointNode, uiEnvironment.GetEvaluationEnv ());
		pointNode->SetValueCombinationMode (NE::ValueCombinationMode::CrossProduct);
		NUIE::UINodePtr viewerNode = uiManager.AddNode (NUIE::UINodePtr (new NUIE::MultiLineViewerUINode (L"Viewer", NUIE::Point (600, 150), 5)), uiEnvironment.GetEvaluationEnv ());

		uiManager.ConnectOutputSlotToInputSlot (startInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("start")));
		uiManager.ConnectOutputSlotToInputSlot (stepInputNode->GetUIOutputSlot (NE::SlotId ("out")), intRangeNodeX->GetUIInputSlot (NE::SlotId ("step")));
		uiManager.ConnectOutputSlotToInputSlot (intRangeNodeX->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("x")));
		uiManager.ConnectOutputSlotToInputSlot (inputNodeY->GetUIOutputSlot (NE::SlotId ("out")), pointNode->GetUIInputSlot (NE::SlotId ("y")));
		uiManager.ConnectOutputSlotToInputSlot (pointNode->GetUIOutputSlot (NE::SlotId ("point")), viewerNode->GetUIInputSlot (NE::SlotId ("in")));
		nodeEditor.Update ();
	}
}

void NodeEditorControl::OnPaint (HWND hwnd)
{
	nodeEditor.Draw ();
	uiEnvironment.DrawToHDC (hwnd);
}

void NodeEditorControl::OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{
	NUIE::KeySet pressedKeys = ConvertKeys (keys);
	NUIE::MouseButton mouseButton = ConvertMouseButton (button);
	nodeEditor.OnMouseDown (pressedKeys, mouseButton, x, y);
}

void NodeEditorControl::OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{
	NUIE::KeySet pressedKeys = ConvertKeys (keys);
	NUIE::MouseButton mouseButton = ConvertMouseButton (button);
	nodeEditor.OnMouseUp (pressedKeys, mouseButton, x, y);
}

void NodeEditorControl::OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y)
{
	NUIE::KeySet pressedKeys = ConvertKeys (keys);
	nodeEditor.OnMouseMove (pressedKeys, x, y);
}

void NodeEditorControl::OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta)
{
	NUIE::KeySet pressedKeys = ConvertKeys (keys);
	NUIE::MouseWheelRotation rotation = delta > 0 ? NUIE::MouseWheelRotation::Forward : NUIE::MouseWheelRotation::Backward;
	nodeEditor.OnMouseWheel (pressedKeys, rotation, x, y);
}

void NodeEditorControl::OnMouseDoubleClick (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y)
{
	NUIE::KeySet pressedKeys = ConvertKeys (keys);
	NUIE::MouseButton mouseButton = ConvertMouseButton (button);
	nodeEditor.OnMouseDoubleClick (pressedKeys, mouseButton, x, y);
}

void NodeEditorControl::OnResize (HWND hwnd, int newWidth, int newHeight)
{
	nodeEditor.OnResize (newWidth, newHeight);
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

void NodeEditorControl::ChangeContext (short contextType)
{
	uiEnvironment.ChangeContext (windowHandle, contextType);
	nodeEditor.InvalidateAllNodesDrawing ();
	InvalidateRect (windowHandle, NULL, FALSE);
}

NUIE::KeySet NodeEditorControl::ConvertKeys (UI::Keys keys)
{
	std::unordered_set<NUIE::KeyCode> keyCodes;
	if (keys.IsKeyPressed (UI::Keys::ControlKey)) {
		keyCodes.insert (NUIE::KeyCode::Control);
	}
	if (keys.IsKeyPressed (UI::Keys::ShiftKey)) {
		keyCodes.insert (NUIE::KeyCode::Shift);
	}
	return NUIE::KeySet (keyCodes);
}

NUIE::MouseButton NodeEditorControl::ConvertMouseButton (UI::MouseButton button)
{
	NUIE::MouseButton mouseButton = NUIE::MouseButton::Left;
	switch (button) {
		case UI::MouseButton::Left: mouseButton = NUIE::MouseButton::Left; break;
		case UI::MouseButton::Middle: mouseButton = NUIE::MouseButton::Middle; break;
		case UI::MouseButton::Right: mouseButton = NUIE::MouseButton::Right; break;
	}
	return mouseButton;
}
