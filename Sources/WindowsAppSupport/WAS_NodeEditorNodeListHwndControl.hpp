#ifndef WAS_NODEEDITORNODELISTHWNDCONTROL_HPP
#define WAS_NODEEDITORNODELISTHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

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

class NodeEditorNodeListHwndControl
{
public:
	using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

	NodeEditorNodeListHwndControl ();
	~NodeEditorNodeListHwndControl ();

	bool					Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height);

	void					Resize (int x, int y, int width, int height);
	HWND					GetEditorHandle ();
	NUIE::DrawingContext&	GetEditorContext ();
	void					InvalidateEditor ();

	void					RegisterNode (const std::wstring& group, const std::wstring& text, const CreatorFunction& creator);

	void					TreeViewDoubleClick (LPNMHDR lpnmhdr);
	void					TreeViewSelectionChanged (LPNMTREEVIEW lpnmtv);
	void					TreeViewBeginDrag (LPNMTREEVIEW lpnmtv);
	void					TreeViewEndDrag (int x, int y);

private:
	void					CreateNode (LPARAM nodeId, int screenX, int screenY);

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
