#include "NUIE_NodeDrawingImage.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

NodeDrawingImage::NodeDrawingImage ()
{

}

NodeDrawingImage::~NodeDrawingImage ()
{

}

void NodeDrawingImage::Reset ()
{
	Clear ();
	nodeRect = Rect (0.0, 0.0, 0.0, 0.0);
	inputSlotConnPositions.clear ();
	outputSlotConnPositions.clear ();
	inputSlotRects.clear ();
	outputSlotRects.clear ();
	specialRects.clear ();
}

void NodeDrawingImage::SetNodeRect (const Rect& rect)
{
	nodeRect = rect;
}

const Rect& NodeDrawingImage::GetNodeRect () const
{
	return nodeRect;
}

void NodeDrawingImage::AddInputSlotConnPosition (const NE::SlotId& slotId, const Point& position)
{
	DBGASSERT (inputSlotConnPositions.find (slotId) == inputSlotConnPositions.end ());
	inputSlotConnPositions.insert ({ slotId, position });
}

const Point& NodeDrawingImage::GetInputSlotConnPosition (const NE::SlotId& slotId) const
{
	return inputSlotConnPositions.find (slotId)->second;
}

void NodeDrawingImage::AddOutputSlotConnPosition (const NE::SlotId& slotId, const Point& position)
{
	DBGASSERT (outputSlotConnPositions.find (slotId) == outputSlotConnPositions.end ());
	outputSlotConnPositions.insert ({ slotId, position });
}

const Point& NodeDrawingImage::GetOutputSlotConnPosition (const NE::SlotId& slotId) const
{
	return outputSlotConnPositions.find (slotId)->second;
}

bool NodeDrawingImage::HasInputSlotRect (const NE::SlotId& slotId) const
{
	return inputSlotRects.find (slotId) != inputSlotRects.end ();
}

void NodeDrawingImage::AddInputSlotRect (const NE::SlotId& slotId, const Rect& rect)
{
	DBGASSERT (inputSlotRects.find (slotId) == inputSlotRects.end ());
	inputSlotRects.insert ({ slotId, rect });
}

const Rect& NodeDrawingImage::GetInputSlotRect (const NE::SlotId& slotId) const
{
	return inputSlotRects.find (slotId)->second;
}

bool NodeDrawingImage::HasOutputSlotRect (const NE::SlotId& slotId) const
{
	return outputSlotRects.find (slotId) != outputSlotRects.end ();
}

void NodeDrawingImage::AddOutputSlotRect (const NE::SlotId& slotId, const Rect& rect)
{
	DBGASSERT (outputSlotRects.find (slotId) == outputSlotRects.end ());
	outputSlotRects.insert ({ slotId, rect });
}

const Rect& NodeDrawingImage::GetOutputSlotRect (const NE::SlotId& slotId) const
{
	return outputSlotRects.find (slotId)->second;
}

void NodeDrawingImage::AddSpecialRect (const std::string& rectId, const Rect& rect)
{
	DBGASSERT (specialRects.find (rectId) == specialRects.end ());
	specialRects.insert ({ rectId, rect });
}

bool NodeDrawingImage::HasSpecialRect (const std::string& rectId) const
{
	return specialRects.find (rectId) != specialRects.end ();
}

const Rect& NodeDrawingImage::GetSpecialRect (const std::string& rectId) const
{
	return specialRects.find (rectId)->second;
}

}
