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

	bool	CopyFrom (const NE::NodeManager& source, const NE::NodeCollection& nodesToCopy);
	bool	PasteTo (NE::NodeManager& target);

	bool	CanPaste () const;
	void	Clear ();

private:
	NE::NodeManager tempNodeManager;
};

}

#endif
