#include "BuiltInCommands.hpp"

namespace BI
{

CreateNodeCommand::CreateNodeCommand (const std::wstring& name, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& viewPosition) :
	SingleCommand (name, false),
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
	uiManager.AddNode (CreateNode (viewBox.ViewToModel (viewPosition)), uiEnvironment.GetEvaluationEnv ());
}

}
