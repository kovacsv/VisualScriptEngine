#include "NUIE_SkinParams.hpp"

namespace NUIE
{

SkinParams::SkinParams ()
{

}

SkinParams::~SkinParams ()
{

}

BasicSkinParams::BasicSkinParams (
	const Color&		backgroundColor,
	const Pen&			connectionLinePen,
	const double&		nodePadding,
	const Font&			nodeTextFont,
	const Color&		nodeHeaderTextColor,
	const Color&		nodeContentTextColor,
	const Color&		nodeHeaderBackgroundColor,
	const Color&		nodeContentBackgroundColor,
	const Pen&			nodeBorderPen,
	const Color&		slotTextColor,
	const Color&		slotTextBackgroundColor,
	const BlendColor&	selectionBlendColor,
	const Pen&			selectionRectPen,
	const Pen&			nodeSelectionRectPen,
	const Color&		hasValueStatusColor,
	const Color&		hasNoValueStatusColor,
	const Pen&			buttonBorderPen,
	const Color&		buttonBackgroundColor,
	const Color&		selectedButtonBackgroundColor,
	const Color&		textPanelTextColor,
	const Color&		textPanelBackgroundColor,
	const Font&			groupNameFont,
	const Color&		groupNameColor,
	const Color&		groupBackgroundColor,
	const double&		groupPadding
) :
	backgroundColor (backgroundColor),
	connectionLinePen (connectionLinePen),
	nodePadding (nodePadding),
	nodeTextFont (nodeTextFont),
	nodeHeaderTextColor (nodeHeaderTextColor),
	nodeContentTextColor (nodeContentTextColor),
	nodeHeaderBackgroundColor (nodeHeaderBackgroundColor),
	nodeContentBackgroundColor (nodeContentBackgroundColor),
	nodeBorderPen (nodeBorderPen),
	slotTextColor (slotTextColor),
	slotTextBackgroundColor (slotTextBackgroundColor),
	selectionBlendColor (selectionBlendColor),
	selectionRectPen (selectionRectPen),
	nodeSelectionRectPen (nodeSelectionRectPen),
	hasValueStatusColor (hasValueStatusColor),
	hasNoValueStatusColor (hasNoValueStatusColor),
	buttonBorderPen (buttonBorderPen),
	buttonBackgroundColor (buttonBackgroundColor),
	selectedButtonBackgroundColor (selectedButtonBackgroundColor),
	textPanelTextColor (textPanelTextColor),
	textPanelBackgroundColor (textPanelBackgroundColor),
	groupNameFont (groupNameFont),
	groupNameColor (groupNameColor),
	groupBackgroundColor (groupBackgroundColor),
	groupPadding (groupPadding)
{

}

BasicSkinParams::~BasicSkinParams ()
{

}

const Color& BasicSkinParams::GetBackgroundColor () const
{
	return backgroundColor;
}

const Pen& BasicSkinParams::GetConnectionLinePen () const
{
	return connectionLinePen;
}

double BasicSkinParams::GetNodePadding () const
{
	return nodePadding;
}

const Font& BasicSkinParams::GetNodeTextFont () const
{
	return nodeTextFont;
}

const Color& BasicSkinParams::GetNodeHeaderTextColor () const
{
	return nodeHeaderTextColor;
}

const Color& BasicSkinParams::GetNodeContentTextColor () const
{
	return nodeContentTextColor;
}

const Color& BasicSkinParams::GetNodeHeaderBackgroundColor () const
{
	return nodeHeaderBackgroundColor;
}

const Color& BasicSkinParams::GetNodeContentBackgroundColor () const
{
	return nodeContentBackgroundColor;
}

const Pen& BasicSkinParams::GetNodeBorderPen () const
{
	return nodeBorderPen;
}

const Color& BasicSkinParams::GetSlotTextColor () const
{
	return slotTextColor;
}

const Color& BasicSkinParams::GetSlotTextBackgroundColor () const
{
	return slotTextBackgroundColor;
}

const BlendColor& BasicSkinParams::GetSelectionBlendColor () const
{
	return selectionBlendColor;
}

const NUIE::Pen& BasicSkinParams::GetSelectionRectPen () const
{
	return selectionRectPen;
}

const NUIE::Pen& BasicSkinParams::GetNodeSelectionRectPen () const
{
	return nodeSelectionRectPen;
}

const Color& BasicSkinParams::GetHasValueStatusColor () const
{
	return hasValueStatusColor;
}

const Color& BasicSkinParams::GetHasNoValueStatusColor () const
{
	return hasNoValueStatusColor;
}

const Pen& BasicSkinParams::GetButtonBorderPen () const
{
	return buttonBorderPen;
}

const Color& BasicSkinParams::GetButtonBackgroundColor () const
{
	return buttonBackgroundColor;
}

const Color& BasicSkinParams::GetSelectedButtonBackgroundColor () const
{
	return selectedButtonBackgroundColor;
}

const Color& BasicSkinParams::GetTextPanelTextColor () const
{
	return textPanelTextColor;
}

const Color& BasicSkinParams::GetTextPanelBackgroundColor () const
{
	return textPanelBackgroundColor;
}

const Font& BasicSkinParams::GetGroupNameFont () const
{
	return groupNameFont;
}

const Color& BasicSkinParams::GetGroupNameColor () const
{
	return groupNameColor;
}

const Color& BasicSkinParams::GetGroupBackgroundColor () const
{
	return groupBackgroundColor;
}

double BasicSkinParams::GetGroupPadding () const
{
	return groupPadding;
}

const BasicSkinParams& GetDefaultSkinParams ()
{
	static const BasicSkinParams defaultSkinParams (
		/*backgroundColor*/ Color (240, 240, 240),
		/*connectionLinePen*/ Pen (Color (0, 0, 0), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeTextFont*/ Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ Color (255, 255, 255),
		/*nodeContentTextColor*/ Color (0, 0, 0),
		/*nodeHeaderBackgroundColor*/ Color (100, 100, 100),
		/*nodeContentBackgroundColor*/ Color (200, 200, 200),
		/*nodeBorderPen*/ Pen (Color (0, 0, 0), 1.0),
		/*slotTextColor*/ Color (0, 0, 0),
		/*slotTextBackgroundColor*/ Color (225, 225, 225),
		/*selectionBlendColor*/ BlendColor (Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ Pen (Color (0, 138, 184), 1.0),
		/*nodeSelectionRectPen*/ Pen (Color (0, 138, 184), 5.0),
		/*hasValueStatusColor*/ Color (0, 255, 0),
		/*hasNoValueStatusColor*/ Color (255, 0, 0),
		/*buttonBorderPen*/ Pen (Color (50, 75, 100), 1.0),
		/*buttonBackgroundColor*/ Color (150, 175, 200),
		/*selectedButtonBackgroundColor*/ Color (190, 215, 240),
		/*textPanelTextColor*/ Color (0, 0, 0),
		/*textPanelBackgroundColor*/ Color (255, 255, 100),
		/*groupNameFont*/ Font (L"Arial", 18.0),
		/*groupNameColor*/ Color (0, 0, 0),
		/*groupBackgroundColor*/ Color (160, 200, 240),
		/*groupPadding*/ 10.0
	);
	// TODO: disabled blend color
	return defaultSkinParams;
}

}
