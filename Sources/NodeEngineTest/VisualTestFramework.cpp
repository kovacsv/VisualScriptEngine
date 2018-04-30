#include "VisualTestFramework.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "SimpleTest.hpp"

#include <iostream>
#include <fstream>

TestEventHandlers::TestEventHandlers () :
	commandToSelect ()
{
	
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const CommandStructure& commands)
{
	return SelectCommandByName (commands);
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const UINodePtr&, const CommandStructure& commands)
{
	return SelectCommandByName (commands);
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::OutputSlotPtr&, const CommandStructure& commands)
{
	return SelectCommandByName (commands);
}

CommandPtr TestEventHandlers::OnContextMenu (NodeUIManager&, NodeUIEnvironment&, const Point&, const NE::InputSlotPtr&, const CommandStructure& commands)
{
	return SelectCommandByName (commands);
}

bool TestEventHandlers::OnParameterSettings (NodeParameterAccessorPtr)
{
	DBGBREAK ();
	return false;
}

void TestEventHandlers::SetNextCommandName (const std::wstring& nextCommandName)
{
	DBGASSERT (commandToSelect.empty ());
	commandToSelect = nextCommandName;
}

CommandPtr TestEventHandlers::SelectCommandByName (const CommandStructure& commands)
{
	DBGASSERT (!commandToSelect.empty ());
	CommandPtr selectedCommand = nullptr;
	commands.EnumerateCommands ([&] (const CommandPtr& command) {
		if (selectedCommand == nullptr) {
			selectedCommand = SelectCommandByName (command);
		}
	});
	DBGASSERT (selectedCommand != nullptr);
	commandToSelect.clear ();
	return selectedCommand;
}

CommandPtr TestEventHandlers::SelectCommandByName (const CommandPtr& command)
{
	if (command->HasChildCommands ()) {
		CommandPtr foundCommand = nullptr;
		command->EnumerateChildCommands ([&] (const CommandPtr& childCommand) {
			foundCommand = SelectCommandByName (childCommand);
		});
		return foundCommand;
	} else {
		if (command->GetName () == commandToSelect) {
			return command;
		}
	}
	return nullptr;
}

TestNodeUIEnvironment::TestNodeUIEnvironment (NodeEditor& nodeEditor) :
	NUIE::NodeUIEnvironment (),
	nodeEditor (nodeEditor),
	drawingContext (800, 600),
	skinParams (),
	eventHandlers (),
	evaluationEnv (nullptr)
{
	
}

DrawingContext& TestNodeUIEnvironment::GetDrawingContext ()
{
	return drawingContext;
}

SkinParams& TestNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

EvaluationEnv& TestNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void TestNodeUIEnvironment::OnValuesRecalculated ()
{

}

void TestNodeUIEnvironment::OnRedrawRequested ()
{
	nodeEditor.Draw ();
}

EventHandlers& TestNodeUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

void TestNodeUIEnvironment::SetNextCommandName (const std::wstring& nextCommandName)
{
	eventHandlers.SetNextCommandName (nextCommandName);
}

const SvgDrawingContext& TestNodeUIEnvironment::GetSvgDrawingContext () const
{
	return drawingContext;
}

NodeEditorTestEnv::NodeEditorTestEnv () :
	uiEnvironment (nodeEditor),
	nodeEditor (uiEnvironment)
{
}

bool NodeEditorTestEnv::CheckReference (const std::string& referenceFileName)
{
	const SvgDrawingContext& context = uiEnvironment.GetSvgDrawingContext ();

	std::string testFilesPath = SimpleTest::GetAppFolderLocation () + "VisualTestFiles" + PATH_SEPARATOR;
	std::string referenceFilePath = testFilesPath + referenceFileName;
	std::wifstream referenceFile;
	referenceFile.open (referenceFilePath);
	if (!referenceFile.is_open ()) {
		context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
		return false;
	}

	std::wstringstream referenceFileBuffer;
	referenceFileBuffer << referenceFile.rdbuf ();
	std::wstring referenceContent = referenceFileBuffer.str ();
	referenceFile.close ();

	std::wstring currentContent = context.GetAsString ();
	referenceContent = ReplaceAll (referenceContent, L"\r\n", L"\n");
	currentContent = ReplaceAll (currentContent, L"\r\n", L"\n");
	if (referenceContent != currentContent) {
		std::wcout << std::endl << L"=== CURRENT ===" << std::endl;
		std::wcout << currentContent << std::endl;
		std::wcout << L"=== REFERENCE ===" << std::endl;
		std::wcout << referenceContent << std::endl;
		context.WriteToFile (testFilesPath + "Current_" + referenceFileName);
		return false;
	}
	return true;
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
	nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Left, (int) from.GetX (), (int) from.GetY ());
	nodeEditor.OnMouseMove (EmptyModifierKeys, (int) to.GetX (), (int) to.GetY ());
	if (beforeMouseUp != nullptr) {
		beforeMouseUp ();
	}
	nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Left, (int) to.GetX (), (int) to.GetY ());
}

void NodeEditorTestEnv::SetNextCommandName (const std::wstring& nextCommandName)
{
	uiEnvironment.SetNextCommandName (nextCommandName);
}
