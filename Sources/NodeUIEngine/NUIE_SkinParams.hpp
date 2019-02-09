#ifndef NUIE_SKINPARAMS_HPP
#define NUIE_SKINPARAMS_HPP

#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"

namespace NUIE
{

class SkinParams
{
public:
	SkinParams ();
	virtual ~SkinParams ();

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
	virtual const Pen&		GetNodeSelectionRectPen () const = 0;

	virtual const Color&	GetHasValueStatusColor () const = 0;
	virtual const Color&	GetHasNoValueStatusColor () const = 0;

	virtual const Pen&		GetButtonBorderPen () const = 0;
	virtual const Color&	GetButtonBackgroundColor () const = 0;
	virtual const Color&	GetSelectedButtonBackgroundColor () const = 0;

	virtual const Color&	GetTextPanelTextColor () const = 0;
	virtual const Color&	GetTextPanelBackgroundColor () const = 0;

	virtual const Font&		GetGroupNameFont () const = 0;
	virtual const Color&	GetGroupNameColor () const = 0;
	virtual const Color&	GetGroupBackgroundColor () const = 0;
	virtual double			GetGroupPadding () const = 0;
};

typedef std::shared_ptr<SkinParams>			SkinParamsPtr;
typedef std::shared_ptr<const SkinParams>	SkinParamsConstPtr;

class BasicSkinParams : public SkinParams
{
public:
	BasicSkinParams () = delete;
	BasicSkinParams (
		const Color&	backgroundColor,
		const Pen&		connectionLinePen,
		const double&	nodePadding,
		const Font&		nodeTextFont,
		const Color&	nodeHeaderTextColor,
		const Color&	nodeContentTextColor,
		const Color&	nodeHeaderBackgroundColor,
		const Color&	nodeContentBackgroundColor,
		const Pen&		nodeBorderPen,
		const Color&	slotTextColor,
		const Color&	slotTextBackgroundColor,
		const Color&	selectionBlendColor,
		const Pen&		selectionRectPen,
		const Pen&		nodeSelectionRectPen,
		const Color&	hasValueStatusColor,
		const Color&	hasNoValueStatusColor,
		const Pen&		buttonBorderPen,
		const Color&	buttonBackgroundColor,
		const Color&	selectedButtonBackgroundColor,
		const Color&	textPanelTextColor,
		const Color&	textPanelBackgroundColor,
		const Font&		groupNameFont,
		const Color&	groupNameColor,
		const Color&	groupBackgroundColor,
		const double&	groupPadding	
	);
	virtual ~BasicSkinParams ();

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
	virtual const Pen&		GetNodeSelectionRectPen () const override;

	virtual const Color&	GetHasValueStatusColor () const override;
	virtual const Color&	GetHasNoValueStatusColor () const override;

	virtual const Pen&		GetButtonBorderPen () const override;
	virtual const Color&	GetButtonBackgroundColor () const override;
	virtual const Color&	GetSelectedButtonBackgroundColor () const override;

	virtual const Color&	GetTextPanelTextColor () const override;
	virtual const Color&	GetTextPanelBackgroundColor () const override;

	virtual const Font&		GetGroupNameFont () const override;
	virtual const Color&	GetGroupNameColor () const override;
	virtual const Color&	GetGroupBackgroundColor () const override;
	virtual double			GetGroupPadding () const override;

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
	Pen		nodeSelectionRectPen;

	Color	hasValueStatusColor;
	Color	hasNoValueStatusColor;

	Pen		buttonBorderPen;
	Color	buttonBackgroundColor;
	Color	selectedButtonBackgroundColor;

	Color	textPanelTextColor;
	Color	textPanelBackgroundColor;

	Font	groupNameFont;
	Color	groupNameColor;
	Color	groupBackgroundColor;
	double	groupPadding;
};

const BasicSkinParams& GetDefaultSkinParams ();

}

#endif
