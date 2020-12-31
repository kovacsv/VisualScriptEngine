#ifndef NUIE_NODEDRAWINGIMAGE_HPP
#define NUIE_NODEDRAWINGIMAGE_HPP

#include "NUIE_DrawingImage.hpp"
#include "NE_SlotId.hpp"
#include <unordered_map>

namespace NUIE
{

class NodeDrawingImage : public DrawingImage
{
public:
	NodeDrawingImage ();
	NodeDrawingImage (const NodeDrawingImage& rhs) = delete;
	~NodeDrawingImage ();

	NodeDrawingImage&	operator= (const NodeDrawingImage& rhs) = delete;

	void				Reset ();
	
	void				SetNodeRect (const Rect& rect);
	const Rect&			GetNodeRect () const;

	void				SetExtendedNodeRect (const Rect& rect);
	const Rect&			GetExtendedNodeRect () const;

	void				AddInputSlotConnPosition (const NE::SlotId& slotId, const Point& position);
	const Point&		GetInputSlotConnPosition (const NE::SlotId& slotId) const;

	void				AddOutputSlotConnPosition (const NE::SlotId& slotId, const Point& position);
	const Point&		GetOutputSlotConnPosition (const NE::SlotId& slotId) const;

	bool				HasInputSlotRect (const NE::SlotId& slotId) const;
	void				AddInputSlotRect (const NE::SlotId& slotId, const Rect& rect);
	const Rect&			GetInputSlotRect (const NE::SlotId& slotId) const;

	bool				HasOutputSlotRect (const NE::SlotId& slotId) const;
	void				AddOutputSlotRect (const NE::SlotId& slotId, const Rect& rect);
	const Rect&			GetOutputSlotRect (const NE::SlotId& slotId) const;

	void				AddSpecialRect (const std::string& rectId, const Rect& rect);
	bool				HasSpecialRect (const std::string& rectId) const;
	const Rect&			GetSpecialRect (const std::string& rectId) const;

private:
	Rect									nodeRect;
	Rect									extendedNodeRect;
	std::unordered_map<NE::SlotId, Point>	inputSlotConnPositions;
	std::unordered_map<NE::SlotId, Point>	outputSlotConnPositions;
	std::unordered_map<NE::SlotId, Rect>	inputSlotRects;
	std::unordered_map<NE::SlotId, Rect>	outputSlotRects;
	std::unordered_map<std::string, Rect>	specialRects;
};

}

#endif
