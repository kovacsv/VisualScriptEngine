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
	NodeUIEnvironment ();
	virtual ~NodeUIEnvironment ();

	virtual DrawingContext&		GetDrawingContext () = 0;
	virtual SkinParams&			GetSkinParams () = 0;
	virtual EventHandlers&		GetEventHandlers () = 0;
	virtual NE::EvaluationEnv&	GetEvaluationEnv () = 0;
	virtual void				RequestRedraw () = 0;
};

class NodeUIEnvironmentDecorator : public NodeUIEnvironment
{
public:
	NodeUIEnvironmentDecorator (NodeUIEnvironment& decorated);
	virtual ~NodeUIEnvironmentDecorator ();

	virtual DrawingContext&		GetDrawingContext () override;
	virtual SkinParams&			GetSkinParams () override;
	virtual EventHandlers&		GetEventHandlers () override;
	virtual NE::EvaluationEnv&	GetEvaluationEnv () override;
	virtual void				RequestRedraw () override;

private:
	NodeUIEnvironment& decorated;
};

class NodeUIEnvironmentContextDecorator : public NodeUIEnvironmentDecorator
{
public:
	NodeUIEnvironmentContextDecorator (NodeUIEnvironment& decorated, DrawingContext& decoratedDrawingContext);
	virtual ~NodeUIEnvironmentContextDecorator ();

	virtual DrawingContext&		GetDrawingContext () override;

private:
	DrawingContext& decoratedDrawingContext;
};

typedef std::shared_ptr<NodeUIEnvironment> NodeUIEnvironmentPtr;

}

#endif
