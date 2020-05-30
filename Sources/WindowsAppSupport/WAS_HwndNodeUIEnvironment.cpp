#include "WAS_HwndNodeUIEnvironment.hpp"

namespace WAS
{

HwndNodeUIEnvironment::HwndNodeUIEnvironment (	const NUIE::NativeNodeEditorControlPtr& nodeEditorControl,
												const NE::StringConverterPtr& stringConverter,
												const NUIE::SkinParamsPtr& skinParams,
												const WAS::HwndEventHandlerPtr& eventHandler,
												const NE::EvaluationDataPtr& evalData) :
	NUIE::NodeUIEnvironment (),
	nodeEditorControl (nodeEditorControl),
	stringConverter (stringConverter),
	skinParams (skinParams),
	eventHandler (eventHandler),
	clipboardHandler (new NUIE::InMemoryClipboardHandler ()),
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
	eventHandler->Init (&*nodeEditorControl);
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

NUIE::EventHandler& HwndNodeUIEnvironment::GetEventHandler ()
{
	return *eventHandler;
}

NUIE::ClipboardHandler& HwndNodeUIEnvironment::GetClipboardHandler ()
{
	return *clipboardHandler;
}

NUIE::NativeNodeEditorControlPtr HwndNodeUIEnvironment::GetNodeEditorControl () const
{
	return nodeEditorControl;
}

}
