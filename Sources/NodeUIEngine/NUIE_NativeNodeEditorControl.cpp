#include "NUIE_NativeNodeEditorControl.hpp"

namespace NUIE
{

NativeNodeEditorControl::NativeNodeEditorControl (InputHandling inputHandling) :
	inputHandling (inputHandling)
{

}

NativeNodeEditorControl::~NativeNodeEditorControl ()
{

}

bool NativeNodeEditorControl::IsInputHandlingEnabled () const
{
	return inputHandling == InputHandling::Enabled;
}

}
