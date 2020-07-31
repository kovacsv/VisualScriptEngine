#include "VisualTestFramework.hpp"
#include "NUIE_FileIO.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "BI_BuiltInNodes.hpp"
#include "NE_StringUtils.hpp"

#include "SimpleTest.hpp"
#include "TestReference.hpp"

#include <iostream>
#include <fstream>

class IncreaseNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE(IncreaseNode);

public:
	IncreaseNode () :
		IncreaseNode (LocString (), Point ())
	{
	}

	IncreaseNode (const LocString& name, const Point& position) :
		BI::BasicUINode (name, position)
	{

	}

	virtual ~IncreaseNode ()
	{
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), LocString (L"In"), nullptr, NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), LocString (L"Out"))));
	}

	NE::ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		NE::ValueConstPtr inValue = EvaluateInputSlot (NE::SlotId ("in"), env);
		if (!IsSingleType<NE::IntValue> (inValue)) {
			return nullptr;
		}
		return NE::ValuePtr (new NE::IntValue (NE::IntValue::Get (inValue) + 1));
	}

	virtual NE::Stream::Status Read (NE::InputStream& inputStream) override
	{
		NE::ObjectHeader header (inputStream);
		BasicUINode::Read (inputStream);
		return inputStream.GetStatus ();
	}

	virtual NE::Stream::Status Write (NE::OutputStream& outputStream) const override
	{
		NE::ObjectHeader header (outputStream, serializationInfo);
		BasicUINode::Write (outputStream);
		return outputStream.GetStatus ();
	}
};

DYNAMIC_SERIALIZATION_INFO (IncreaseNode, 1, "{8C06D4A9-B042-4E23-8556-410AA5ED2B35}");

class MyCreateNodeCommand : public NUIE::SingleMenuCommand
{
public:
	enum class NodeType
	{
		Number,
		Integer,
		Addition,
		Increase,
		Viewer
	};

	MyCreateNodeCommand (NodeEditor* nodeEditor, NodeType nodeType, const NE::LocString& name, const NUIE::Point& position) :
		NUIE::SingleMenuCommand (name, false),
		nodeEditor (nodeEditor),
		nodeType (nodeType),
		position (position)
	{
	
	}

	virtual void Do () override
	{
		const NUIE::ViewBox& viewBox = nodeEditor->GetViewBox ();
		nodeEditor->AddNode (CreateNode (viewBox.ViewToModel (position)));
	}

	NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition)
	{
		switch (nodeType) {
			case NodeType::Number:
				return NUIE::UINodePtr (new BI::DoubleUpDownNode (LocString (L"Number"), modelPosition, 0.0, 5.0));
			case NodeType::Integer:
				return NUIE::UINodePtr (new BI::IntegerUpDownNode (LocString (L"Integer"), modelPosition, 0, 1));
			case NodeType::Addition:
				return NUIE::UINodePtr (new BI::AdditionNode (LocString (L"Addition"), modelPosition));
			case NodeType::Increase:
				return NUIE::UINodePtr (new IncreaseNode (LocString (L"Increase"), modelPosition));
			case NodeType::Viewer:
				return NUIE::UINodePtr (new BI::MultiLineViewerNode (LocString (L"Viewer"), modelPosition, 5));
		}
		return nullptr;
	}

private:
	NodeEditor*		nodeEditor;
	NodeType		nodeType;
	NUIE::Point		position;
};

TestEventHandler::TestEventHandler (NodeEditor* nodeEditor) :
	nodeEditor (nodeEditor),
	commandToSelect (),
	paramSettingsHandler (nullptr)
{
	
}

