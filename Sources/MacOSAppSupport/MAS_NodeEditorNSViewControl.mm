#include "MAS_NodeEditorNSViewControl.hpp"
#include "NE_Debug.hpp"

namespace MAS
{

NodeEditorNSViewControl::NodeEditorNSViewControl () :
	NodeEditorNSViewControl (NUIE::NativeDrawingContextPtr (nullptr)) // TODO
{

}

NodeEditorNSViewControl::NodeEditorNSViewControl (const NUIE::NativeDrawingContextPtr& nativeContext) :
	NativeNodeEditorControl (),
	nodeEditor (nullptr),
	nativeContext (nativeContext)
{

}

NodeEditorNSViewControl::~NodeEditorNSViewControl ()
{

}

bool NodeEditorNSViewControl::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	#pragma unused (nativeParentHandle)
	#pragma unused (x)
	#pragma unused (y)
	#pragma unused (width)
	#pragma unused (height)
	
	nodeEditor = nodeEditorPtr;
	DBGASSERT (nodeEditor != nullptr);

	// TODO
	return true;
}

void* NodeEditorNSViewControl::GetEditorNativeHandle () const
{
	// TODO
	return nullptr;
}

bool NodeEditorNSViewControl::IsEditorFocused () const
{
	// TODO
	return false;
}

void NodeEditorNSViewControl::Resize (int x, int y, int width, int height)
{
	#pragma unused (x)
	#pragma unused (y)
	#pragma unused (width)
	#pragma unused (height)

	// TODO
}

void NodeEditorNSViewControl::Invalidate ()
{
	// TODO
}

void NodeEditorNSViewControl::Draw ()
{
	// TODO
}

NUIE::DrawingContext& NodeEditorNSViewControl::GetDrawingContext ()
{
	return *nativeContext;
}

NUIE::NodeEditor* NodeEditorNSViewControl::GetNodeEditor ()
{
	return nodeEditor;
}

}
