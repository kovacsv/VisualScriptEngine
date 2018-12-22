#ifndef NUIE_NODEUIENVIRONMENT_HPP
#define NUIE_NODEUIENVIRONMENT_HPP

#include <memory>

namespace NE
{

class StringSettings;
class EvaluationEnv;

}

namespace NUIE
{

class SkinParams;
class DrawingContext;
class EventHandlers;

class NodeUIDrawingEnvironment
{
public:
	NodeUIDrawingEnvironment ();
	virtual ~NodeUIDrawingEnvironment ();

	virtual const NE::StringSettings&	GetStringSettings () = 0;
	virtual const SkinParams&			GetSkinParams () = 0;
	virtual DrawingContext&				GetDrawingContext () = 0;
};

class NodeUICalculationEnvironment
{
public:
	NodeUICalculationEnvironment ();
	virtual ~NodeUICalculationEnvironment ();

	virtual NE::EvaluationEnv&	GetEvaluationEnv () = 0;
	virtual void				OnEvaluationBegin () = 0;
	virtual void				OnEvaluationEnd () = 0;
	virtual void				OnValuesRecalculated () = 0;
	virtual void				OnRedrawRequested () = 0;
};

class NodeUIInteractionEnvironment
{
public:
	NodeUIInteractionEnvironment ();
	virtual ~NodeUIInteractionEnvironment ();

	virtual EventHandlers&		GetEventHandlers () = 0;
	virtual double				GetMouseMoveMinOffset () = 0;
};

class NodeUIEnvironment :	public NodeUIDrawingEnvironment,
							public NodeUICalculationEnvironment,
							public NodeUIInteractionEnvironment
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

	virtual const NE::StringSettings&	GetStringSettings () override;
	virtual const SkinParams&			GetSkinParams () override;
	virtual DrawingContext&				GetDrawingContext () override;

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
