#include "BI_BuiltInCommands.hpp"
#include "NUIE_NodeUIManagerCommands.hpp"

namespace BI
{

CreateNodeCommand::CreateNodeCommand (const std::wstring& name, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& viewPosition) :
	SingleMenuCommand (name, false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	viewPosition (viewPosition)
{

}

CreateNodeCommand::~CreateNodeCommand ()
{

}

void CreateNodeCommand::Do ()
{
	const NUIE::ViewBox& viewBox = uiManager.GetViewBox ();
	NUIE::UINodePtr uiNode = CreateNode (viewBox.ViewToModel (viewPosition));
	NUIE::AddNodeCommand command (uiNode, uiEnvironment.GetEvaluationEnv ());
	uiManager.ExecuteCommand (command);
}

}
