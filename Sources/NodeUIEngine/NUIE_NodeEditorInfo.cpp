#include "NUIE_NodeEditorInfo.hpp"

namespace NUIE
{

SlotInfo::SlotInfo () :
	id (),
	name (),
	modelRect (),
	screenRect ()
{

}

NodeInfo::NodeInfo () :
	id (),
	name (),
	modelRect (),
	screenRect (),
	inputSlots (),
	outputSlots ()
{

}

NodeEditorInfo::NodeEditorInfo () :
	nodes ()
{

}

}
