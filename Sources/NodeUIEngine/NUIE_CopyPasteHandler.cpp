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
	NE::EmptyAppendEventHandler eventHandler;
	return NE::NodeManagerMerge::AppendNodeManager (source, tempNodeManager, nodeCollection, eventHandler);
}

bool CopyPasteHandler::PasteTo (NE::NodeManager& target, NE::NodeCollection& nodeCollection)
{
	class PasteEventHandler : public NE::AppendEventHandler
	{
	public:
		PasteEventHandler (NE::NodeCollection& nodeCollection) :
			nodeCollection (nodeCollection)
		{

		}

		virtual void NodeAdded (const NE::NodeId& nodeId) override
		{
			nodeCollection.Insert (nodeId);
		}

	private:
		NE::NodeCollection& nodeCollection;
	};

	NE::AllNodesFilter allNodesFilter;
	PasteEventHandler eventHandler (nodeCollection);
	return NE::NodeManagerMerge::AppendNodeManager (tempNodeManager, target, allNodesFilter, eventHandler);
}

void CopyPasteHandler::Clear ()
{
	tempNodeManager.Clear ();
}

}
