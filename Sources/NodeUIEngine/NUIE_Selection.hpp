#ifndef NUIE_SELECTION_HPP
#define NUIE_SELECTION_HPP

#include "NE_NodeCollection.hpp"

namespace NUIE
{

class Selection
{
public:
	enum class OperationResult
	{
		Changed,
		NotChanged
	};

	Selection ();
	~Selection ();

	bool						IsEmpty () const;
	OperationResult				Update (const Selection& newSelection);

	const NE::NodeCollection&	GetNodes () const;
	OperationResult				SetNodes (const NE::NodeCollection& newNodes);

	bool						ContainsNode (const NE::NodeId& nodeId) const;
	OperationResult				AddNode (const NE::NodeId& nodeId);
	OperationResult				RemoveNode (const NE::NodeId& nodeId);

	OperationResult				Clear ();

private:
	NE::NodeCollection			nodes;
};

extern const Selection EmptySelection;

}

#endif
