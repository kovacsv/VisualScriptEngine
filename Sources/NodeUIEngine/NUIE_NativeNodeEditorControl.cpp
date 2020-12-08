#include "NUIE_NativeNodeEditorControl.hpp"

namespace NUIE
{

NativeNodeEditorControl::NativeNodeEditorControl () :
	inputHandlingEnabled (true)
{

}

NativeNodeEditorControl::~NativeNodeEditorControl ()
{

}

bool NativeNodeEditorControl::IsInputHandlingEnabled () const
{
	return inputHandlingEnabled;
}

void NativeNodeEditorControl::EnableInputHandling (bool enabled)
{
	inputHandlingEnabled = enabled;
}

}
