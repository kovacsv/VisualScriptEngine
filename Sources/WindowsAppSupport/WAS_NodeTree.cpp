#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

NodeTree::Item::Item (const std::wstring& name, const CreatorFunction& creator) :
	name (name),
	creator (creator)
{

}

NodeTree::Group::Group (const std::wstring& name) :
	name (name)
{

}

NodeTree::NodeTree ()
{

}

void NodeTree::AddItem (const std::wstring& groupName, const std::wstring& itemName, const CreatorFunction& creator)
{
	Group* group = nullptr;
	auto foundGroup = std::find_if (groups.begin (), groups.end (), [&] (const Group& group) {
		return group.name == groupName;
	});
	if (foundGroup != groups.end ()) {
		group = &*foundGroup;
	} else {
		groups.push_back (Group (groupName));
		group = &groups.back ();
	}
	group->items.push_back (Item (itemName, creator));
}

const std::vector<NodeTree::Group>& NodeTree::GetGroups () const
{
	return groups;
}

const NodeTree EmptyNodeTree;

NodeTreeView::NodeTreeView () :
	listHandle (NULL)
{

}

NodeTreeView::~NodeTreeView ()
{

}

bool NodeTreeView::Init (HWND parentHandle, int x, int y, int width, int height)
{
	listHandle = CreateWindowEx (
		0, WC_TREEVIEW, NULL, WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		x, y, width, height, parentHandle, NULL, NULL, NULL
	);
	if (DBGERROR (listHandle == NULL)) {
		return false;
	}
	return true;
}

void NodeTreeView::Resize (int x, int y, int width, int height)
{
	if (listHandle == NULL) {
		return;
	}
	MoveWindow (listHandle, x, y, width, height, TRUE);
}

bool NodeTreeView::HasGroup (const std::wstring& group) const
{
	return groups.find (group) != groups.end ();
}

void NodeTreeView::AddGroup (const std::wstring& group)
{
	auto found = groups.find (group);
	if (DBGERROR (found != groups.end ())) {
		return;
	}

	TVINSERTSTRUCT tvInsertStruct;
	ZeroMemory (&tvInsertStruct, sizeof (TVINSERTSTRUCT));
	tvInsertStruct.hParent = NULL;
	tvInsertStruct.hInsertAfter = NULL;
	tvInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsertStruct.item.pszText = (LPWSTR) group.c_str ();
	tvInsertStruct.item.cchTextMax = sizeof (tvInsertStruct.item.pszText) / sizeof (wchar_t);
	tvInsertStruct.item.lParam = (LPARAM) -1;

	HTREEITEM groupItem = (HTREEITEM) SendMessage (listHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
	groups.insert ({ group, groupItem });
}

void NodeTreeView::AddItem (const std::wstring& group, const std::wstring& text, LPARAM lParam)
{
	auto found = groups.find (group);
	if (DBGERROR (found == groups.end ())) {
		return;
	}

	TVINSERTSTRUCT tvInsertStruct;
	ZeroMemory (&tvInsertStruct, sizeof (TVINSERTSTRUCT));
	tvInsertStruct.hParent = found->second;
	tvInsertStruct.hInsertAfter = NULL;
	tvInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsertStruct.item.pszText = (LPWSTR) text.c_str ();
	tvInsertStruct.item.cchTextMax = sizeof (tvInsertStruct.item.pszText) / sizeof (wchar_t);
	tvInsertStruct.item.lParam = lParam;
	SendMessage (listHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
}

void NodeTreeView::ExpandAll ()
{
	for (const auto& it : groups) {
		TreeView_Expand (listHandle, it.second, TVM_EXPAND);
	}
}

HWND NodeTreeView::GetListHandle ()
{
	return listHandle;
}

}
