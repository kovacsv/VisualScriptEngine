#ifndef WAS_NODEEDITORHWNDCONTROL_HPP
#define WAS_NODEEDITORHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "NUIE_NativeNodeEditorControl.hpp"
#include "NUIE_DrawingContext.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_CustomControl.hpp"
#include "WAS_NodeTree.hpp"

namespace WAS
{

class NodeEditorHwndControl : public NUIE::NativeNodeEditorControl
{
public:
	NodeEditorHwndControl ();
	NodeEditorHwndControl (const NUIE::NativeDrawingContextPtr& nativeContext);
	virtual ~NodeEditorHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height) override;
	virtual void*					GetEditorNativeHandle () const override;
	virtual bool					IsEditorFocused () const override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	NUIE::NodeEditor*				GetNodeEditor ();
	void							Draw ();

private:
	NUIE::NodeEditor*				nodeEditor;
	NUIE::NativeDrawingContextPtr	nativeContext;
	CustomControl					control;
};

}

#endif
