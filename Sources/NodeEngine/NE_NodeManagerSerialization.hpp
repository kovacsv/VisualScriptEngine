#ifndef NE_NODEMANAGERSERIALIZATION_HPP
#define NE_NODEMANAGERSERIALIZATION_HPP

#include "NE_NodeManager.hpp"

namespace NE
{

class NodeManagerSerialization
{
public:
	static Stream::Status	Read (NodeManager& nodeManager, InputStream& inputStream);
	static Stream::Status	Write (const NodeManager& nodeManager, OutputStream& outputStream);

private:
	static Stream::Status	ReadNodes (NodeManager& nodeManager, InputStream& inputStream, const ObjectVersion& version);
	static Stream::Status	ReadConnections (NodeManager& nodeManager, InputStream& inputStream, const ObjectVersion& version);
	static Stream::Status	ReadGroups (NodeManager& nodeManager, InputStream& inputStream, const ObjectVersion& version);
	static Stream::Status	WriteNodes (const NodeManager& nodeManager, OutputStream& outputStream);
	static Stream::Status	WriteConnections (const NodeManager& nodeManager, OutputStream& outputStream);
	static Stream::Status	WriteGroups (const NodeManager& nodeManager, OutputStream& outputStream);
};

}

#endif
