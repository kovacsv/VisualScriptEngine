#ifndef NODEDRAWINGMODIFIER_HPP
#define NODEDRAWINGMODIFIER_HPP

#include "NUIE_Geometry.hpp"
#include <functional>

namespace NUIE
{

class NodeDrawingModfier
{
public:
	virtual void EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const = 0;
	virtual void EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>& processor) const = 0;
};

}

#endif
