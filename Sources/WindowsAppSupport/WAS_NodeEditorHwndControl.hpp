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

class NodeEditorNodeTreeHwndControl : public NUIE::NativeNodeEditorControl
{
public:
	NodeEditorNodeTreeHwndControl ();
	NodeEditorNodeTreeHwndControl (const NUIE::NativeDrawingContextPtr& nativeContext);
	virtual ~NodeEditorNodeTreeHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height) override;
	virtual void*					GetEditorNativeHandle () const override;
	virtual bool					IsEditorFocused () const override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	void							FillNodeTree (const NodeTree& nodeTree);
	void							TreeViewDoubleClick (LPNMHDR lpnmhdr);
	void							TreeViewSelectionChanged (LPNMTREEVIEW lpnmtv);
	void							TreeViewBeginDrag (LPNMTREEVIEW lpnmtv);
	void							TreeViewEndDrag (int x, int y);

private:
	void							CreateNode (LPARAM nodeId, int screenX, int screenY);

	NodeTreeView									nodeTreeView;
	NodeEditorHwndControl							nodeEditorControl;
	CustomControl									mainControl;

	LPARAM											selectedNode;
	LPARAM											draggedNode;
	std::unordered_map<LPARAM, CreatorFunction>		nodeIdToCreator;
};

}

#endif
