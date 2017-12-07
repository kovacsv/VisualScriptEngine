#include "SkinParams.hpp"

namespace NUIE
{

SkinParams::SkinParams () :
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

SkinParams::~SkinParams ()
{

}

const Color& SkinParams::GetBackgroundColor () const
{
	return backgroundColor;
}

const Pen& SkinParams::GetConnectionLinePen () const
{
	return connectionLinePen;
}

double SkinParams::GetNodePadding () const
{
	return nodePadding;
}

const Font& SkinParams::GetNodeTextFont () const
{
	return nodeTextFont;
}

const Color& SkinParams::GetNodeHeaderTextColor () const
{
	return nodeHeaderTextColor;
}

const Color& SkinParams::GetNodeContentTextColor () const
{
	return nodeContentTextColor;
}

const Color& SkinParams::GetNodeHeaderBackgroundColor () const
{
	return nodeHeaderBackgroundColor;
}

const Color& SkinParams::GetNodeContentBackgroundColor () const
{
	return nodeContentBackgroundColor;
}

const Pen& SkinParams::GetNodeBorderPen () const
{
	return nodeBorderPen;
}

const Color& SkinParams::GetSlotTextColor () const
{
	return slotTextColor;
}

const Color& SkinParams::GetSlotTextBackgroundColor () const
{
	return slotTextBackgroundColor;
}

const NUIE::Color& SkinParams::GetSelectionBlendColor () const
{
	return selectionBlendColor;
}

const NUIE::Pen& SkinParams::GetSelectionRectPen () const
{
	return selectionRectPen;
}

const Color& SkinParams::GetHasValueStatusColor () const
{
	return hasValueStatusColor;
}

const Color& SkinParams::GetHasNoValueStatusColor () const
{
	return hasNoValueStatusColor;
}

}
