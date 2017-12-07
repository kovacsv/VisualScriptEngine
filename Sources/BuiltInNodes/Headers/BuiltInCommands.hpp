#ifndef BUILTINCOMMANDS_HPP
#define BUILTINCOMMANDS_HPP

#include "CommandStructure.hpp"
#include "NodeUIManager.hpp"
#include "NodeUIEnvironment.hpp"

namespace NUIE
{

class CreateNodeCommand : public SingleCommand
{
public:
	CreateNodeCommand (const std::wstring& name, NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, const Point& viewPosition);
	virtual ~CreateNodeCommand ();

	virtual void Do () override;

	virtual UINodePtr CreateNode (const Point& modelPosition) = 0;

private:
	NodeUIManager&		uiManager;
	NodeUIEnvironment&	uiEnvironment;
	Point				viewPosition;
};

}

#endif
