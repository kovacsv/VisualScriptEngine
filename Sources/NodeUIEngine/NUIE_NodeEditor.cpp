#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeMenuCommands.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"
#include "NUIE_Version.hpp"
#include "NE_MemoryStream.hpp"

#include <fstream>

namespace NUIE
{

static const std::string NodeEditorFileMarker = "NodeEditorFile";

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
		Rect groupRect = uiGroup->GetRect (env, nodeRectGetter, uiManager.GetUIGroupNodes (uiGroup));
		boundingRect = Rect::FromTwoPoints (
			Point (std::min (groupRect.GetLeft (), boundingRect.GetLeft ()), std::min (groupRect.GetTop (), boundingRect.GetTop ())),
			Point (std::max (groupRect.GetRight (), boundingRect.GetRight ()), std::max (groupRect.GetBottom (), boundingRect.GetBottom ()))
		);
		return true;
	});

	return true;
}

ExternalHeaderIO::~ExternalHeaderIO ()
{

}

ExternalFileIO::~ExternalFileIO ()
{

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

void NodeEditor::New ()
{
	uiManager.New ();
	Update ();
}

bool NodeEditor::Open (const std::wstring& fileName, const ExternalFileIO* externalFileIO, const ExternalHeaderIO* externalHeader)
{
	std::vector<char> buffer;
	if (DBGERROR (!externalFileIO->ReadBufferFromFile (fileName, buffer))) {
		return false;
	}

	NE::MemoryInputStream inputStream (buffer);
	return Open (inputStream, externalHeader);
}

bool NodeEditor::Open (NE::InputStream& inputStream, const ExternalHeaderIO* externalHeader)
{
	if (externalHeader != nullptr) {
		if (!externalHeader->Read (inputStream)) {
			return false;
		}
	}

	std::string fileMarker;
	inputStream.Read (fileMarker);
	if (fileMarker != NodeEditorFileMarker) {
		return false;
	}

	Version readVersion;
	readVersion.Read (inputStream);
	if (readVersion > EngineVersion) {
		return false;
	}

	int readFileVersion = 0;
	inputStream.Read (readFileVersion);
	if (readFileVersion != FileVersion) {
		return false;
	}

	if (DBGERROR (!uiManager.Open (inputStream))) {
		return false;
	}

	Update ();
	return true;
}

bool NodeEditor::Save (const std::wstring& fileName, const ExternalFileIO* externalFileIO, const ExternalHeaderIO* externalHeader) const
{
	NE::MemoryOutputStream outputStream;
	if (DBGERROR (!Save (outputStream, externalHeader))) {
		return false;
	}

	const std::vector<char>& buffer = outputStream.GetBuffer ();
	if (DBGERROR (!externalFileIO->WriteBufferToFile (fileName, buffer))) {
		return false;
	}

	return true;
}

bool NodeEditor::Save (NE::OutputStream& outputStream, const ExternalHeaderIO* externalHeader) const
{
	if (externalHeader != nullptr) {
		externalHeader->Write (outputStream);
	}

	outputStream.Write (NodeEditorFileMarker);
	EngineVersion.Write (outputStream);
	outputStream.Write (FileVersion);
	if (DBGERROR (!uiManager.Save (outputStream))) {
		return false;
	}
	return true;
}

bool NodeEditor::NeedToSave () const
{
	return uiManager.NeedToSave ();
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
