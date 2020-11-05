#include "WAS_NodeTree.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_GdiplusUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

NodeTreeView::NodeTreeView () :
	treeHandle (NULL),
	imageList (NULL),
	closedBitmapIndex (-1),
	openedBitmapIndex (-1),
	groups ()
{

}

NodeTreeView::~NodeTreeView ()
{
	if (imageList != NULL) {
		ImageList_Destroy (imageList);
	}
}

bool NodeTreeView::Init (HWND parentHandle, int x, int y, int width, int height)
{
	treeHandle = CreateWindowEx (
		0, WC_TREEVIEW, NULL, WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		x, y, width, height, parentHandle, NULL, NULL, NULL
	);
	if (DBGERROR (treeHandle == NULL)) {
		return false;
	}
	TreeView_SetItemHeight (treeHandle, 20);
	return true;
}

bool NodeTreeView::InitImageList (HBITMAP groupClosedBitmap, HBITMAP groupOpenedBitmap)
{
	if (DBGERROR (imageList != NULL)) {
		return false;
	}
	imageList = ImageList_Create (18, 18, ILC_COLOR32, 2, 1);
	if (DBGERROR (imageList == NULL)) {
		return false;
	}
	closedBitmapIndex = ImageList_Add (imageList, groupClosedBitmap, NULL);
	openedBitmapIndex = ImageList_Add (imageList, groupOpenedBitmap, NULL);
	TreeView_SetImageList (treeHandle, imageList, TVSIL_NORMAL);
	return true;
}

void NodeTreeView::Resize (int x, int y, int width, int height)
{
	if (treeHandle == NULL) {
		return;
	}
	MoveWindow (treeHandle, x, y, width, height, TRUE);
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

	if (imageList != NULL) {
		tvInsertStruct.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvInsertStruct.item.iImage = closedBitmapIndex;
		tvInsertStruct.item.iSelectedImage = closedBitmapIndex;
	}

	HTREEITEM groupItem = (HTREEITEM) SendMessage (treeHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
	groups.insert ({ group, groupItem });
}

void NodeTreeView::AddItem (const std::wstring& group, const std::wstring& text, HBITMAP bitmap, LPARAM lParam)
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

	if (imageList != NULL && bitmap != NULL) {
		int imageIndex = ImageList_Add (imageList, bitmap, NULL);
		tvInsertStruct.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvInsertStruct.item.iImage = imageIndex;
		tvInsertStruct.item.iSelectedImage = imageIndex;
	}

	SendMessage (treeHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
}

void NodeTreeView::GroupExpanded (const TVITEMW& group)
{
	TVITEM newGroup = group;
	if (group.state & TVIS_EXPANDED) {
		newGroup.iImage = openedBitmapIndex;
		newGroup.iSelectedImage = openedBitmapIndex;
	} else {
		newGroup.iImage = closedBitmapIndex;
		newGroup.iSelectedImage = closedBitmapIndex;
	}
	TreeView_SetItem (treeHandle, &newGroup);
}

void NodeTreeView::ExpandAll ()
{
	for (const auto& it : groups) {
		TreeView_Expand (treeHandle, it.second, TVM_EXPAND);
	}
}

HWND NodeTreeView::GetTreeHandle ()
{
	return treeHandle;
}

}
