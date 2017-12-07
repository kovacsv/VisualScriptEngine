#include "NodeUIEnvironment.hpp"

namespace NUIE
{

NodeUIEnvironment::NodeUIEnvironment (NodeDrawingContext& drawingContext, SkinParams& skinParams, EventHandlers& eventHandlers, NE::EvaluationEnv& evaluationEnv) :
	drawingContext (drawingContext),
	skinParams (skinParams),
	eventHandlers (eventHandlers),
	evaluationEnv (evaluationEnv)
{

}

NodeUIEnvironment::~NodeUIEnvironment ()
{

}

NodeDrawingContext& NodeUIEnvironment::GetDrawingContext ()
{
	return drawingContext;
}

SkinParams& NodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

EventHandlers& NodeUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

NE::EvaluationEnv& NodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

NodeUIEnvironmentDecorator::NodeUIEnvironmentDecorator (NodeUIEnvironment& decorated) :
	NodeUIEnvironment (decorated.GetDrawingContext (), decorated.GetSkinParams (), decorated.GetEventHandlers (), decorated.GetEvaluationEnv ())
{

}

NodeUIEnvironmentDecorator::~NodeUIEnvironmentDecorator ()
{

}

NodeUIEnvironmentContextDecorator::NodeUIEnvironmentContextDecorator (NodeUIEnvironment& decorated, NodeDrawingContext& decoratedDrawingContext) :
	NodeUIEnvironmentDecorator (decorated),
	decoratedDrawingContext (decoratedDrawingContext)
{

}

NodeUIEnvironmentContextDecorator::~NodeUIEnvironmentContextDecorator ()
{

}

NodeDrawingContext& NodeUIEnvironmentContextDecorator::GetDrawingContext ()
{
	return decoratedDrawingContext;
}

}
