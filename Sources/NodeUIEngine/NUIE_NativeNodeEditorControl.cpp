#include "NUIE_NativeNodeEditorControl.hpp"

namespace NUIE
{

NativeNodeEditorControl::NativeNodeEditorControl () :
	inputHandling (InputHandling::Enabled)
{

}

NativeNodeEditorControl::~NativeNodeEditorControl ()
{

}

bool NativeNodeEditorControl::IsInputHandlingEnabled () const
{
	return inputHandling == InputHandling::Enabled;
}

void NativeNodeEditorControl::SetInputHandling (InputHandling newInputHandling)
{
	inputHandling = newInputHandling;
}

}
