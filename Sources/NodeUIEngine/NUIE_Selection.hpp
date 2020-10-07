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

	bool						ContainsNode (const NE::NodeId& nodeId) const;
	const NE::NodeCollection&	GetNodes () const;

	OperationResult				Update (const Selection& newSelection);

	OperationResult				SetNodes (const NE::NodeCollection& newNodes);
	OperationResult				AddNode (const NE::NodeId& nodeId);
	OperationResult				DeleteNode (const NE::NodeId& nodeId);

	OperationResult				Clear ();

private:
	NE::NodeCollection			nodes;
};

extern const Selection EmptySelection;

}

#endif
