#ifndef WAS_NODEEDITORHWNDCONTROL_HPP
#define WAS_NODEEDITORHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_BitmapContextGdi.hpp"

namespace WAS
{

class NodeEditorHwndBasedControl
{
public:
	NodeEditorHwndBasedControl ();
	virtual ~NodeEditorHwndBasedControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height) = 0;
	virtual HWND					GetEditorHandle () = 0;

	virtual void					Resize (int x, int y, int width, int height) = 0;
	virtual void					Invalidate () = 0;
	virtual NUIE::DrawingContext&	GetDrawingContext () = 0;
};

typedef std::shared_ptr<NodeEditorHwndBasedControl> NodeEditorHwndBasedControlPtr;

class NodeEditorHwndControl : public NodeEditorHwndBasedControl
{
public:
	NodeEditorHwndControl ();
	~NodeEditorHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height) override;
	virtual HWND					GetEditorHandle () override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	NUIE::NodeEditor*				GetNodeEditor ();
	void							Draw ();

private:
	NUIE::NodeEditor*		nodeEditor;
	WAS::BitmapContextGdi	bitmapContext;
	HWND					hwnd;
};

class NodeTreeView
{
public:
	NodeTreeView ();
	~NodeTreeView ();

	bool	Init (HWND parentHandle, int x, int y, int width, int height);
	bool	HasGroup (const std::wstring& group) const;
	void	AddGroup (const std::wstring& group);
	void	AddItem (const std::wstring& group, const std::wstring& text, LPARAM lParam);
	void	ExpandAll ();

	HWND	GetWindowHandle ();

private:
	HWND											hwnd;
	std::unordered_map<std::wstring, HTREEITEM>		groups;
};

class NodeEditorNodeListHwndControl : public NodeEditorHwndBasedControl
{
public:
	using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

	NodeEditorNodeListHwndControl ();
	~NodeEditorNodeListHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height) override;
	virtual HWND					GetEditorHandle () override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	void							RegisterNode (const std::wstring& group, const std::wstring& text, const CreatorFunction& creator);

	void							TreeViewDoubleClick (LPNMHDR lpnmhdr);
	void							TreeViewSelectionChanged (LPNMTREEVIEW lpnmtv);
	void							TreeViewBeginDrag (LPNMTREEVIEW lpnmtv);
	void							TreeViewEndDrag (int x, int y);

private:
	void							CreateNode (LPARAM nodeId, int screenX, int screenY);

	NodeTreeView									nodeTreeView;
	NodeEditorHwndControl							nodeEditorControl;
	HWND											mainHandle;

	LPARAM											selectedNode;
	LPARAM											draggedNode;
	LPARAM											nextNodeId;
	std::unordered_map<LPARAM, CreatorFunction>		nodeIdToCreator;
};

}

#endif
