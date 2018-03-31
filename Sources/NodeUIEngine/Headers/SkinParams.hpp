#ifndef SKINPARAMS_HPP
#define SKINPARAMS_HPP

#include "Drawing.hpp"
#include "Geometry.hpp"

namespace NUIE
{

class SkinParams
{
public:
	virtual const Color&	GetBackgroundColor () const = 0;
	virtual const Pen&		GetConnectionLinePen () const = 0;

	virtual double			GetNodePadding () const = 0;
	virtual const Font&		GetNodeTextFont () const = 0;
	virtual const Color&	GetNodeHeaderTextColor () const = 0;
	virtual const Color&	GetNodeContentTextColor () const = 0;
	virtual const Color&	GetNodeHeaderBackgroundColor () const = 0;
	virtual const Color&	GetNodeContentBackgroundColor () const = 0;
	virtual const Pen&		GetNodeBorderPen () const = 0;

	virtual const Color&	GetSlotTextColor () const = 0;
	virtual const Color&	GetSlotTextBackgroundColor () const = 0;

	virtual const Color&	GetSelectionBlendColor () const = 0;
	virtual const Pen&		GetSelectionRectPen () const = 0;

	virtual const Color&	GetHasValueStatusColor () const = 0;
	virtual const Color&	GetHasNoValueStatusColor () const = 0;
};

class DefaultSkinParams : public SkinParams
{
public:
	DefaultSkinParams ();
	virtual ~DefaultSkinParams ();

	virtual const Color&	GetBackgroundColor () const override;
	virtual const Pen&		GetConnectionLinePen () const override;

	virtual double			GetNodePadding () const override;
	virtual const Font&		GetNodeTextFont () const override;
	virtual const Color&	GetNodeHeaderTextColor () const override;
	virtual const Color&	GetNodeContentTextColor () const override;
	virtual const Color&	GetNodeHeaderBackgroundColor () const override;
	virtual const Color&	GetNodeContentBackgroundColor () const override;
	virtual const Pen&		GetNodeBorderPen () const override;

	virtual const Color&	GetSlotTextColor () const override;
	virtual const Color&	GetSlotTextBackgroundColor () const override;

	virtual const Color&	GetSelectionBlendColor () const override;
	virtual const Pen&		GetSelectionRectPen () const override;

	virtual const Color&	GetHasValueStatusColor () const override;
	virtual const Color&	GetHasNoValueStatusColor () const override;

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
