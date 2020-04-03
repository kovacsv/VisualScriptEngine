#include "NUIE_NodeUIEnvironment.hpp"

namespace NUIE
{

NodeUIDrawingEnvironment::NodeUIDrawingEnvironment ()
{

}

NodeUIDrawingEnvironment::~NodeUIDrawingEnvironment ()
{

}

NodeUICalculationEnvironment::NodeUICalculationEnvironment ()
{

}

NodeUICalculationEnvironment::~NodeUICalculationEnvironment ()
{

}

NodeUIInteractionEnvironment::NodeUIInteractionEnvironment ()
{

}

NodeUIInteractionEnvironment::~NodeUIInteractionEnvironment ()
{

}

NodeUIEnvironment::NodeUIEnvironment ()
{

}

NodeUIEnvironment::~NodeUIEnvironment ()
{

}

NodeUIDrawingEnvironmentDecorator::NodeUIDrawingEnvironmentDecorator (NodeUIDrawingEnvironment& decorated) :
	NodeUIDrawingEnvironment (),
	decorated (decorated)
{

}

NodeUIDrawingEnvironmentDecorator::~NodeUIDrawingEnvironmentDecorator ()
{

}

const NE::StringSettings& NodeUIDrawingEnvironmentDecorator::GetStringSettings ()
{
	return decorated.GetStringSettings ();
}

const SkinParams& NodeUIDrawingEnvironmentDecorator::GetSkinParams ()
{
	return decorated.GetSkinParams ();
}

DrawingContext& NodeUIDrawingEnvironmentDecorator::GetDrawingContext ()
{
	return decorated.GetDrawingContext ();
}

double NodeUIDrawingEnvironmentDecorator::GetWindowScale ()
{
	return decorated.GetWindowScale ();
}

NodeUIDrawingEnvironmentContextDecorator::NodeUIDrawingEnvironmentContextDecorator (NodeUIDrawingEnvironment& decorated, DrawingContext& decoratedDrawingContext) :
	NodeUIDrawingEnvironmentDecorator (decorated),
	decoratedDrawingContext (decoratedDrawingContext)
{

}

NodeUIDrawingEnvironmentContextDecorator::~NodeUIDrawingEnvironmentContextDecorator ()
{

}

DrawingContext& NodeUIDrawingEnvironmentContextDecorator::GetDrawingContext ()
{
	return decoratedDrawingContext;
}

}
