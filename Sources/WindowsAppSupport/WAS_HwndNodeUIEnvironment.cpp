#include "WAS_HwndNodeUIEnvironment.hpp"

namespace WAS
{

HwndNodeUIEnvironment::HwndNodeUIEnvironment (const NE::EvaluationDataPtr& evalData) :
	NUIE::NodeUIEnvironment (),
	stringSettings (L'.', L',', 2),
	skinParams (),
	eventHandlers (),
	evaluationEnv (evalData),
	nodeEditorControl ()
{
	
}

void HwndNodeUIEnvironment::Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle)
{
	RECT clientRect;
	GetClientRect (parentHandle, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	nodeEditorControl.Init (nodeEditorPtr, parentHandle, 0, 0, width, height);
	eventHandlers.Init (nodeEditorControl.GetEditorHandle ());
}

void HwndNodeUIEnvironment::RegisterNode (const std::wstring& group, const std::wstring& text, const std::function<NUIE::UINodePtr (const NUIE::Point&)>& creator)
{
	nodeEditorControl.RegisterNode (group, text, creator);
}

void HwndNodeUIEnvironment::OnResize (int x, int y, int width, int height)
{
	nodeEditorControl.Resize (x, y, width, height);
}

const NE::StringSettings& HwndNodeUIEnvironment::GetStringSettings ()
{
	return stringSettings;
}

const NUIE::SkinParams& HwndNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NUIE::DrawingContext& HwndNodeUIEnvironment::GetDrawingContext ()
{
	return nodeEditorControl.GetEditorContext ();
}
	
NE::EvaluationEnv& HwndNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void HwndNodeUIEnvironment::OnEvaluationBegin ()
{

}

void HwndNodeUIEnvironment::OnEvaluationEnd ()
{

}

void HwndNodeUIEnvironment::OnValuesRecalculated ()
{
		
}

void HwndNodeUIEnvironment::OnRedrawRequested ()
{
	nodeEditorControl.InvalidateEditor ();
}

NUIE::EventHandlers& HwndNodeUIEnvironment::GetEventHandlers ()
{
	return eventHandlers;
}

double HwndNodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

}
