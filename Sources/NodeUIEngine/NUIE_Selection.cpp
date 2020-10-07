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

Selection::OperationResult Selection::Update (const Selection& newSelection)
{
	OperationResult result = OperationResult::NotChanged;
	if (nodes != newSelection.nodes) {
		nodes = newSelection.nodes;
		result = OperationResult::Changed;
	}
	return result;
}

Selection::OperationResult Selection::SetNodes (const NE::NodeCollection& newNodes)
{
	OperationResult result = OperationResult::NotChanged;
	if (nodes != newNodes) {
		nodes = newNodes;
		result = OperationResult::Changed;
	}
	return result;
}

Selection::OperationResult Selection::AddNode (const NE::NodeId& nodeId)
{
	OperationResult result = OperationResult::NotChanged;
	if (!nodes.Contains (nodeId)) {
		nodes.Insert (nodeId);
		result = OperationResult::Changed;
	}
	return result;
}

Selection::OperationResult Selection::DeleteNode (const NE::NodeId& nodeId)
{
	OperationResult result = OperationResult::NotChanged;
	if (nodes.Contains (nodeId)) {
		nodes.Erase (nodeId);
		result = OperationResult::Changed;
	}
	return result;
}

Selection::OperationResult Selection::Clear ()
{
	OperationResult result = OperationResult::NotChanged;
	if (!nodes.IsEmpty ()) {
		nodes.Clear ();
		result = OperationResult::Changed;
	}
	return result;
}

const Selection EmptySelection;

}
