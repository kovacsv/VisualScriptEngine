#ifndef NUIE_NODEEDITORINFO_HPP
#define NUIE_NODEEDITORINFO_HPP

#include "NE_SlotId.hpp"
#include "NE_NodeId.hpp"
#include "NUIE_Geometry.hpp"

#include <vector>
#include <string>

namespace NUIE
{

struct CanvasInfo
{
	CanvasInfo ();

	int width;
	int height;
};

struct SlotInfo
{
	SlotInfo ();

	NE::SlotId		id;
	std::wstring	name;
	Rect			modelRect;
	Rect			screenRect;
};

struct NodeInfo
{
	NodeInfo ();

	NE::NodeId				id;
	std::wstring			name;
	
	Rect					modelRect;
	Rect					screenRect;

	std::vector<SlotInfo>	inputSlots;
	std::vector<SlotInfo>	outputSlots;
};

struct GroupInfo
{
	GroupInfo ();

	std::wstring				name;

	Rect						modelRect;
	Rect						screenRect;

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

	CanvasInfo						canvas;
	std::vector<NodeInfo>			nodes;
	std::vector<GroupInfo>			groups;
	std::vector<ConnectionInfo>		connections;
};

}

#endif
