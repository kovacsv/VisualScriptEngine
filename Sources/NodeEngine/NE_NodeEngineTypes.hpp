#ifndef NE_TYPES_HPP
#define NE_TYPES_HPP

#include <memory>

namespace NE
{

class EvaluationEnv;

class Node;
using NodePtr = std::shared_ptr<Node>;
using NodeConstPtr = std::shared_ptr<const Node>;

class InputSlot;
using InputSlotPtr = std::shared_ptr<InputSlot>;
using InputSlotConstPtr = std::shared_ptr<const InputSlot>;

class OutputSlot;
using OutputSlotPtr = std::shared_ptr<OutputSlot>;
using OutputSlotConstPtr = std::shared_ptr<const OutputSlot>;

class NodeGroup;
using NodeGroupPtr = std::shared_ptr<NodeGroup>;
using NodeGroupConstPtr = std::shared_ptr<const NodeGroup>;

}

#endif
