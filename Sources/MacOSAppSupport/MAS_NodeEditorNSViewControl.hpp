#ifndef MAS_NODEEDITORNSVIEWCONTROL_HPP
#define MAS_NODEEDITORNSVIEWCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "NUIE_NativeNodeEditorControl.hpp"
#include "NUIE_DrawingContext.hpp"

namespace MAS
{

class NodeEditorNSViewControl : public NUIE::NativeNodeEditorControl
{
public:
	NodeEditorNSViewControl ();
	NodeEditorNSViewControl (const NUIE::NativeDrawingContextPtr& nativeContext);
	virtual ~NodeEditorNSViewControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height) override;
	virtual void*					GetEditorNativeHandle () const override;
	virtual bool					IsEditorFocused () const override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual void					Draw () override;

	virtual NUIE::DrawingContext&	GetDrawingContext () override;
	NUIE::NodeEditor*				GetNodeEditor ();

private:
	NUIE::NodeEditor*				nodeEditor;
	NUIE::NativeDrawingContextPtr	nativeContext;
	void*							nsView;
};

}

#endif
