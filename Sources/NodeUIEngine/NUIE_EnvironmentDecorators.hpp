#ifndef NUIE_ENVIRONMENTDECORATORS_HPP
#define NUIE_ENVIRONMENTDECORATORS_HPP

#include "NUIE_NodeUIEnvironment.hpp"

namespace NUIE
{

class DrawingEnvironmentDecorator : public NodeUIDrawingEnvironment
{
public:
	DrawingEnvironmentDecorator (NodeUIDrawingEnvironment& decorated);
	virtual ~DrawingEnvironmentDecorator ();

	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const SkinParams&			GetSkinParams () override;
	virtual DrawingContext&				GetDrawingContext () override;
	virtual double						GetWindowScale () override;

private:
	NodeUIDrawingEnvironment& decorated;
};

class DrawingEnvironmentContextDecorator : public DrawingEnvironmentDecorator
{
public:
	DrawingEnvironmentContextDecorator (NodeUIDrawingEnvironment& decorated, DrawingContext& decoratedDrawingContext);
	virtual ~DrawingEnvironmentContextDecorator ();

	virtual DrawingContext&		GetDrawingContext () override;

private:
	DrawingContext& decoratedDrawingContext;
};

}

#endif
