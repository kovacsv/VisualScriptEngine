#include "BI_BuiltInCommands.hpp"

namespace BI
{

CreateNodeCommand::CreateNodeCommand (const std::wstring& name, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& viewPosition) :
	SingleUICommand (name, false),
	uiManager (uiManager),
	uiEnvironment (uiEnvironment),
	viewPosition (viewPosition)
{

}

CreateNodeCommand::~CreateNodeCommand ()
{

}

bool CreateNodeCommand::IsUndoable () const
{
	return true;
}

void CreateNodeCommand::Do ()
{
	const NUIE::ViewBox& viewBox = uiManager.GetViewBox ();
	uiManager.AddNode (CreateNode (viewBox.ViewToModel (viewPosition)), uiEnvironment.GetEvaluationEnv ());
}

}
