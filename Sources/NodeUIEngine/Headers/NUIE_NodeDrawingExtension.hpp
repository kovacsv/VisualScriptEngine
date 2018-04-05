#ifndef NODEDRAWINGEXTENSION_HPP
#define NODEDRAWINGEXTENSION_HPP

#include "NUIE_Geometry.hpp"
#include <functional>

namespace NUIE
{

class NodeDrawingExtension
{
public:
	virtual void EnumerateSelectionRectangles (const std::function<void (const Rect&)>& processor) const = 0;
	virtual void EnumerateTemporaryConnections (const std::function<void (const Point&, const Point&)>& processor) const = 0;
};

}

#endif
