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

bool CopyPasteHandler::CopyFrom (const NE::NodeManager& source, const NodeCollection& nodeCollection)
{
	class CopyFilter : public NE::NodeFilter
	{
	public:
		CopyFilter (const NodeCollection& nodeCollection) :
			nodeCollection (nodeCollection)
		{
			
		}

		virtual bool NeedToProcessNode (const NE::NodeId& nodeId) const override
		{
			return nodeCollection.Contains (nodeId);
		}

	private:
		const NodeCollection& nodeCollection;
	};

	tempNodeManager.Clear ();
	CopyFilter copyFilter (nodeCollection);
	return tempNodeManager.Append (source, copyFilter);
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target)
{
	// TODO: Node created does not called after append
	NE::AllNodesFilter allNodesFilter;
	return target.Append (tempNodeManager, allNodesFilter);
}

}
