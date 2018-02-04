#ifndef COPYPASTEHANDLER_HPP
#define COPYPASTEHANDLER_HPP

#include "NodeManager.hpp"

namespace NUIE
{

class CopyPasteHandler
{
public:
	CopyPasteHandler ();

	bool	CanPaste () const;
	bool	CopyFrom (const NE::NodeManager& source, NE::NodeFilter& nodeFilter);
	bool	PasteTo (NE::NodeManager& target);

private:
	NE::NodeManager tempNodeManager;
};

}

#endif
