#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NE_MemoryStream.hpp"

#include <fstream>

namespace NUIE
{

static const std::string NodeEditorFileMarker = "NodeEditorFile";
static const size_t NodeEditorFileVersion = 1;
static bool GetBoundingRect (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, Rect& boundingRect)
{
	class StaticNodeRectGetter : public NodeRectGetter
	{
	public:
		StaticNodeRectGetter (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) :
			uiManager (uiManager),
			env (env)
		{
		}

		virtual Rect GetNodeRect (const NE::NodeId& nodeId) const override
		{
			UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
			return uiNode->GetNodeRect (env);
		}

	private:
		const NodeUIManager& uiManager;
		NodeUIDrawingEnvironment& env;
	};

	bool isEmptyRect = true;
	uiManager.EnumerateUINodes ([&] (const UINodeConstPtr& uiNode) {
		Rect nodeRect = uiNode->GetNodeRect (env);
		if (isEmptyRect) {
			boundingRect = nodeRect;
			isEmptyRect = false;
		} else {
			boundingRect = Rect::FromTwoPoints (
				Point (std::min (nodeRect.GetLeft (), boundingRect.GetLeft ()), std::min (nodeRect.GetTop (), boundingRect.GetTop ())),
				Point (std::max (nodeRect.GetRight (), boundingRect.GetRight ()), std::max (nodeRect.GetBottom (), boundingRect.GetBottom ()))
			);
		}
		return true;
	});

	if (isEmptyRect) {
		return false;
	}

	StaticNodeRectGetter nodeRectGetter (uiManager, env);
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupConstPtr& uiGroup) {
		Rect groupRect = uiGroup->GetRect (env, nodeRectGetter);
		boundingRect = Rect::FromTwoPoints (
			Point (std::min (groupRect.GetLeft (), boundingRect.GetLeft ()), std::min (groupRect.GetTop (), boundingRect.GetTop ())),
			Point (std::max (groupRect.GetRight (), boundingRect.GetRight ()), std::max (groupRect.GetBottom (), boundingRect.GetBottom ()))
		);
		return true;
	});

	return true;
}

NodeEditor::NodeEditor (NodeUIEnvironment& uiEnvironment) :
	uiManager (),
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
	interactionHandler.HandleKeyPress (uiEnvironment, pressedKey);
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
	uiManager.Draw (uiEnvironment, interactionHandler.GetDrawingModifier ());
}

void NodeEditor::AddNode (const UINodePtr& uiNode)
{
	uiManager.SaveUndoState ();
	uiManager.AddNode (uiNode, uiEnvironment.GetEvaluationEnv ());
	Update ();
}

Point NodeEditor::ViewToModel (const Point& viewPoint) const
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	return viewBox.ViewToModel (viewPoint);
}

void NodeEditor::FitToWindow ()
{
	Rect boundingRect;
	if (!GetBoundingRect (uiManager, uiEnvironment, boundingRect)) {
		return;
	}

	double viewPadding = uiEnvironment.GetSkinParams ().GetNodePadding ();

	const DrawingContext& drawingContext = uiEnvironment.GetDrawingContext ();
	Size contextSize (drawingContext.GetWidth (), drawingContext.GetHeight ());
	ViewBox newViewBox = FitRectToSize (contextSize, viewPadding, boundingRect);
	uiManager.SetViewBox (newViewBox);
	Update ();
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

bool NodeEditor::Load (const std::string& fileName)
{
	std::ifstream file;
	file.open (fileName, std::ios::binary);
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

bool NodeEditor::Save (const std::string& fileName) const
{
	NE::MemoryOutputStream outputStream;
	outputStream.Write (NodeEditorFileMarker);
	outputStream.Write (NodeEditorFileVersion);
	if (DBGERROR (!uiManager.Save (outputStream))) {
		return false;
	}

	std::ofstream file;
	file.open (fileName, std::ios::binary);
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
	interactionHandler.ExecuteCommand (command);
	Update ();
}

void NodeEditor::Redo ()
{
	UICommandPtr command (new RedoCommand (uiManager, uiEnvironment));
	interactionHandler.ExecuteCommand (command);
	Update ();
}

}
