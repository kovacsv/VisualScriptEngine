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
	ConnectionMarker		connectionMarker,
	const Size&				connectionMarkerSize,
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
	SlotMarker				slotMarker,
	const Size&				slotMarkerSize,
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
	connectionMarker (connectionMarker),
	connectionMarkerSize (connectionMarkerSize),
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
	slotMarker (slotMarker),
	slotMarkerSize (slotMarkerSize),
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

SkinParams::ConnectionMarker BasicSkinParams::GetConnectionMarker () const
{
	return connectionMarker;
}

const Size& BasicSkinParams::GetConnectionMarkerSize () const
{
	return connectionMarkerSize;
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

SkinParams::SlotMarker BasicSkinParams::GetSlotMarker () const
{
	return slotMarker;
}

const Size& BasicSkinParams::GetSlotMarkerSize () const
{
	return slotMarkerSize;
}

const BlendColor& BasicSkinParams::GetDisabledBlendColor () const
{
	return disabledBlendColor;
}

const BlendColor& BasicSkinParams::GetSelectionBlendColor () const
{
	return selectionBlendColor;
}

const Pen& BasicSkinParams::GetSelectionRectPen () const
{
	return selectionRectPen;
}

const Pen& BasicSkinParams::GetNodeSelectionRectPen () const
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
		/*backgroundColor*/ NUIE::Color (250, 250, 250),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*connectionMarker*/ NUIE::SkinParams::ConnectionMarker::None,
		/*connectionMarkerSize*/ NUIE::Size (8.0, 8.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderErrorTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderBackgroundColor*/ NUIE::Color (41, 127, 255),
		/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (199, 80, 80),
		/*nodeContentTextFont*/ NUIE::Font (L"Arial", 14.0),
		/*nodeContentTextColor*/ NUIE::Color (0, 0, 0),
		/*nodeContentBackgroundColor*/ NUIE::Color (236, 236, 236),
		/*slotTextColor*/ NUIE::Color (0, 0, 0),
		/*slotTextBackgroundColor*/ NUIE::Color (246, 246, 246),
		/*slotMarker*/ NUIE::SkinParams::SlotMarker::None,
		/*slotMarkerSize*/ NUIE::Size (8.0, 8.0),
		/*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (41, 127, 255), 0.25),
		/*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 1.0),
		/*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 3.0),
		/*buttonBorderPen*/ NUIE::Pen (NUIE::Color (146, 152, 155), 1.0),
		/*buttonBackgroundColor*/ NUIE::Color (217, 217, 217),
		/*textPanelTextColor*/ NUIE::Color (0, 0, 0),
		/*textPanelBackgroundColor*/ NUIE::Color (236, 236, 236),
		/*groupNameFont*/ NUIE::Font (L"Arial", 16.0),
		/*groupNameColor*/ NUIE::Color (0, 0, 0),
		/*groupBackgroundColors*/ NUIE::NamedColorSet ({
			{ NE::LocalizeString (L"Blue"), NUIE::Color (160, 200, 240) },
			{ NE::LocalizeString (L"Green"), NUIE::Color (160, 239, 160) },
			{ NE::LocalizeString (L"Red"), NUIE::Color (239, 189, 160) }
		}),
		/*groupPadding*/ 10.0
	);
	return defaultSkinParams;
}

}
