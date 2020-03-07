#ifndef WAS_NODEEDITORHWNDCONTROL_HPP
#define WAS_NODEEDITORHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_CustomControl.hpp"
#include "WAS_NodeTree.hpp"

namespace WAS
{

class NodeEditorHwndBasedControl
{
public:
	NodeEditorHwndBasedControl ();
	virtual ~NodeEditorHwndBasedControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, const NodeTree& nodeTree, HWND parentHandle, int x, int y, int width, int height) = 0;
	virtual const NodeTree&			GetNodeTree () const = 0;
	virtual HWND					GetEditorHandle () const = 0;

	virtual void					Resize (int x, int y, int width, int height) = 0;
	virtual void					Invalidate () = 0;
	virtual NUIE::DrawingContext&	GetDrawingContext () = 0;
};

using NodeEditorHwndBasedControlPtr = std::shared_ptr<NodeEditorHwndBasedControl>;

class NodeEditorHwndControl : public NodeEditorHwndBasedControl
{
public:
	NodeEditorHwndControl ();
	~NodeEditorHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, const NodeTree& newNodeTree, HWND parentHandle, int x, int y, int width, int height) override;
	virtual const NodeTree&			GetNodeTree () const override;
	virtual HWND					GetEditorHandle () const override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	NUIE::NodeEditor*				GetNodeEditor ();
	void							Draw ();

private:
	NUIE::NodeEditor*		nodeEditor;
	BitmapContextGdi		bitmapContext;
	NodeTree				nodeTree;
	CustomControl			control;
};

class NodeEditorNodeListHwndControl : public NodeEditorHwndBasedControl
{
public:
	NodeEditorNodeListHwndControl ();
	~NodeEditorNodeListHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, const NodeTree& nodeTree, HWND parentHandle, int x, int y, int width, int height) override;
	virtual const NodeTree&			GetNodeTree () const override;
	virtual HWND					GetEditorHandle () const override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

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
