#ifndef BI_BUILTINCOMMANDS_HPP
#define BI_BUILTINCOMMANDS_HPP

#include "NUIE_MenuCommands.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeUIEnvironment.hpp"

namespace BI
{

class CreateNodeCommand : public NUIE::SingleMenuCommand
{
public:
	CreateNodeCommand (const std::wstring& name, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& viewPosition);
	virtual ~CreateNodeCommand ();

	virtual void Do () override;

	virtual NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition) = 0;

private:
	NUIE::NodeUIManager&		uiManager;
	NUIE::NodeUIEnvironment&	uiEnvironment;
	NUIE::Point					viewPosition;
};

}

#endif