MenuCommandPtr TestEventHandler::OnContextMenu (const Point& position, const MenuCommandStructure& commands)
{
	NUIE::MenuCommandStructure actualCommands = commands;
	NUIE::MultiMenuCommandPtr createMultiCommand (new NUIE::MultiMenuCommand (NE::LocString (L"Add Node")));

	createMultiCommand->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (nodeEditor, MyCreateNodeCommand::NodeType::Number, NE::LocString (L"Create Number Node"), position)));
	createMultiCommand->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (nodeEditor, MyCreateNodeCommand::NodeType::Integer, NE::LocString (L"Create Integer Node"), position)));
	createMultiCommand->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (nodeEditor, MyCreateNodeCommand::NodeType::Addition, NE::LocString (L"Create Addition Node"), position)));
	createMultiCommand->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (nodeEditor, MyCreateNodeCommand::NodeType::Increase, NE::LocString (L"Create Increase Node"), position)));
	createMultiCommand->AddChildCommand (NUIE::MenuCommandPtr (new MyCreateNodeCommand (nodeEditor, MyCreateNodeCommand::NodeType::Viewer, NE::LocString (L"Create Viewer Node"), position)));
	actualCommands.AddCommand (createMultiCommand);

	return SelectCommandByName (actualCommands);
}

