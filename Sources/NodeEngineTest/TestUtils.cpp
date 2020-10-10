#include "TestUtils.hpp"
#include "NE_StringConverter.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_DrawingContext.hpp"

TestUIEnvironment::TestUIEnvironment () :
	TestUIEnvironment (nullptr)
{

}

TestUIEnvironment::TestUIEnvironment (const EvaluationDataPtr& data) :
	evalEnv (data),
	eventHandler (),
	clipboardHandler ()
{

}

const NE::StringConverter& TestUIEnvironment::GetStringConverter ()
{
	return GetDefaultStringConverter ();
}

const SkinParams& TestUIEnvironment::GetSkinParams ()
{
	return GetDefaultSkinParams ();
}

DrawingContext& TestUIEnvironment::GetDrawingContext ()
{
	static NullDrawingContext context;
	return context;
}

double TestUIEnvironment::GetWindowScale ()
{
	return 1.0;
}

NE::EvaluationEnv& TestUIEnvironment::GetEvaluationEnv ()
{
	return evalEnv;
}

void TestUIEnvironment::OnEvaluationBegin ()
{

}

void TestUIEnvironment::OnEvaluationEnd ()
{

}

void TestUIEnvironment::OnValuesRecalculated ()
{

}

void TestUIEnvironment::OnRedrawRequested ()
{

}

NUIE::EventHandler& TestUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

NUIE::ClipboardHandler& TestUIEnvironment::GetClipboardHandler ()
{
	return clipboardHandler;
}

void TestUIEnvironment::OnSelectionChanged (const Selection&)
{

}

void TestUIEnvironment::OnUndoStateChanged (const UndoState&)
{

}
