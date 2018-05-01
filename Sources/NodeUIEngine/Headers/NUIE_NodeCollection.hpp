#ifndef NODECOLLECTION_HPP
#define NODECOLLECTION_HPP

#include "NE_NodeId.hpp"

#include <vector>
#include <unordered_set>

namespace NUIE
{

class NodeCollection
{
	SERIALIZABLE;

public:
	NodeCollection ();
	NodeCollection (const NE::NodeId& nodeId);
	~NodeCollection ();

	bool				IsEmpty () const;
	bool				Contains (const NE::NodeId& nodeId) const;
	size_t				Count () const;
	NE::NodeId			GetLast () const;

	void				Enumerate (const std::function<bool (const NE::NodeId&)>& processor) const;
	void				Insert (const NE::NodeId& nodeId);
	void				Erase (const NE::NodeId& nodeId);
	void				Clear ();

	bool				operator== (const NodeCollection& rhs) const;
	bool				operator!= (const NodeCollection& rhs) const;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	std::vector<NE::NodeId>			nodes;
	std::unordered_set<NE::NodeId>	nodeSet;
};

}

#endif
