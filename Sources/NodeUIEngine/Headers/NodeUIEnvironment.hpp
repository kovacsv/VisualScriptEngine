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

class NodeUIEnvironment
{
public:
	NodeUIEnvironment (DrawingContext& drawingContext, SkinParams& skinParams, EventHandlers& eventHandlers, NE::EvaluationEnv& evaluationEnv);
	virtual ~NodeUIEnvironment ();

	virtual DrawingContext&		GetDrawingContext ();
	virtual SkinParams&			GetSkinParams ();
	virtual EventHandlers&		GetEventHandlers ();
	virtual NE::EvaluationEnv&	GetEvaluationEnv ();

private:
	DrawingContext&		drawingContext;
	SkinParams&			skinParams;
	EventHandlers&		eventHandlers;
	NE::EvaluationEnv&	evaluationEnv;
};

class NodeUIEnvironmentDecorator : public NodeUIEnvironment
{
public:
	NodeUIEnvironmentDecorator (NodeUIEnvironment& decorated);
	virtual ~NodeUIEnvironmentDecorator ();
};

class NodeUIEnvironmentContextDecorator : public NodeUIEnvironmentDecorator
{
public:
	NodeUIEnvironmentContextDecorator (NodeUIEnvironment& decorated, DrawingContext& decoratedDrawingContext);
	virtual ~NodeUIEnvironmentContextDecorator ();

	virtual DrawingContext& GetDrawingContext () override;

private:
	DrawingContext& decoratedDrawingContext;
};

typedef std::shared_ptr<NodeUIEnvironment> NodeUIEnvironmentPtr;

}

#endif
