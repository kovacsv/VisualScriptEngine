#ifndef NODEUIENVIRONMENT_HPP
#define NODEUIENVIRONMENT_HPP

#include <memory>

namespace NE
{

class EvaluationEnv;

}

namespace NUIE
{

class NodeDrawingContext;
class SkinParams;
class EventHandlers;

class NodeUIEnvironment
{
public:
	NodeUIEnvironment (NodeDrawingContext& drawingContext, SkinParams& skinParams, EventHandlers& eventHandlers, NE::EvaluationEnv& evaluationEnv);
	virtual ~NodeUIEnvironment ();

	virtual NodeDrawingContext&		GetDrawingContext ();
	virtual SkinParams&				GetSkinParams ();
	virtual EventHandlers&			GetEventHandlers ();
	virtual NE::EvaluationEnv&		GetEvaluationEnv ();

private:
	NodeDrawingContext&		drawingContext;
	SkinParams&				skinParams;
	EventHandlers&			eventHandlers;
	NE::EvaluationEnv&		evaluationEnv;
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
	NodeUIEnvironmentContextDecorator (NodeUIEnvironment& decorated, NodeDrawingContext& decoratedDrawingContext);
	virtual ~NodeUIEnvironmentContextDecorator ();

	virtual NodeDrawingContext& GetDrawingContext () override;

private:
	NodeDrawingContext& decoratedDrawingContext;
};

typedef std::shared_ptr<NodeUIEnvironment> NodeUIEnvironmentPtr;

}

#endif
