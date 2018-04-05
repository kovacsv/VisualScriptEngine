#ifndef COPYPASTEHANDLER_HPP
#define COPYPASTEHANDLER_HPP

#include "NE_NodeManager.hpp"
#include "NUIE_NodeCollection.hpp"

namespace NUIE
{

class CopyPasteHandler
{
public:
	CopyPasteHandler ();

	bool	CanPaste () const;
	bool	CopyFrom (const NE::NodeManager& source, const NodeCollection& nodeCollection);
	bool	PasteTo (NE::NodeManager& target);

private:
	NE::NodeManager tempNodeManager;
};

}

#endif
