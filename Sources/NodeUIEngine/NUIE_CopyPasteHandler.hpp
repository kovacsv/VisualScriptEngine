#ifndef NUIE_COPYPASTEHANDLER_HPP
#define NUIE_COPYPASTEHANDLER_HPP

#include "NE_NodeManager.hpp"
#include "NE_NodeCollection.hpp"

namespace NUIE
{

class CopyPasteHandler
{
public:
	CopyPasteHandler ();

	bool	CanPaste () const;
	bool	CopyFrom (const NE::NodeManager& source, const NE::NodeCollection& nodeCollection);
	bool	PasteTo (NE::NodeManager& target, NE::NodeCollection& pastedNodes);
	void	Clear ();

private:
	NE::NodeManager tempNodeManager;
};

}

#endif
