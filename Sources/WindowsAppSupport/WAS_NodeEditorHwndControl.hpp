#ifndef WAS_NODEEDITORHWNDCONTROL_HPP
#define WAS_NODEEDITORHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_BitmapContextGdi.hpp"

namespace WAS
{

using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

class NodeTree
{
public:
	class Item
	{
	public:
		Item (const std::wstring& name, const CreatorFunction& creator);

		std::wstring		name;
		CreatorFunction		creator;
	};

	class Group
	{
	public:
		Group (const std::wstring& name);

		std::wstring		name;
		std::vector<Item>	items;
	};

	NodeTree ();

	void						AddItem (const std::wstring& groupName, const std::wstring& itemName, const CreatorFunction& creator);
	const std::vector<Group>&	GetGroups () const;

private:
	std::vector<Group> groups;
};

extern const NodeTree EmptyNodeTree;

class NodeEditorHwndBasedControl
{
public:
	NodeEditorHwndBasedControl ();
	virtual ~NodeEditorHwndBasedControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, const NodeTree& nodeTree, HWND parentHandle, int x, int y, int width, int height) = 0;
	virtual NodeTree				GetNodeTree () const = 0;
	virtual HWND					GetEditorHandle () const = 0;

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

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, const NodeTree& newNodeTree, HWND parentHandle, int x, int y, int width, int height) override;
	virtual NodeTree				GetNodeTree () const override;
	virtual HWND					GetEditorHandle () const override;

	virtual void					Resize (int x, int y, int width, int height) override;
	virtual void					Invalidate () override;
	virtual NUIE::DrawingContext&	GetDrawingContext () override;

	NUIE::NodeEditor*				GetNodeEditor ();
	const NodeTree&					GetNodeTree ();
	void							Draw ();

private:
	NUIE::NodeEditor*		nodeEditor;
	BitmapContextGdi		bitmapContext;
	NodeTree				nodeTree;
	HWND					hwnd;
};

class NodeTreeView
{
public:
	NodeTreeView ();
	~NodeTreeView ();

	bool	Init (HWND parentHandle, int x, int y, int width, int height);
	void	Resize (int x, int y, int width, int height);

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
	NodeEditorNodeListHwndControl ();
	~NodeEditorNodeListHwndControl ();

	virtual bool					Init (NUIE::NodeEditor* nodeEditorPtr, const NodeTree& nodeTree, HWND parentHandle, int x, int y, int width, int height) override;
	virtual NodeTree				GetNodeTree () const override;
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
	HWND											mainHandle;

	LPARAM											selectedNode;
	LPARAM											draggedNode;
	std::unordered_map<LPARAM, CreatorFunction>		nodeIdToCreator;
};

}

#endif
