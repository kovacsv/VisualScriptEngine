#include "NodeEditor.hpp"
#include "MemoryStream.hpp"

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

void NodeEditor::OnMouseDown (const KeySet& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseDown (uiEnvironment, keys, button, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseUp (const KeySet& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseUp (uiEnvironment, keys, button, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseMove (const KeySet& keys, int x, int y)
{
	mouseEventTranslator.OnMouseMove (uiEnvironment, keys, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseWheel (const KeySet& keys, MouseWheelRotation rotation, int x, int y)
{
	mouseEventTranslator.OnMouseWheel (uiEnvironment, keys, rotation, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnMouseDoubleClick (const KeySet& keys, MouseButton button, int x, int y)
{
	mouseEventTranslator.OnMouseDoubleClick (uiEnvironment, keys, button, Point (x, y));
	uiManager.Update (uiEnvironment);
}

void NodeEditor::OnResize (int newWidth, int newHeight)
{
	uiManager.ResizeContext (uiEnvironment, newWidth, newHeight);
	uiManager.Update (uiEnvironment);
}

void NodeEditor::InvalidateAllNodesDrawing ()
{
	uiManager.InvalidateAllNodesDrawing ();
}

void NodeEditor::Update ()
{
	uiManager.Update (uiEnvironment);
}

void NodeEditor::Draw ()
{
	uiManager.Draw (uiEnvironment, uiInteractionHandler.GetDrawingExtension ());
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

}
