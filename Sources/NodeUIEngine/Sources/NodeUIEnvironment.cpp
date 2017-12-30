#include "NodeUIEnvironment.hpp"

namespace NUIE
{

NodeUIEnvironment::NodeUIEnvironment ()
{

}

NodeUIEnvironment::~NodeUIEnvironment ()
{

}

NodeUIEnvironmentDecorator::NodeUIEnvironmentDecorator (NodeUIEnvironment& decorated) :
	NodeUIEnvironment (),
	decorated (decorated)
{

}

NodeUIEnvironmentDecorator::~NodeUIEnvironmentDecorator ()
{

}

DrawingContext& NodeUIEnvironmentDecorator::GetDrawingContext ()
{
	return decorated.GetDrawingContext ();
}

SkinParams& NodeUIEnvironmentDecorator::GetSkinParams ()
{
	return decorated.GetSkinParams ();
}

EventHandlers& NodeUIEnvironmentDecorator::GetEventHandlers ()
{
	return decorated.GetEventHandlers ();
}

NE::EvaluationEnv& NodeUIEnvironmentDecorator::GetEvaluationEnv ()
{
	return decorated.GetEvaluationEnv ();
}

void NodeUIEnvironmentDecorator::RequestRedraw ()
{
	decorated.RequestRedraw ();
}

NodeUIEnvironmentContextDecorator::NodeUIEnvironmentContextDecorator (NodeUIEnvironment& decorated, DrawingContext& decoratedDrawingContext) :
	NodeUIEnvironmentDecorator (decorated),
	decoratedDrawingContext (decoratedDrawingContext)
{

}

NodeUIEnvironmentContextDecorator::~NodeUIEnvironmentContextDecorator ()
{

}

DrawingContext& NodeUIEnvironmentContextDecorator::GetDrawingContext ()
{
	return decoratedDrawingContext;
}

}
