#ifndef NODEDRAWINGMODIFIER_HPP
#define NODEDRAWINGMODIFIER_HPP

#include "NE_NodeId.hpp"
#include "NUIE_Geometry.hpp"
#include <functional>

namespace NUIE
{

// TODO: typo
class NodeDrawingModfier
{
public:
	NodeDrawingModfier ();
	virtual ~NodeDrawingModfier ();

	virtual void	EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const = 0;
	virtual void	EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>& processor) const = 0;
	virtual Point	GetNodeOffset (const NE::NodeId& nodeId) const = 0;
};

}

#endif
