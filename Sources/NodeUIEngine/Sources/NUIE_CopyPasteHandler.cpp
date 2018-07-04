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
	class CopyFilter : public NE::NodeFilter
	{
	public:
		CopyFilter (const NE::NodeCollection& nodeCollection) :
			nodeCollection (nodeCollection)
		{
			
		}

		virtual bool NeedToProcessNode (const NE::NodeId& nodeId) const override
		{
			return nodeCollection.Contains (nodeId);
		}

	private:
		const NE::NodeCollection& nodeCollection;
	};

	tempNodeManager.Clear ();
	CopyFilter copyFilter (nodeCollection);
	return NE::NodeManagerMerge::AppendNodeManager (source, tempNodeManager, copyFilter);
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

}
