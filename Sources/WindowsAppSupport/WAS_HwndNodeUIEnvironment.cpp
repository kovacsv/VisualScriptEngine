#include "WAS_HwndNodeUIEnvironment.hpp"

namespace WAS
{

HwndNodeUIEnvironment::HwndNodeUIEnvironment (	const WAS::NodeEditorHwndBasedControlPtr& nodeEditorControl,
												const NE::StringConverterPtr& stringConverter,
												const NUIE::SkinParamsPtr& skinParams,
												const WAS::HwndEventHandlersPtr eventHandlers,
												const NE::EvaluationDataPtr& evalData) :
	NUIE::NodeUIEnvironment (),
	nodeEditorControl (nodeEditorControl),
	stringConverter (stringConverter),
	skinParams (skinParams),
	eventHandlers (eventHandlers),
	evaluationEnv (evalData)
{
	
}

void HwndNodeUIEnvironment::Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle)
{
	RECT clientRect;
	GetClientRect (parentHandle, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	nodeEditorControl->Init (nodeEditorPtr, parentHandle, 0, 0, width, height);
	eventHandlers->Init (&*nodeEditorControl);
}

void HwndNodeUIEnvironment::OnResize (int x, int y, int width, int height)
{
	nodeEditorControl->Resize (x, y, width, height);
}

const NE::StringConverter& HwndNodeUIEnvironment::GetStringConverter ()
{
	return *stringConverter;
}

const NUIE::SkinParams& HwndNodeUIEnvironment::GetSkinParams ()
{
	return *skinParams;
}

NUIE::DrawingContext& HwndNodeUIEnvironment::GetDrawingContext ()
{
	return nodeEditorControl->GetDrawingContext ();
}

double HwndNodeUIEnvironment::GetWindowScale ()
{
	return 1.0;
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
	nodeEditorControl->Invalidate ();
}

NUIE::EventHandlers& HwndNodeUIEnvironment::GetEventHandlers ()
{
	return *eventHandlers;
}

double HwndNodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

WAS::NodeEditorHwndBasedControlPtr HwndNodeUIEnvironment::GetNodeEditorControl () const
{
	return nodeEditorControl;
}

}
