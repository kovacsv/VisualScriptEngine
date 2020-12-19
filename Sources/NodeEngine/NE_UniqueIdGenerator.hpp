#ifndef NE_UNIQUEIDGENERATOR_HPP
#define NE_UNIQUEIDGENERATOR_HPP

#include "NE_Serializable.hpp"
#include "NE_NodeId.hpp"
#include "NE_NodeGroupId.hpp"

#include <atomic>

namespace NE
{

using IdType = size_t;

class UniqueIdGenerator
{
	SERIALIZABLE;

public:
	UniqueIdGenerator ();
	~UniqueIdGenerator ();

	void					Clear ();
	NodeId					GenerateNodeId ();
	NodeGroupId				GenerateNodeGroupId ();

	Stream::Status			Read (InputStream& inputStream);
	Stream::Status			Write (OutputStream& outputStream) const;

private:
	std::atomic<IdType>		nextNodeId;
	std::atomic<IdType>		nextNodeGroupId;
};

}

#endif
