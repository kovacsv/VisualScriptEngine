#include "NUIE_EnvironmentDecorators.hpp"

namespace NUIE
{

DrawingEnvironmentDecorator::DrawingEnvironmentDecorator (NodeUIDrawingEnvironment& decorated) :
	NodeUIDrawingEnvironment (),
	decorated (decorated)
{

}

DrawingEnvironmentDecorator::~DrawingEnvironmentDecorator ()
{

}

const NE::StringConverter& DrawingEnvironmentDecorator::GetStringConverter ()
{
	return decorated.GetStringConverter ();
}

const SkinParams& DrawingEnvironmentDecorator::GetSkinParams ()
{
	return decorated.GetSkinParams ();
}

DrawingContext& DrawingEnvironmentDecorator::GetDrawingContext ()
{
	return decorated.GetDrawingContext ();
}

double DrawingEnvironmentDecorator::GetWindowScale ()
{
	return decorated.GetWindowScale ();
}

DrawingEnvironmentContextDecorator::DrawingEnvironmentContextDecorator (NodeUIDrawingEnvironment& decorated, DrawingContext& decoratedDrawingContext) :
	DrawingEnvironmentDecorator (decorated),
	decoratedDrawingContext (decoratedDrawingContext)
{

}

DrawingEnvironmentContextDecorator::~DrawingEnvironmentContextDecorator ()
{

}

DrawingContext& DrawingEnvironmentContextDecorator::GetDrawingContext ()
{
	return decoratedDrawingContext;
}

}
