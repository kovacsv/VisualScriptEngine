#include "NUIE_SkinParams.hpp"
#include "NE_Localization.hpp"

namespace NUIE
{

NamedColorSet::NamedColorSet (const std::vector<NamedColor>& colors) :
	colors (colors)
{
}

const std::vector<NamedColorSet::NamedColor>& NamedColorSet::GetColors () const
{
	return colors;
}

SkinParams::SkinParams ()
{

}

SkinParams::~SkinParams ()
{

}

BasicSkinParams::BasicSkinParams (
	const Color&			backgroundColor,
	const Pen&				connectionLinePen,
	const double&			nodePadding,
	const Pen&				nodeBorderPen,
	const Font&				nodeHeaderTextFont,
	const Color&			nodeHeaderTextColor,
	const Color&			nodeHeaderErrorTextColor,
	const Color&			nodeHeaderBackgroundColor,
	const Color&			nodeHeaderErrorBackgroundColor,
	const Font&				nodeContentTextFont,
	const Color&			nodeContentTextColor,
	const Color&			nodeContentBackgroundColor,
	const Color&			slotTextColor,
	const Color&			slotTextBackgroundColor,
	bool					needToDrawSlotCircles,
	const Size&				slotCircleSize,
	const BlendColor&		disabledBlendColor,
	const BlendColor&		selectionBlendColor,
	const Pen&				selectionRectPen,
	const Pen&				nodeSelectionRectPen,
	const Pen&				buttonBorderPen,
	const Color&			buttonBackgroundColor,
	const Color&			textPanelTextColor,
	const Color&			textPanelBackgroundColor,
	const Font&				groupNameFont,
	const Color&			groupNameColor,
	const NamedColorSet&	groupBackgroundColors,
	const double&			groupPadding,
	const double&			mouseMoveMinOffset
) :
	backgroundColor (backgroundColor),
	connectionLinePen (connectionLinePen),
	nodePadding (nodePadding),
	nodeBorderPen (nodeBorderPen),
	nodeHeaderTextFont (nodeHeaderTextFont),
	nodeHeaderTextColor (nodeHeaderTextColor),
	nodeHeaderErrorTextColor (nodeHeaderErrorTextColor),
	nodeHeaderBackgroundColor (nodeHeaderBackgroundColor),
	nodeHeaderErrorBackgroundColor (nodeHeaderErrorBackgroundColor),
	nodeContentTextFont (nodeContentTextFont),
	nodeContentTextColor (nodeContentTextColor),
	nodeContentBackgroundColor (nodeContentBackgroundColor),
	slotTextColor (slotTextColor),
	slotTextBackgroundColor (slotTextBackgroundColor),
	needToDrawSlotCircles (needToDrawSlotCircles),
	slotCircleSize (slotCircleSize),
	disabledBlendColor (disabledBlendColor),
	selectionBlendColor (selectionBlendColor),
	selectionRectPen (selectionRectPen),
	nodeSelectionRectPen (nodeSelectionRectPen),
	buttonBorderPen (buttonBorderPen),
	buttonBackgroundColor (buttonBackgroundColor),
	textPanelTextColor (textPanelTextColor),
	textPanelBackgroundColor (textPanelBackgroundColor),
	groupNameFont (groupNameFont),
	groupNameColor (groupNameColor),
	groupBackgroundColors (groupBackgroundColors),
	groupPadding (groupPadding),
	mouseMoveMinOffset (mouseMoveMinOffset)
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

const Pen& BasicSkinParams::GetNodeBorderPen () const
{
	return nodeBorderPen;
}

const Font& BasicSkinParams::GetNodeHeaderTextFont () const
{
	return nodeHeaderTextFont;
}

const Color& BasicSkinParams::GetNodeHeaderTextColor () const
{
	return nodeHeaderTextColor;
}

const Color& BasicSkinParams::GetNodeHeaderErrorTextColor () const
{
	return nodeHeaderErrorTextColor;
}

const Color& BasicSkinParams::GetNodeHeaderBackgroundColor () const
{
	return nodeHeaderBackgroundColor;
}

const Color& BasicSkinParams::GetNodeHeaderErrorBackgroundColor () const
{
	return nodeHeaderErrorBackgroundColor;
}

const Font& BasicSkinParams::GetNodeContentTextFont () const
{
	return nodeContentTextFont;
}

const Color& BasicSkinParams::GetNodeContentTextColor () const
{
	return nodeContentTextColor;
}

const Color& BasicSkinParams::GetNodeContentBackgroundColor () const
{
	return nodeContentBackgroundColor;
}

const Color& BasicSkinParams::GetSlotTextColor () const
{
	return slotTextColor;
}

const Color& BasicSkinParams::GetSlotTextBackgroundColor () const
{
	return slotTextBackgroundColor;
}

bool BasicSkinParams::NeedToDrawSlotCircles () const
{
	return needToDrawSlotCircles;
}

const Size& BasicSkinParams::GetSlotCircleSize () const
{
	return slotCircleSize;
}

const BlendColor& BasicSkinParams::GetDisabledBlendColor () const
{
	return disabledBlendColor;
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

const Pen& BasicSkinParams::GetButtonBorderPen () const
{
	return buttonBorderPen;
}

const Color& BasicSkinParams::GetButtonBackgroundColor () const
{
	return buttonBackgroundColor;
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

const NamedColorSet& BasicSkinParams::GetGroupBackgroundColors () const
{
	return groupBackgroundColors;
}

double BasicSkinParams::GetGroupPadding () const
{
	return groupPadding;
}

double BasicSkinParams::GetMouseMoveMinOffset () const
{
	return mouseMoveMinOffset;
}

const BasicSkinParams& GetDefaultSkinParams ()
{
	static const BasicSkinParams defaultSkinParams (
		/*backgroundColor*/ Color (240, 240, 240),
		/*connectionLinePen*/ Pen (Color (0, 0, 0), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ Pen (Color (0, 0, 0), 1.0),
		/*nodeHeaderTextFont*/ Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ Color (255, 255, 255),
		/*nodeHeaderErrorTextColor*/ Color (255, 255, 255),
		/*nodeHeaderBackgroundColor*/ Color (100, 100, 100),
		/*nodeHeaderErrorBackgroundColor*/ Color (225, 0, 0),
		/*nodeContentTextFont*/ Font (L"Arial", 16.0),
		/*nodeContentTextColor*/ Color (0, 0, 0),
		/*nodeContentBackgroundColor*/ Color (200, 200, 200),
		/*slotTextColor*/ Color (0, 0, 0),
		/*slotTextBackgroundColor*/ Color (225, 225, 225),
		/*needToDrawSlotCircles*/ false,
		/*slotCircleSize*/ Size (8.0, 8.0),
		/*selectionBlendColor*/ BlendColor (Color (240, 240, 240), 0.5),
		/*disabledBlendColor*/ BlendColor (Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ Pen (Color (0, 138, 184), 1.0),
		/*nodeSelectionRectPen*/ Pen (Color (0, 138, 184), 5.0),
		/*buttonBorderPen*/ Pen (Color (50, 75, 100), 1.0),
		/*buttonBackgroundColor*/ Color (150, 175, 200),
		/*textPanelTextColor*/ Color (0, 0, 0),
		/*textPanelBackgroundColor*/ Color (255, 255, 100),
		/*groupNameFont*/ Font (L"Arial", 18.0),
		/*groupNameColor*/ Color (0, 0, 0),
		/*groupBackgroundColors*/ NamedColorSet ({
			{ NE::LocalizeString (L"Blue"), Color (160, 200, 240) },
			{ NE::LocalizeString (L"Green"), Color (160, 239, 160) },
			{ NE::LocalizeString (L"Red"), Color (239, 189, 160) }
		}),
		/*groupPadding*/ 10.0,
		/*mouseMoveMinOffset*/ 2.0
	);
	return defaultSkinParams;
}

}
