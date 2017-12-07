#ifndef SKINPARAMS_HPP
#define SKINPARAMS_HPP

#include "Drawing.hpp"
#include "Geometry.hpp"

namespace NUIE
{

class SkinParams
{
public:
	SkinParams ();
	virtual ~SkinParams ();

	const Color&	GetBackgroundColor () const;
	const Pen&		GetConnectionLinePen () const;

	double			GetNodePadding () const;
	const Font&		GetNodeTextFont () const;
	const Color&	GetNodeHeaderTextColor () const;
	const Color&	GetNodeContentTextColor () const;
	const Color&	GetNodeHeaderBackgroundColor () const;
	const Color&	GetNodeContentBackgroundColor () const;
	const Pen&		GetNodeBorderPen () const;

	const Color&	GetSlotTextColor () const;
	const Color&	GetSlotTextBackgroundColor () const;

	const Color&	GetSelectionBlendColor () const;
	const Pen&		GetSelectionRectPen () const;

	const Color&	GetHasValueStatusColor () const;
	const Color&	GetHasNoValueStatusColor () const;

protected:
	Color	backgroundColor;
	Pen		connectionLinePen;

	double	nodePadding;
	Font	nodeTextFont;
	Color	nodeHeaderTextColor;
	Color	nodeContentTextColor;
	Color	nodeHeaderBackgroundColor;
	Color	nodeContentBackgroundColor;
	Pen		nodeBorderPen;

	Color	slotTextColor;
	Color	slotTextBackgroundColor;

	Color	selectionBlendColor;
	Pen		selectionRectPen;

	Color	hasValueStatusColor;
	Color	hasNoValueStatusColor;
};

}

#endif