MenuCommandPtr TestEventHandler::OnContextMenu (const Point&, const UINodePtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

MenuCommandPtr TestEventHandler::OnContextMenu (const Point&, const UIOutputSlotConstPtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

MenuCommandPtr TestEventHandler::OnContextMenu (const Point&, const UIInputSlotConstPtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

MenuCommandPtr TestEventHandler::OnContextMenu (const Point&, const UINodeGroupPtr&, const MenuCommandStructure& commands)
{
	return SelectCommandByName (commands);
}

void TestEventHandler::OnDoubleClick (MouseButton, const Point&)
{

}

bool TestEventHandler::OnParameterSettings (ParameterInterfacePtr paramInterface, const UINodePtr&)
{
	if (DBGERROR (paramSettingsHandler == nullptr)) {
		return false;
	}
	bool result = paramSettingsHandler (paramInterface);
	paramSettingsHandler = nullptr;
	return result;
}

bool TestEventHandler::OnParameterSettings (ParameterInterfacePtr paramInterface, const UINodeGroupPtr&)
{
	if (DBGERROR (paramSettingsHandler == nullptr)) {
		return false;
	}
	bool result = paramSettingsHandler (paramInterface);
	paramSettingsHandler = nullptr;
	return result;
}

void TestEventHandler::SetNextCommandName (const std::wstring& nextCommandName)
{
	DBGASSERT (commandToSelect.empty ());
	commandToSelect = nextCommandName;
}

void TestEventHandler::SetNextCommandParameterSettings (const ParameterSettingsHandler& handler)
{
	SetNextCommandName (L"Node Settings");
	paramSettingsHandler = handler;
}

MenuCommandPtr TestEventHandler::SelectCommandByName (const MenuCommandStructure& commands)
{
	DBGASSERT (!commandToSelect.empty ());
	MenuCommandPtr selectedCommand = nullptr;
	commands.EnumerateCommands ([&] (const MenuCommandPtr& command) {
		if (selectedCommand == nullptr) {
			selectedCommand = SelectCommandByName (command);
		}
	});
	DBGASSERT (selectedCommand != nullptr);
	commandToSelect.clear ();
	return selectedCommand;
}

MenuCommandPtr TestEventHandler::SelectCommandByName (const MenuCommandPtr& command)
{
	if (command->HasChildCommands ()) {
		MenuCommandPtr foundCommand = nullptr;
		command->EnumerateChildCommands ([&] (const MenuCommandPtr& childCommand) {
			if (foundCommand == nullptr) {
				foundCommand = SelectCommandByName (childCommand);
			}
		});
		return foundCommand;
	} else {
		if (command->GetName () == commandToSelect) {
			return command;
		}
	}
	return nullptr;
}

TestNodeUIEnvironment::TestNodeUIEnvironment (NodeEditor& nodeEditor, const BasicSkinParams& skinParams) :
	NUIE::NodeUIEnvironment (),
	nodeEditor (nodeEditor),
	stringConverter (GetDefaultStringConverter ()),
	skinParams (skinParams),
	drawingContext (800, 600),
	eventHandler (&nodeEditor),
	evaluationEnv (nullptr),
	windowScale (1.0)
{
	
}

const StringConverter& TestNodeUIEnvironment::GetStringConverter ()
{
	return stringConverter;
}

const SkinParams& TestNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

DrawingContext& TestNodeUIEnvironment::GetDrawingContext ()
{
	return drawingContext;
}

double TestNodeUIEnvironment::GetWindowScale ()
{
	return windowScale;
}

EvaluationEnv& TestNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void TestNodeUIEnvironment::OnEvaluationBegin ()
{

}

void TestNodeUIEnvironment::OnEvaluationEnd ()
{

}

void TestNodeUIEnvironment::OnValuesRecalculated ()
{

}

void TestNodeUIEnvironment::OnRedrawRequested ()
{
	nodeEditor.Draw ();
}

EventHandler& TestNodeUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

ClipboardHandler& TestNodeUIEnvironment::GetClipboardHandler ()
{
	return clipboardHandler;
}

double TestNodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

void TestNodeUIEnvironment::SetNextCommandName (const std::wstring& nextCommandName)
{
	eventHandler.SetNextCommandName (nextCommandName);
}

void TestNodeUIEnvironment::SetNextCommandParameterSettings (const ParameterSettingsHandler& handler)
{
	eventHandler.SetNextCommandParameterSettings (handler);
}

const SvgDrawingContext& TestNodeUIEnvironment::GetSvgDrawingContext () const
{
	return drawingContext;
}

void TestNodeUIEnvironment::SetWindowScale (double newWindowScale)
{
	windowScale = newWindowScale;
}

NodeEditorTestEnv::NodeEditorTestEnv (const BasicSkinParams& skinParams) :
	uiEnvironment (nodeEditor, skinParams),
	nodeEditor (uiEnvironment)
{
	nodeEditor.Update ();
}

bool NodeEditorTestEnv::CheckReference (const std::wstring& referenceFileName)
{
	const SvgDrawingContext& context = uiEnvironment.GetSvgDrawingContext ();
	return CheckDrawingReference (context, referenceFileName);
}

void NodeEditorTestEnv::Click (const Point& point)
{
	nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::CtrlClick (const Point& point)
{
	nodeEditor.OnMouseDown (ModifierKeys ({ ModifierKeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (ModifierKeys ({ ModifierKeyCode::Control }), MouseButton::Left, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::RightClick (const Point& point)
{
	nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Right, (int) point.GetX (), (int) point.GetY ());
	nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Right, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::Wheel (MouseWheelRotation rotation, const Point& point)
{
	nodeEditor.OnMouseWheel (EmptyModifierKeys, rotation, (int) point.GetX (), (int) point.GetY ());
}

void NodeEditorTestEnv::DragDrop (const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp)
{
	DragDrop (EmptyModifierKeys, from, to, beforeMouseUp);
}

void NodeEditorTestEnv::DragDrop (const ModifierKeys& keys, const Point& from, const Point& to, const std::function<void ()>& beforeMouseUp)
{
	nodeEditor.OnMouseDown (keys, MouseButton::Left, (int) from.GetX (), (int) from.GetY ());
	nodeEditor.OnMouseMove (keys, (int) to.GetX (), (int) to.GetY ());
	if (beforeMouseUp != nullptr) {
		beforeMouseUp ();
	}
	nodeEditor.OnMouseUp (keys, MouseButton::Left, (int) to.GetX (), (int) to.GetY ());
}

void NodeEditorTestEnv::KeyPress (const NUIE::Key& key)
{
	nodeEditor.OnKeyPress (key);
}

void NodeEditorTestEnv::SetNextCommandName (const std::wstring& nextCommandName)
{
	uiEnvironment.SetNextCommandName (nextCommandName);
}

void NodeEditorTestEnv::SetNextCommandParameterSettings (const ParameterSettingsHandler& handler)
{
	uiEnvironment.SetNextCommandParameterSettings (handler);
}
