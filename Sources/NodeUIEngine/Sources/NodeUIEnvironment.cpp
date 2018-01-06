#include "NodeUIEnvironment.hpp"

namespace NUIE
{

NodeUIDrawingEnvironment::NodeUIDrawingEnvironment ()
{

}

NodeUIDrawingEnvironment::~NodeUIDrawingEnvironment ()
{

}

NodeUIEnvironment::NodeUIEnvironment () :
	NodeUIDrawingEnvironment ()
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

DrawingContext& NodeUIDrawingEnvironmentDecorator::GetDrawingContext ()
{
	return decorated.GetDrawingContext ();
}

SkinParams& NodeUIDrawingEnvironmentDecorator::GetSkinParams ()
{
	return decorated.GetSkinParams ();
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
