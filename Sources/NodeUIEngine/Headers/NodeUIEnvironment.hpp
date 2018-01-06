#ifndef NODEUIENVIRONMENT_HPP
#define NODEUIENVIRONMENT_HPP

#include <memory>

namespace NE
{

class EvaluationEnv;

}

namespace NUIE
{

class DrawingContext;
class SkinParams;
class EventHandlers;

class NodeUIDrawingEnvironment
{
public:
	NodeUIDrawingEnvironment ();
	virtual ~NodeUIDrawingEnvironment ();

	virtual DrawingContext&		GetDrawingContext () = 0;
	virtual SkinParams&			GetSkinParams () = 0;
};

class NodeUICalculationEnvironment
{
public:
	NodeUICalculationEnvironment ();
	virtual ~NodeUICalculationEnvironment ();

	virtual EventHandlers&		GetEventHandlers () = 0;
	virtual NE::EvaluationEnv&	GetEvaluationEnv () = 0;
	virtual void				RequestRedraw () = 0;
};

class NodeUIEnvironment :	public NodeUIDrawingEnvironment,
							public NodeUICalculationEnvironment
{
public:
	NodeUIEnvironment ();
	virtual ~NodeUIEnvironment ();
};

class NodeUIDrawingEnvironmentDecorator : public NodeUIDrawingEnvironment
{
public:
	NodeUIDrawingEnvironmentDecorator (NodeUIDrawingEnvironment& decorated);
	virtual ~NodeUIDrawingEnvironmentDecorator ();

	virtual DrawingContext&		GetDrawingContext () override;
	virtual SkinParams&			GetSkinParams () override;

private:
	NodeUIDrawingEnvironment& decorated;
};

class NodeUIDrawingEnvironmentContextDecorator : public NodeUIDrawingEnvironmentDecorator
{
public:
	NodeUIDrawingEnvironmentContextDecorator (NodeUIDrawingEnvironment& decorated, DrawingContext& decoratedDrawingContext);
	virtual ~NodeUIDrawingEnvironmentContextDecorator ();

	virtual DrawingContext&		GetDrawingContext () override;

private:
	DrawingContext& decoratedDrawingContext;
};

}

#endif
