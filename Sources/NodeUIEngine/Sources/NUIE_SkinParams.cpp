#include "NUIE_SkinParams.hpp"

namespace NUIE
{

DefaultSkinParams::DefaultSkinParams () :
	backgroundColor (240, 240, 240),
	connectionLinePen (Color (0, 0, 0), 1.0),
	nodePadding (5.0),
	nodeTextFont (L"Arial", 16.0),
	nodeHeaderTextColor (255, 255, 255),
	nodeContentTextColor (0, 0, 0),
	nodeHeaderBackgroundColor (100, 100, 100),
	nodeContentBackgroundColor (200, 200, 200),
	nodeBorderPen (Color (0, 0, 0), 1.0),
	slotTextColor (0, 0, 0),
	slotTextBackgroundColor (225, 225, 225),
	selectionBlendColor (0, 255, 0),
	selectionRectPen (Color (0, 138, 184), 1.0),
	hasValueStatusColor (Color (0, 255, 0)),
	hasNoValueStatusColor (Color (255, 0, 0))
{

}

DefaultSkinParams::~DefaultSkinParams ()
{

}

const Color& DefaultSkinParams::GetBackgroundColor () const
{
	return backgroundColor;
}

const Pen& DefaultSkinParams::GetConnectionLinePen () const
{
	return connectionLinePen;
}

double DefaultSkinParams::GetNodePadding () const
{
	return nodePadding;
}

const Font& DefaultSkinParams::GetNodeTextFont () const
{
	return nodeTextFont;
}

const Color& DefaultSkinParams::GetNodeHeaderTextColor () const
{
	return nodeHeaderTextColor;
}

const Color& DefaultSkinParams::GetNodeContentTextColor () const
{
	return nodeContentTextColor;
}

const Color& DefaultSkinParams::GetNodeHeaderBackgroundColor () const
{
	return nodeHeaderBackgroundColor;
}

const Color& DefaultSkinParams::GetNodeContentBackgroundColor () const
{
	return nodeContentBackgroundColor;
}

const Pen& DefaultSkinParams::GetNodeBorderPen () const
{
	return nodeBorderPen;
}

const Color& DefaultSkinParams::GetSlotTextColor () const
{
	return slotTextColor;
}

const Color& DefaultSkinParams::GetSlotTextBackgroundColor () const
{
	return slotTextBackgroundColor;
}

const NUIE::Color& DefaultSkinParams::GetSelectionBlendColor () const
{
	return selectionBlendColor;
}

const NUIE::Pen& DefaultSkinParams::GetSelectionRectPen () const
{
	return selectionRectPen;
}

const Color& DefaultSkinParams::GetHasValueStatusColor () const
{
	return hasValueStatusColor;
}

const Color& DefaultSkinParams::GetHasNoValueStatusColor () const
{
	return hasNoValueStatusColor;
}

}
