#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeCommonMenuCommands.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_VersionCompatibility.hpp"
#include "NE_MemoryStream.hpp"

#include <fstream>

namespace NUIE
{

static const std::string NodeEditorFileMarker = "NodeEditorFile";

NodeEditor::NodeEditor (NodeUIEnvironment& uiEnvironment) :
	uiManager (uiEnvironment),
	interactionHandler (uiManager),
	mouseEventTranslator (interactionHandler),
	uiEnvironment (uiEnvironment)
{

}

NodeEditor::~NodeEditor ()
{

}

void NodeEditor::OnMouseDown (const ModifierKeys& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseDown (uiEnvironment, keys, button, Point (x, y));
	Update ();
}

void NodeEditor::OnMouseUp (const ModifierKeys& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseUp (uiEnvironment, keys, button, Point (x, y));
	Update ();
}

void NodeEditor::OnMouseMove (const ModifierKeys& keys, int x, int y)
{
	mouseEventTranslator.OnMouseMove (uiEnvironment, keys, Point (x, y));
	Update ();
}

void NodeEditor::OnMouseWheel (const ModifierKeys& keys, MouseWheelRotation rotation, int x, int y)
{
	mouseEventTranslator.OnMouseWheel (uiEnvironment, keys, rotation, Point (x, y));
	Update ();
}

void NodeEditor::OnMouseDoubleClick (const ModifierKeys& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseDoubleClick (uiEnvironment, keys, button, Point (x, y));
	Update ();
}

void NodeEditor::OnKeyPress (const Key& pressedKey)
{
	interactionHandler.HandleKeyPress (uiEnvironment, pressedKey);
	Update ();
}

void NodeEditor::OnResize (int newWidth, int newHeight)
{
	uiManager.ResizeContext (uiEnvironment, newWidth, newHeight);
	Update ();
}

NodeEditor::UpdateMode NodeEditor::GetUpdateMode () const
{
	switch (uiManager.GetUpdateMode ()) {
		case NodeUIManager::UpdateMode::Automatic:
			return UpdateMode::Automatic;
		case NodeUIManager::UpdateMode::Manual:
			return UpdateMode::Manual;
	}
	DBGBREAK ();
	return UpdateMode::Automatic;
}

void NodeEditor::SetUpdateMode (UpdateMode newUpdateMode)
{
	switch (newUpdateMode) {
		case UpdateMode::Automatic:
			ManualUpdate ();
			uiManager.SetUpdateMode (NodeUIManager::UpdateMode::Automatic);
			break;
		case UpdateMode::Manual:
			uiManager.SetUpdateMode (NodeUIManager::UpdateMode::Manual);
			break;
		default:
			DBGBREAK ();
			break;
	}
}

void NodeEditor::Update ()
{
	uiManager.Update (uiEnvironment);
}

void NodeEditor::ManualUpdate ()
{
	uiManager.RequestRecalculateAndRedraw ();
	uiManager.ManualUpdate (uiEnvironment);
}

void NodeEditor::Draw ()
{
	uiManager.Draw (uiEnvironment, interactionHandler.GetDrawingModifier ());
}

void NodeEditor::AddNode (const UINodePtr& uiNode)
{
	AddNodeCommand command (uiNode, uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
	Update ();
}

void NodeEditor::ConnectOutputSlotToInputSlot (const UIOutputSlotConstPtr& outputSlot, const UIInputSlotConstPtr& inputSlot)
{
	ConnectSlotsCommand command (outputSlot, inputSlot);
	uiManager.ExecuteCommand (command);
	Update ();
}

Point NodeEditor::ViewToModel (const Point& viewPoint) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	return viewBox.ViewToModel (viewPoint);
}

void NodeEditor::AlignToWindow ()
{
	uiManager.AlignToWindow (uiEnvironment);
	Update ();
}

void NodeEditor::CenterToWindow ()
{
	uiManager.CenterToWindow (uiEnvironment);
	Update ();
}

void NodeEditor::FitToWindow ()
{
	uiManager.FitToWindow (uiEnvironment);
	Update ();
}

const ViewBox& NodeEditor::GetViewBox () const
{
	return uiManager.GetViewBox ();
}

void NodeEditor::SetViewBox (const ViewBox& newViewBox)
{
	uiManager.SetViewBox (newViewBox);
	Update ();
}

const NE::NodeCollection& NodeEditor::GetSelectedNodes () const
{
	return uiManager.GetSelectedNodes ();
}

void NodeEditor::SetSelectedNodes (const NE::NodeCollection& newSelectedNodes)
{
	uiManager.SetSelectedNodes (newSelectedNodes);
	Update ();
}

void NodeEditor::New ()
{
	uiManager.New (uiEnvironment);
	Update ();
}

bool NodeEditor::Open (const std::wstring& fileName)
{
	std::vector<char> buffer;
	if (DBGERROR (!ReadBufferFromFile (fileName, buffer))) {
		return false;
	}

	NE::MemoryInputStream inputStream (buffer);
	return Open (inputStream);
}

bool NodeEditor::Open (NE::InputStream& inputStream)
{
	std::string fileMarker;
	inputStream.Read (fileMarker);
	if (fileMarker != NodeEditorFileMarker) {
		return false;
	}

	Version readVersion;
	readVersion.Read (inputStream);
	if (!IsCompatibleVersion (readVersion)) {
		return false;
	}

	if (DBGERROR (!uiManager.Open (uiEnvironment, inputStream))) {
		return false;
	}

	Update ();
	return true;
}

bool NodeEditor::Save (const std::wstring& fileName)
{
	NE::MemoryOutputStream outputStream;
	if (DBGERROR (!Save (outputStream))) {
		return false;
	}

	const std::vector<char>& buffer = outputStream.GetBuffer ();
	if (DBGERROR (!WriteBufferToFile (fileName, buffer))) {
		return false;
	}

	return true;
}

bool NodeEditor::Save (NE::OutputStream& outputStream)
{
	const Version& currentVersion = GetCurrentVersion ();
	outputStream.Write (NodeEditorFileMarker);
	currentVersion.Write (outputStream);
	if (DBGERROR (!uiManager.Save (outputStream))) {
		return false;
	}
	return true;
}

bool NodeEditor::NeedToSave () const
{
	return uiManager.NeedToSave ();
}

void NodeEditor::ExecuteMenuCommand (const MenuCommandPtr& command)
{
	command->Do ();
	Update ();
}

void NodeEditor::ApplyParameterChanges (const ParameterInterfacePtr& parameters)
{
	ApplyParametersCommand command (parameters, uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
	Update ();
}

void NodeEditor::SetSelectedNodesParameters ()
{
	const NE::NodeCollection& selectedNodes = GetSelectedNodes ();
	if (selectedNodes.IsEmpty ()) {
		return;
	}

	size_t nodeCount = selectedNodes.Count ();
	UINodePtr currentNode = uiManager.GetUINode (selectedNodes.Get (nodeCount - 1));
	MenuCommandPtr command (new SetParametersMenuCommand (uiManager, uiEnvironment, currentNode, selectedNodes));
	ExecuteMenuCommand (command);
}

void NodeEditor::GroupSelectedNodes ()
{
	const NE::NodeCollection& selectedNodes = GetSelectedNodes ();
	if (selectedNodes.IsEmpty ()) {
		return;
	}

	MenuCommandPtr command (new CreateGroupMenuCommand (uiManager, selectedNodes));
	ExecuteMenuCommand (command);
}

void NodeEditor::UngroupSelectedNodes ()
{
	const NE::NodeCollection& selectedNodes = GetSelectedNodes ();
	if (selectedNodes.IsEmpty ()) {
		return;
	}

	MenuCommandPtr command (new RemoveNodesFromGroupMenuCommand (uiManager, selectedNodes));
	ExecuteMenuCommand (command);
}

void NodeEditor::Undo ()
{
	UndoCommand command (uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
	Update ();
}

void NodeEditor::Redo ()
{
	RedoCommand command (uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
	Update ();
}

}
