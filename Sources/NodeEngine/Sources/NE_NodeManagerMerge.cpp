#include "NE_NodeManagerMerge.hpp"
#include "NE_MemoryStream.hpp"

namespace NE
{

bool AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter)
{
	MemoryOutputStream outputStream;
	if (DBGERROR (source.WriteNodes (outputStream, nodeFilter) != Stream::Status::NoError)) {
		return false;
	}
	MemoryInputStream inputStream (outputStream.GetBuffer ());
	if (DBGERROR (target.ReadNodes (inputStream, NodeManager::IdHandlingPolicy::GenerateNewId) != Stream::Status::NoError)) {
		return false;
	}
	return true;
}

}
