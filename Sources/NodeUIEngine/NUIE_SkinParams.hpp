#ifndef NUIE_SKINPARAMS_HPP
#define NUIE_SKINPARAMS_HPP

#include "NUIE_Geometry.hpp"
#include "NUIE_Drawing.hpp"

#include <vector>
#include <memory>

namespace NUIE
{
class NamedColorSet
{
public:
	struct NamedColor
	{
		std::wstring	name;
		Color			color;
	};

	NamedColorSet (const std::vector<NamedColor>& colors);

	const std::vector<NamedColor>&	GetColors () const;

private:
	std::vector<NamedColor> colors;
};

class SkinParams
{
public:
	enum class ConnectionMarker
	{
		None,
		Circle
	};

	enum class SlotMarker
	{
		None,
		Circle
	};

	enum class HiddenSlotMarker
	{
		None,
		Arrow
	};

	SkinParams ();
	virtual ~SkinParams ();

	virtual const Color&			GetBackgroundColor () const = 0;
	virtual const Pen&				GetConnectionLinePen () const = 0;
	virtual ConnectionMarker		GetConnectionMarker () const = 0;
	virtual const Size&				GetConnectionMarkerSize () const = 0;

	virtual double					GetNodePadding () const = 0;
	virtual const Pen&				GetNodeBorderPen () const = 0;

	virtual const Font&				GetNodeHeaderTextFont () const = 0;
	virtual const Color&			GetNodeHeaderTextColor () const = 0;
	virtual const Color&			GetNodeHeaderErrorTextColor () const = 0;
	virtual const Color&			GetNodeHeaderBackgroundColor () const = 0;
	virtual const Color&			GetNodeHeaderErrorBackgroundColor () const = 0;

	virtual const Font&				GetNodeContentTextFont () const = 0;
	virtual const Color&			GetNodeContentTextColor () const = 0;
	virtual const Color&			GetNodeContentBackgroundColor () const = 0;

	virtual const Color&			GetSlotTextColor () const = 0;
	virtual const Color&			GetSlotTextBackgroundColor () const = 0;
	virtual SlotMarker				GetSlotMarker () const = 0;
	virtual HiddenSlotMarker		GetHiddenSlotMarker () const = 0;
	virtual const Size&				GetSlotMarkerSize () const = 0;

	virtual const BlendColor&		GetDisabledBlendColor () const = 0;
	virtual const BlendColor&		GetSelectionBlendColor () const = 0;
	virtual const Pen&				GetSelectionRectPen () const = 0;
	virtual const Pen&				GetNodeSelectionRectPen () const = 0;

	virtual const Pen&				GetButtonBorderPen () const = 0;
	virtual const Color&			GetButtonBackgroundColor () const = 0;

	virtual const Color&			GetTextPanelTextColor () const = 0;
	virtual const Color&			GetTextPanelBackgroundColor () const = 0;

	virtual const Font&				GetGroupNameFont () const = 0;
	virtual const Color&			GetGroupNameColor () const = 0;
	virtual const NamedColorSet&	GetGroupBackgroundColors () const = 0;
	virtual double					GetGroupPadding () const = 0;
};

using SkinParamsPtr = std::shared_ptr<SkinParams>;
using SkinParamsConstPtr = std::shared_ptr<const SkinParams>;

class BasicSkinParams : public SkinParams
{
public:
	BasicSkinParams () = delete;
	BasicSkinParams (
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
		HiddenSlotMarker		hiddenSlotMarker,
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
	);
	virtual ~BasicSkinParams ();

	virtual const Color&			GetBackgroundColor () const override;
	virtual const Pen&				GetConnectionLinePen () const override;
	virtual ConnectionMarker		GetConnectionMarker () const override;
	virtual const Size&				GetConnectionMarkerSize () const override;

	virtual double					GetNodePadding () const override;
	virtual const Font&				GetNodeContentTextFont () const override;
	virtual const Font&				GetNodeHeaderTextFont () const override;
	virtual const Color&			GetNodeHeaderTextColor () const override;
	virtual const Color&			GetNodeHeaderErrorTextColor () const override;
	virtual const Color&			GetNodeContentTextColor () const override;
	virtual const Color&			GetNodeHeaderBackgroundColor () const override;
	virtual const Color&			GetNodeHeaderErrorBackgroundColor () const override;
	virtual const Color&			GetNodeContentBackgroundColor () const override;
	virtual const Pen&				GetNodeBorderPen () const override;

	virtual const Color&			GetSlotTextColor () const override;
	virtual const Color&			GetSlotTextBackgroundColor () const override;
	virtual SlotMarker				GetSlotMarker () const override;
	virtual HiddenSlotMarker		GetHiddenSlotMarker () const override;
	virtual const Size&				GetSlotMarkerSize () const override;

	virtual const BlendColor&		GetDisabledBlendColor () const override;
	virtual const BlendColor&		GetSelectionBlendColor () const override;
	virtual const Pen&				GetSelectionRectPen () const override;
	virtual const Pen&				GetNodeSelectionRectPen () const override;

	virtual const Pen&				GetButtonBorderPen () const override;
	virtual const Color&			GetButtonBackgroundColor () const override;

	virtual const Color&			GetTextPanelTextColor () const override;
	virtual const Color&			GetTextPanelBackgroundColor () const override;

	virtual const Font&				GetGroupNameFont () const override;
	virtual const Color&			GetGroupNameColor () const override;
	virtual const NamedColorSet&	GetGroupBackgroundColors () const override;
	virtual double					GetGroupPadding () const override;

private:
	Color				backgroundColor;
	Pen					connectionLinePen;
	ConnectionMarker	connectionMarker;
	Size				connectionMarkerSize;

	double				nodePadding;
	Pen					nodeBorderPen;

	Font				nodeHeaderTextFont;
	Color				nodeHeaderTextColor;
	Color				nodeHeaderErrorTextColor;
	Color				nodeHeaderBackgroundColor;
	Color				nodeHeaderErrorBackgroundColor;

	Font				nodeContentTextFont;
	Color				nodeContentTextColor;
	Color				nodeContentBackgroundColor;

	Color				slotTextColor;
	Color				slotTextBackgroundColor;
	SlotMarker			slotMarker;
	HiddenSlotMarker	hiddenSlotMarker;
	Size				slotMarkerSize;

	BlendColor			disabledBlendColor;
	BlendColor			selectionBlendColor;
	Pen					selectionRectPen;
	Pen					nodeSelectionRectPen;

	Pen					buttonBorderPen;
	Color				buttonBackgroundColor;

	Color				textPanelTextColor;
	Color				textPanelBackgroundColor;

	Font				groupNameFont;
	Color				groupNameColor;
	NamedColorSet		groupBackgroundColors;
	double				groupPadding;
};

const BasicSkinParams& GetDefaultSkinParams ();

}

#endif
