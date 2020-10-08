#ifndef NUIE_SELECTION_HPP
#define NUIE_SELECTION_HPP

#include "NE_NodeCollection.hpp"

namespace NUIE
{

class Selection
{
public:
	enum class ChangeResult
	{
		Changed,
		NotChanged
	};

	Selection ();
	~Selection ();

	bool						IsEmpty () const;

	bool						ContainsNode (const NE::NodeId& nodeId) const;
	const NE::NodeCollection&	GetNodes () const;

	ChangeResult				Update (const Selection& newSelection);

	ChangeResult				SetNodes (const NE::NodeCollection& newNodes);
	ChangeResult				AddNode (const NE::NodeId& nodeId);
	ChangeResult				DeleteNode (const NE::NodeId& nodeId);

	ChangeResult				Clear ();

private:
	NE::NodeCollection			nodes;
};

extern const Selection EmptySelection;

}

#endif
