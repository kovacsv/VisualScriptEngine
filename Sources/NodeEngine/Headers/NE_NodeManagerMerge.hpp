#ifndef NODEMANAGERMERGRE_HPP
#define NODEMANAGERMERGRE_HPP

#include "NE_NodeManager.hpp"

namespace NE
{

bool AppendNodeManager (const NodeManager& source, NodeManager& target, const NodeFilter& nodeFilter);

}

#endif
