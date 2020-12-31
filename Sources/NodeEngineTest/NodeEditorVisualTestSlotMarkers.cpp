#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeCommonMenuCommands.hpp"
#include "NUIE_FeatureToggle.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestSlotMarkers
{

const BasicSkinParams& GetSkinParamsWithMarkers ()
{
	static const BasicSkinParams markersSkinParams (
		/*backgroundColor*/ NUIE::Color (250, 250, 250),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*connectionMarker*/ NUIE::SkinParams::ConnectionMarker::Circle,
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
		/*slotMarker*/ NUIE::SkinParams::SlotMarker::Circle,
		/*hiddenSlotMarker*/ NUIE::SkinParams::HiddenSlotMarker::Arrow,
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
		/*groupPadding*/ 12.0
	);
	return markersSkinParams;
}

TEST (SlotMarkersTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetSkinParamsWithMarkers ());
	ASSERT (env.CheckReference (L"Markers_Initial.svg"));

	{ // start connecting double output slot without target
		Point doubleOutputSlotPosition = env.doubleUpDownNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
		Point targetPosition = doubleOutputSlotPosition + Point (200.0, -100.0);
		env.DragDrop (doubleOutputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference (L"Markers_DuringConnectionForward.svg"));
		});
	}

	{ // start connecting viewer input slot without target
		Point viewerInputSlotPosition = env.viewerUINode1->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
		Point targetPosition = viewerInputSlotPosition + Point (-200.0, 100.0);
		env.DragDrop (viewerInputSlotPosition, targetPosition, [&] () {
			ASSERT (env.CheckReference (L"Markers_DuringConnectionBackward.svg"));
		});
	}
}

TEST (SlotMarkersFitToWindowTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetSkinParamsWithMarkers ());
	env.nodeEditor.FitToWindow ();
	ASSERT (env.CheckReference (L"Markers_FitToWindow.svg"));
}

TEST (HideConnectionsTest)
{
	FeatureEnableGuard featureGuard (Feature::ConnectionDisplay, true);
	SimpleNodeEditorTestEnvWithConnections env (GetSkinParamsWithMarkers ());

	ASSERT (env.CheckReference (L"HideConnectionsTest_Initial.svg"));
	
	{ // hide input connections for viewer 2
		env.SetNextCommandName (L"Hidden");
		env.RightClick (env.viewer2InputSlotRect.GetCenter ());
		ASSERT (env.CheckReference (L"HideConnectionsTest_ConnectionHidden.svg"));
	}
	
	{ // select viewer 2
		env.Click (env.viewer2HeaderPoint);
		ASSERT (env.CheckReference (L"HideConnectionsTest_Viewer2Selected.svg"));
	}
}

}
