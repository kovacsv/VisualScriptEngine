#include "CopyPasteHandler.hpp"
#include "Debug.hpp"

namespace NUIE
{

CopyPasteHandler::CopyPasteHandler () :
	tempNodeManager ()
{
}

bool CopyPasteHandler::CanPaste () const
{
	return !tempNodeManager.IsEmpty ();
}

bool CopyPasteHandler::CopyFrom (const NE::NodeManager& source, NE::NodeFilter& nodeFilter)
{
	tempNodeManager.Clear ();
	return source.MergeTo (tempNodeManager, nodeFilter);
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target)
{
	NE::AllNodesFilter allNodesFilter;
	return tempNodeManager.MergeTo (target, allNodesFilter);
}

}
