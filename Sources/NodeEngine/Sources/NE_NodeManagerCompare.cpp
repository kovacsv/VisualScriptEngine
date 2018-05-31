#include "NE_NodeManagerCompare.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

static bool IsBinaryEqualNodes (const NodeConstPtr& a, const NodeConstPtr& b)
{
	MemoryOutputStream aStream;
	MemoryOutputStream bStream;

	a->Write (aStream);
	b->Write (bStream);

	return aStream.GetBuffer () == bStream.GetBuffer ();
}

NodeManagerDelta::NodeManagerDelta ()
{

}

void NodeManagerDelta::AddNewNode (const NodeId& nodeId)
{
	newNodes.push_back (nodeId);
}

void NodeManagerDelta::AddDeletedNode (const NodeId& nodeId)
{
	deletedNodes.push_back (nodeId);
}

void NodeManagerDelta::AddModifiedNode (const NodeId& nodeId)
{
	modifiedNodes.push_back (nodeId);
}

bool NodeManagerDelta::IsEmpty () const
{
	return newNodes.empty () && deletedNodes.empty () && modifiedNodes.empty ();
}

const std::vector<NodeId>& NodeManagerDelta::GetNewNodes () const
{
	return newNodes;
}

const std::vector<NodeId>& NodeManagerDelta::GetDeletedNodes () const
{
	return deletedNodes;
}

const std::vector<NodeId>& NodeManagerDelta::GetModifiedNodes () const
{
	return modifiedNodes;
}

NodeManagerDelta CompareNodeManagers (const NodeManager& source, const NodeManager& target)
{
	NodeManagerDelta delta;

	source.EnumerateNodes ([&] (const NodeConstPtr& sourceNode) {
		if (!target.ContainsNode (sourceNode->GetId ())) {
			delta.AddDeletedNode (sourceNode->GetId ());
		} else {
			NodeConstPtr targetNode = target.GetNode (sourceNode->GetId ());
			if (!IsBinaryEqualNodes (sourceNode, targetNode)) {
				delta.AddModifiedNode (sourceNode->GetId ());
			}
		}
		return true;
	});

	target.EnumerateNodes ([&] (const NodeConstPtr& targetNode) {
		if (!source.ContainsNode (targetNode->GetId ())) {
			delta.AddNewNode (targetNode->GetId ());
		}
		return true;
	});

	return delta;
}

}
