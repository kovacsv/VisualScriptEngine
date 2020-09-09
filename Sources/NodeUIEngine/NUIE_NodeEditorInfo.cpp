#include "NUIE_NodeEditorInfo.hpp"

namespace NUIE
{

ViewInfo::ViewInfo () :
	width (0),
	height (0)
{

}

SlotInfo::SlotInfo () :
	id (),
	name (),
	modelRect (),
	viewRect ()
{

}

NodeInfo::NodeInfo () :
	id (),
	name (),
	modelRect (),
	viewRect (),
	inputSlots (),
	outputSlots ()
{

}

GroupInfo::GroupInfo () :
	name (),
	modelRect (),
	viewRect (),
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
	view (),
	nodes (),
	groups (),
	connections ()
{

}

}
