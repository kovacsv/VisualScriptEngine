#ifndef COPYPASTEHANDLER_HPP
#define COPYPASTEHANDLER_HPP

#include "NodeManager.hpp"
#include "NodeCollection.hpp"

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
