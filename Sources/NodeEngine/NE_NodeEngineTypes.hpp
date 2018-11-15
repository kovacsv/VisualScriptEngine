#ifndef NE_TYPES_HPP
#define NE_TYPES_HPP

#include <memory>

namespace NE
{

class EvaluationEnv;

class NodeEvaluator;
typedef std::shared_ptr<NodeEvaluator> NodeEvaluatorPtr;
typedef std::shared_ptr<const NodeEvaluator> NodeEvaluatorConstPtr;

class Node;
typedef std::shared_ptr<Node> NodePtr;
typedef std::shared_ptr<const Node> NodeConstPtr;

class InputSlot;
typedef std::shared_ptr<InputSlot> InputSlotPtr;
typedef std::shared_ptr<const InputSlot> InputSlotConstPtr;

class OutputSlot;
typedef std::shared_ptr<OutputSlot> OutputSlotPtr;
typedef std::shared_ptr<const OutputSlot> OutputSlotConstPtr;

class NodeGroup;
typedef std::shared_ptr<NodeGroup> NodeGroupPtr;
typedef std::shared_ptr<const NodeGroup> NodeGroupConstPtr;

}

#endif