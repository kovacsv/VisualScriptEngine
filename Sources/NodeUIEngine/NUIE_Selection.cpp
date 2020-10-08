#include "NUIE_Selection.hpp"

namespace NUIE
{

Selection::Selection () :
	nodes ()
{

}

Selection::~Selection ()
{

}

bool Selection::IsEmpty () const
{
	return nodes.IsEmpty ();
}

bool Selection::ContainsNode (const NE::NodeId& nodeId) const
{
	return nodes.Contains (nodeId);
}

const NE::NodeCollection& Selection::GetNodes () const
{
	return nodes;
}

Selection::ChangeResult Selection::Update (const Selection& newSelection)
{
	ChangeResult result = ChangeResult::NotChanged;
	if (nodes != newSelection.nodes) {
		nodes = newSelection.nodes;
		result = ChangeResult::Changed;
	}
	return result;
}

Selection::ChangeResult Selection::SetNodes (const NE::NodeCollection& newNodes)
{
	ChangeResult result = ChangeResult::NotChanged;
	if (nodes != newNodes) {
		nodes = newNodes;
		result = ChangeResult::Changed;
	}
	return result;
}

Selection::ChangeResult Selection::AddNode (const NE::NodeId& nodeId)
{
	ChangeResult result = ChangeResult::NotChanged;
	if (!nodes.Contains (nodeId)) {
		nodes.Insert (nodeId);
		result = ChangeResult::Changed;
	}
	return result;
}

Selection::ChangeResult Selection::DeleteNode (const NE::NodeId& nodeId)
{
	ChangeResult result = ChangeResult::NotChanged;
	if (nodes.Contains (nodeId)) {
		nodes.Erase (nodeId);
		result = ChangeResult::Changed;
	}
	return result;
}

Selection::ChangeResult Selection::Clear ()
{
	ChangeResult result = ChangeResult::NotChanged;
	if (!nodes.IsEmpty ()) {
		nodes.Clear ();
		result = ChangeResult::Changed;
	}
	return result;
}

const Selection EmptySelection;

}
