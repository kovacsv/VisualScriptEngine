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
	const Font&				nodeTextFont,
	const Color&			nodeHeaderTextColor,
	const Color&			nodeContentTextColor,
	const Color&			nodeHeaderBackgroundColor,
	const Color&			nodeHeaderErrorBackgroundColor,
	const Color&			nodeContentBackgroundColor,
	const Pen&				nodeBorderPen,
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
	const double&			groupPadding
) :
	backgroundColor (backgroundColor),
	connectionLinePen (connectionLinePen),
	nodePadding (nodePadding),
	nodeTextFont (nodeTextFont),
	nodeHeaderTextColor (nodeHeaderTextColor),
	nodeContentTextColor (nodeContentTextColor),
	nodeHeaderBackgroundColor (nodeHeaderBackgroundColor),
	nodeHeaderErrorBackgroundColor (nodeHeaderErrorBackgroundColor),
	nodeContentBackgroundColor (nodeContentBackgroundColor),
	nodeBorderPen (nodeBorderPen),
	slotTextColor (slotTextColor),
	slotTextBackgroundColor (slotTextBackgroundColor),
	needToDrawSlotCircles (needToDrawSlotCircles),
	slotCircleSize (slotCircleSize),
	disabledBlendColor (disabledBlendColor),
	selectionBlendColor (selectionBlendColor),
	selectionRectPen (selectionRectPen),
	nodeSelectionRectPen (nodeSelectionRectPen),
	hasValueStatusColor (hasValueStatusColor),
	hasNoValueStatusColor (hasNoValueStatusColor),
	buttonBorderPen (buttonBorderPen),
	buttonBackgroundColor (buttonBackgroundColor),
	textPanelTextColor (textPanelTextColor),
	textPanelBackgroundColor (textPanelBackgroundColor),
	groupNameFont (groupNameFont),
	groupNameColor (groupNameColor),
	groupBackgroundColors (groupBackgroundColors),
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

const Color& BasicSkinParams::GetNodeHeaderErrorBackgroundColor () const
{
	return nodeHeaderErrorBackgroundColor;
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
		/*nodeHeaderErrorBackgroundColor*/ Color (225, 0, 0),
		/*nodeContentBackgroundColor*/ Color (200, 200, 200),
		/*nodeBorderPen*/ Pen (Color (0, 0, 0), 1.0),
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
			{ NE::Localize (L"Blue"), Color (160, 200, 240) },
			{ NE::Localize (L"Green"), Color (160, 239, 160) },
			{ NE::Localize (L"Red"), Color (239, 189, 160) }
		}),
		/*groupPadding*/ 10.0
	);
	return defaultSkinParams;
}

}
