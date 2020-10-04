#ifndef NE_UNIQUEIDGENERATOR_HPP
#define NE_UNIQUEIDGENERATOR_HPP

#include "NE_Serializable.hpp"
#include "NE_NodeId.hpp"

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

	NodeId					GenerateNodeId ();

	Stream::Status			Read (InputStream& inputStream);
	Stream::Status			Write (OutputStream& outputStream) const;

private:
	std::atomic<IdType>		nextId;
};

}

#endif
