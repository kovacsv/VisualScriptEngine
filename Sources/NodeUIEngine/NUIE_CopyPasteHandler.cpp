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
	return NE::NodeManagerMerge::AppendNodeManager (source, tempNodeManager, nodesToCopy, eventHandler);
}

bool CopyPasteHandler::CanPaste () const
{
	return !tempNodeManager.IsEmpty ();
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target)
{
	NE::AllNodesFilter allNodesFilter;
	NE::EmptyAppendEventHandler eventHandler;
	return NE::NodeManagerMerge::AppendNodeManager (tempNodeManager, target, allNodesFilter, eventHandler);
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target, NE::NodeCollection& pastedNodes)
{
	class PasteEventHandler : public NE::AppendEventHandler
	{
	public:
		PasteEventHandler (NE::NodeCollection& pastedNodes) :
			pastedNodes (pastedNodes)
		{

		}

		virtual void TargetNodeAdded (const NE::NodeId& nodeId) override
		{
			pastedNodes.Insert (nodeId);
		}

	private:
		NE::NodeCollection& pastedNodes;
	};

	NE::AllNodesFilter allNodesFilter;
	PasteEventHandler eventHandler (pastedNodes);
	return NE::NodeManagerMerge::AppendNodeManager (tempNodeManager, target, allNodesFilter, eventHandler);
}

void CopyPasteHandler::Clear ()
{
	tempNodeManager.Clear ();
}

}
