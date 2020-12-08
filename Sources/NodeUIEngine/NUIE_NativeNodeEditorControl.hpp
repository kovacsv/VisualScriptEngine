#ifndef NUIE_NATIVENODEEDITORCONTROL_HPP
#define NUIE_NATIVENODEEDITORCONTROL_HPP

#include "NUIE_NodeEditor.hpp"

namespace NUIE
{

class NativeNodeEditorControl
{
public:
	enum class InputHandling
	{
		Enabled,
		Disabled
	};

	NativeNodeEditorControl (InputHandling inputHandling);
	virtual ~NativeNodeEditorControl ();

	bool						IsInputHandlingEnabled () const;

	virtual bool				Init (NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height) = 0;
	virtual void*				GetEditorNativeHandle () const = 0;
	virtual bool				IsMouseOverEditorWindow () const = 0;

	virtual void				Resize (int x, int y, int width, int height) = 0;
	virtual void				Invalidate () = 0;
	virtual void				Draw () = 0;

	virtual DrawingContext&		GetDrawingContext () = 0;

private:
	InputHandling				inputHandling;
};

using NativeNodeEditorControlPtr = std::shared_ptr<NativeNodeEditorControl>;
using NativeNodeEditorControlConstPtr = std::shared_ptr<const NativeNodeEditorControl>;

}

#endif
