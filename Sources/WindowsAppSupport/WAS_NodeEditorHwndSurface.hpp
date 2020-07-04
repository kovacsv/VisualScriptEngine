#ifndef WAS_NODEEDITORHWNDSURFACE_HPP
#define WAS_NODEEDITORHWNDSURFACE_HPP

#include "NUIE_NodeEditor.hpp"
#include "NUIE_NativeNodeEditorControl.hpp"
#include "NUIE_DrawingContext.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

class NodeEditorHwndSurface : public NUIE::NativeNodeEditorControl
{
public:
	NodeEditorHwndSurface ();
	NodeEditorHwndSurface (const NUIE::NativeDrawingContextPtr& nativeContext);
	virtual ~NodeEditorHwndSurface ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeWindowHandle, int x, int y, int width, int height) override;
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
	HWND							windowHandle;
};

}

#endif
