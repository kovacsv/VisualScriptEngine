#ifndef NODEMANAGERCOMPARE_HPP
#define NODEMANAGERCOMPARE_HPP

#include "NE_NodeManager.hpp"

namespace NE
{

class NodeManagerDelta
{
public:
	NodeManagerDelta ();

	void						AddNewNode (const NodeId& nodeId);
	void						AddDeletedNode (const NodeId& nodeId);
	void						AddModifiedNode (const NodeId& nodeId);

	bool						IsEmpty () const;

	const std::vector<NodeId>&	GetNewNodes () const;
	const std::vector<NodeId>&	GetDeletedNodes () const;
	const std::vector<NodeId>&	GetModifiedNodes () const;

private:
	std::vector<NodeId>		newNodes;
	std::vector<NodeId>		deletedNodes;
	std::vector<NodeId>		modifiedNodes;
};

NodeManagerDelta CompareNodeManagers (const NodeManager& source, const NodeManager& target);

}

#endif
