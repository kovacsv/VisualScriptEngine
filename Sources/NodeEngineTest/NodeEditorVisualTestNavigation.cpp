#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "VisualTestFramework.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestNavigation
{

TEST (PanAndZoomTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"PanAndZoom_Basic.svg"));

	Point doubleOutputSlotPosition = env.doubleUpDownNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point rangeStartInputSlotPosition = env.rangeInputNode->GetInputSlotRect (env.uiEnvironment, SlotId ("start")).GetCenter ();
	Point rangeOutputSlotPosition = env.rangeInputNode->GetOutputSlotRect (env.uiEnvironment, SlotId ("out")).GetCenter ();
	Point viewer2InputSlotPosition = env.viewerUINode2->GetInputSlotRect (env.uiEnvironment, SlotId ("in")).GetCenter ();
	{
		env.DragDrop (doubleOutputSlotPosition, rangeStartInputSlotPosition);	
		env.DragDrop (rangeOutputSlotPosition, viewer2InputSlotPosition);
		ASSERT (env.CheckReference (L"PanAndZoom_Connections.svg"));
	}

	Point panStartPoint = env.rangeInputNode->GetRect (env.uiEnvironment).GetTopLeft ();
	Point panEndPoint = panStartPoint + Point (50.0, -50.0);

	{
		env.nodeEditor.OnMouseDown (EmptyModifierKeys, MouseButton::Right, (int) panStartPoint.GetX (), (int) panStartPoint.GetY ());
		env.nodeEditor.OnMouseMove (EmptyModifierKeys, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		env.nodeEditor.OnMouseUp (EmptyModifierKeys, MouseButton::Right, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
		ASSERT (env.CheckReference (L"PanAndZoom_Panned.svg"));
	}

	for (size_t i = 0; i < 5; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Forward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference (L"PanAndZoom_ZoomedIn.svg"));

	for (size_t i = 0; i < 10; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Forward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference (L"PanAndZoom_ZoomedIn2.svg"));

	for (size_t i = 0; i < 20; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Backward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference (L"PanAndZoom_ZoomedOut.svg"));

	for (size_t i = 0; i < 10; i++) {
		env.nodeEditor.OnMouseWheel (EmptyModifierKeys, MouseWheelRotation::Backward, (int) panEndPoint.GetX (), (int) panEndPoint.GetY ());
	}
	ASSERT (env.CheckReference (L"PanAndZoom_ZoomedOut2.svg"));
}

TEST (SwipeTest)
{
	SimpleNodeEditorTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"Swipe_Basic.svg"));
	env.nodeEditor.OnMouseSwipe (EmptyModifierKeys, 10, 100);
	ASSERT (env.CheckReference (L"Swipe_AfterSwipe.svg"));
}

TEST (FitToWindowTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"FitToWindow_Basic.svg"));
	env.nodeEditor.FitToWindow ();
	ASSERT (env.CheckReference (L"FitToWindow_Fit.svg"));
}

TEST (AlignToWindowTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"AlignToWindow_Basic.svg"));
	env.nodeEditor.AlignToWindow ();
	ASSERT (env.CheckReference (L"AlignToWindow_Align.svg"));
}

TEST (AlignToWindowTestWithScale)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	env.uiEnvironment.SetWindowScale (1.5);
	ASSERT (env.CheckReference (L"AlignToWindowWithScale_Basic.svg"));
	env.nodeEditor.AlignToWindow ();
	ASSERT (env.CheckReference (L"AlignToWindowWithScale_Align.svg"));
}

TEST (CenterToWindowTest)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"CenterToWindow_Basic.svg"));
	env.nodeEditor.CenterToWindow ();
	ASSERT (env.CheckReference (L"CenterToWindow_Align.svg"));
}

TEST (CenterToWindowTestWithScale)
{
	SimpleNodeEditorTestEnvWithConnections env (GetDefaultSkinParams ());
	env.uiEnvironment.SetWindowScale (1.5);
	ASSERT (env.CheckReference (L"CenterToWindowWithScale_Basic.svg"));
	env.nodeEditor.CenterToWindow ();
	ASSERT (env.CheckReference (L"CenterToWindowWithScale_Align.svg"));
}

}
