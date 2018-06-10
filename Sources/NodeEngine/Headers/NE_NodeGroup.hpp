#ifndef NODEGROUP_HPP
#define NODEGROUP_HPP

#include "NE_Serializable.hpp"
#include "NE_NodeCollection.hpp"

#include <memory>
#include <unordered_map>

namespace NE
{

class NodeGroup : public DynamicSerializable
{
	SERIALIZABLE;

public:
	NodeGroup ();
	NodeGroup (const NodeCollection& nodes);
	~NodeGroup ();

	bool					IsEmpty () const;
	bool					ContainsNode (const NodeId& nodeId) const;

	bool					AddNode (const NodeId& nodeId);
	bool					DeleteNode (const NodeId& nodeId);
	const NodeCollection&	GetNodes () const;

	Stream::Status			Read (InputStream& inputStream);
	Stream::Status			Write (OutputStream& outputStream) const;

protected:
	NodeCollection nodes;
};

typedef std::shared_ptr<NodeGroup> NodeGroupPtr;
typedef std::shared_ptr<const NodeGroup> NodeGroupConstPtr;

}

#endif
