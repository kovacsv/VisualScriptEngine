#ifndef NODEGROUP_HPP
#define NODEGROUP_HPP

#include "NE_NodeEngineTypes.hpp"
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

	static NodeGroupPtr		Clone (const NodeGroupConstPtr& group);
	static bool				IsEqual (const NodeGroupConstPtr& aGroup, const NodeGroupConstPtr& bGroup);

protected:
	NodeCollection nodes;
};

}

#endif
