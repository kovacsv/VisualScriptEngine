#include "NUIE_CopyPasteHandler.hpp"
#include "NE_NodeManagerMerge.hpp"
#include "NE_Debug.hpp"

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

bool CopyPasteHandler::CopyFrom (const NE::NodeManager& source, const NE::NodeCollection& nodeCollection)
{
	tempNodeManager.Clear ();
	return NE::NodeManagerMerge::AppendNodeManager (source, tempNodeManager, nodeCollection);
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target)
{
	class AllNodesFilter : public NE::NodeFilter
	{
	public:
		virtual bool NeedToProcessNode (const NE::NodeId&) const override
		{
			return true;
		}
	};

	AllNodesFilter allNodesFilter;
	return NE::NodeManagerMerge::AppendNodeManager (tempNodeManager, target, allNodesFilter);
}

void CopyPasteHandler::Clear ()
{
	tempNodeManager.Clear ();
}

}
