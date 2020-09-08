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

GroupInfo::GroupInfo () :
	name (),
	modelRect (),
	screenRect (),
	nodesInGroup ()
{

}

ConnectionInfo::ConnectionInfo () :
	fromNodeId (),
	fromSlotId (),
	toNodeId (),
	toSlotId ()
{

}

NodeEditorInfo::NodeEditorInfo () :
	nodes ()
{

}

}
