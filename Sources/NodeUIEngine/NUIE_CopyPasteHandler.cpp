#include "NUIE_CopyPasteHandler.hpp"
#include "NE_NodeManagerMerge.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

CopyPasteHandler::CopyPasteHandler () :
	tempNodeManager ()
{
}

bool CopyPasteHandler::CopyFrom (const NE::NodeManager& source, const NE::NodeCollection& nodesToCopy)
{
	tempNodeManager.Clear ();
	NE::EmptyAppendEventHandler eventHandler;
	NE::NodeCollectionFilter nodeFilter (nodesToCopy);
	return NE::NodeManagerMerge::AppendNodeManager (source, tempNodeManager, nodeFilter, eventHandler);
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target)
{
	NE::AllNodesFilter allNodesFilter;
	NE::EmptyAppendEventHandler eventHandler;
	return NE::NodeManagerMerge::AppendNodeManager (tempNodeManager, target, allNodesFilter, eventHandler);
}

bool CopyPasteHandler::CanPaste () const
{
	return !tempNodeManager.IsEmpty ();
}

void CopyPasteHandler::Clear ()
{
	tempNodeManager.Clear ();
}

}
