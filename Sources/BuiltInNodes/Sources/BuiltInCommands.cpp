#include "BuiltInCommands.hpp"

namespace NUIE
{

CreateNodeCommand::CreateNodeCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& viewPosition) :
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
	const ViewBox& viewBox = uiManager.GetViewBox ();
	uiManager.AddNode (CreateNode (viewBox.ViewToModel (viewPosition)), uiEnvironment.GetEvaluationEnv ());
}

}
