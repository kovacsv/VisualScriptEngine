#ifndef NUIE_NODEEDITORINFO_HPP
#define NUIE_NODEEDITORINFO_HPP

#include "NE_SlotId.hpp"
#include "NE_NodeId.hpp"
#include "NE_NodeGroupId.hpp"
#include "NUIE_Geometry.hpp"

#include <vector>
#include <string>

namespace NUIE
{

struct ViewInfo
{
	ViewInfo ();

	int		width;
	int		height;
};

struct SlotInfo
{
	SlotInfo ();

	NE::SlotId		id;
	std::wstring	name;
	Rect			modelRect;
	Rect			viewRect;
};

struct NodeInfo
{
	NodeInfo ();

	NE::NodeId				id;
	std::wstring			name;
	
	Rect					modelRect;
	Rect					viewRect;

	std::vector<SlotInfo>	inputSlots;
	std::vector<SlotInfo>	outputSlots;
};

struct GroupInfo
{
	GroupInfo ();

	NE::NodeGroupId				id;

	std::wstring				name;

	Rect						modelRect;
	Rect						viewRect;

	std::vector<NE::NodeId>		nodesInGroup;
};

struct ConnectionInfo
{
	ConnectionInfo ();

	NE::NodeId				fromNodeId;
	NE::SlotId				fromSlotId;
	NE::NodeId				toNodeId;
	NE::SlotId				toSlotId;
};

struct NodeEditorInfo
{
	NodeEditorInfo ();

	ViewInfo						view;
	std::vector<NodeInfo>			nodes;
	std::vector<GroupInfo>			groups;
	std::vector<ConnectionInfo>		connections;
};

}

#endif
