#ifndef WAS_NODEEDITORNODETREEHWNDCONTROL_HPP
#define WAS_NODEEDITORNODETREEHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "NUIE_NodeTree.hpp"
#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_NodeTree.hpp"

namespace WAS
{

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
	virtual void					Draw () override;
	
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	void							FillNodeTree (const NUIE::NodeTree& nodeTree);
	void							TreeViewDoubleClick (LPNMHDR lpnmhdr);
	void							TreeViewSelectionChanged (LPNMTREEVIEW lpnmtv);
	void							TreeViewBeginDrag (LPNMTREEVIEW lpnmtv);
	void							TreeViewEndDrag (int x, int y);

private:
	void							CreateNode (LPARAM nodeId, int screenX, int screenY);

	NodeTreeView										nodeTreeView;
	NodeEditorHwndControl								nodeEditorControl;
	CustomControl										mainControl;

	LPARAM												selectedNode;
	LPARAM												draggedNode;
	std::unordered_map<LPARAM, NUIE::CreatorFunction>	nodeIdToCreator;
};

}

#endif
