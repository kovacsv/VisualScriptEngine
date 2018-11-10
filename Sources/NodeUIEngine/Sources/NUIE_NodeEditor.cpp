#include "NUIE_NodeEditor.hpp"
#include "NUIE_UINodeCommandStructure.hpp"
#include "NE_MemoryStream.hpp"

#include <fstream>
#include <locale>
#include <codecvt>

namespace NUIE
{

static const std::string NodeEditorFileMarker = "NodeEditorFile";
static const size_t NodeEditorFileVersion = 1;

static std::string WideStringToNormalString (const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes (str);
}

NodeEditor::NodeEditor (NodeUIEnvironment& uiEnvironment) :
	uiManager (),
	uiInteractionHandler (uiManager),
	mouseEventTranslator (uiInteractionHandler),
	uiEnvironment (uiEnvironment)
{

}

NodeEditor::~NodeEditor ()
{

}

void NodeEditor::OnMouseDown (const ModifierKeys& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseDown (uiEnvironment, keys, button, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseUp (const ModifierKeys& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseUp (uiEnvironment, keys, button, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseMove (const ModifierKeys& keys, int x, int y)
{
	mouseEventTranslator.OnMouseMove (uiEnvironment, keys, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseWheel (const ModifierKeys& keys, MouseWheelRotation rotation, int x, int y)
{
	mouseEventTranslator.OnMouseWheel (uiEnvironment, keys, rotation, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseDoubleClick (const ModifierKeys& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseDoubleClick (uiEnvironment, keys, button, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnKeyPress (const Key& pressedKey)
{
	uiInteractionHandler.HandleKeyPress (uiEnvironment, pressedKey);
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnResize (int newWidth, int newHeight)
{
	uiManager.ResizeContext (uiEnvironment, newWidth, newHeight);
	uiManager.Update (uiEnvironment);
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
	uiManager.RequestRecalculate ();
	uiManager.RequestRedraw ();
	uiManager.ManualUpdate (uiEnvironment);
}

void NodeEditor::Draw ()
{
	uiManager.Draw (uiEnvironment, uiInteractionHandler.GetDrawingModifier ());
}

void NodeEditor::AddNode (const UINodePtr& uiNode)
{
	uiManager.AddNode (uiNode, uiEnvironment.GetEvaluationEnv ());
	Update ();
}

Point NodeEditor::GetCenterPoint () const
{
	DrawingContext& drawingContext = uiEnvironment.GetDrawingContext ();
	Size screenSize (drawingContext.GetWidth (), drawingContext.GetHeight ());
	Rect screenRect = Rect::FromPositionAndSize (Point (0, 0), screenSize);
	return uiManager.GetViewBox ().ViewToModel (screenRect.GetCenter ());
}

NodeUIManager& NodeEditor::GetNodeUIManager ()
{
	return uiManager;
}

void NodeEditor::Clear ()
{
	uiManager.Clear ();
	Update ();
}

bool NodeEditor::Load (const std::wstring& fileName)
{
	std::ifstream file;
	file.open (WideStringToNormalString (fileName), std::ios::binary);
	if (DBGERROR (!file.is_open ())) {
		return false;
	}

	std::vector<char> buffer;
	buffer.assign (std::istreambuf_iterator<char> (file), std::istreambuf_iterator<char> ());
	file.close ();

	NE::MemoryInputStream inputStream (buffer);
	
	std::string fileMarker;
	inputStream.Read (fileMarker);
	if (DBGERROR (fileMarker != NodeEditorFileMarker)) {
		return false;
	}

	size_t fileVersion = 0;
	inputStream.Read (fileVersion);
	if (DBGERROR (fileVersion != NodeEditorFileVersion)) {
		return false;
	}

	if (DBGERROR (!uiManager.Load (inputStream))) {
		return false;
	}

	Update ();
	return true;
}

bool NodeEditor::Save (const std::wstring& fileName) const
{
	NE::MemoryOutputStream outputStream;
	outputStream.Write (NodeEditorFileMarker);
	outputStream.Write (NodeEditorFileVersion);
	if (DBGERROR (!uiManager.Save (outputStream))) {
		return false;
	}

	std::ofstream file;
	file.open (WideStringToNormalString (fileName), std::ios::binary);
	if (DBGERROR (!file.is_open ())) {
		return false;
	}

	const std::vector<char>& buffer = outputStream.GetBuffer ();
	file.write (buffer.data (), buffer.size ());
	file.close ();

	return true;
}

void NodeEditor::Undo ()
{
	UICommandPtr command (new UndoCommand (uiManager, uiEnvironment));
	uiInteractionHandler.ExecuteCommand (command);
	Update ();
}

void NodeEditor::Redo ()
{
	UICommandPtr command (new RedoCommand (uiManager, uiEnvironment));
	uiInteractionHandler.ExecuteCommand (command);
	Update ();
}

}
