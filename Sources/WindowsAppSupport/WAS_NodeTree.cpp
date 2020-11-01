#include "WAS_NodeTree.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_GdiplusUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

NodeTreeView::NodeTreeView () :
	listHandle (NULL),
	imageList (NULL),
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
	listHandle = CreateWindowEx (
		0, WC_TREEVIEW, NULL, WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		x, y, width, height, parentHandle, NULL, NULL, NULL
	);
	if (DBGERROR (listHandle == NULL)) {
		return false;
	}
	return true;
}

bool NodeTreeView::InitImageList ()
{
	if (DBGERROR (imageList != NULL)) {
		return false;
	}
	imageList = ImageList_Create (18, 18, ILC_COLOR32, 1, 1);
	if (DBGERROR (imageList == NULL)) {
		return false;
	}
	TreeView_SetImageList (listHandle, imageList, TVSIL_NORMAL);
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

void NodeTreeView::AddGroup (const std::wstring& group, HBITMAP bitmap)
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

	if (imageList != NULL && bitmap != NULL) {
		int imageIndex = ImageList_GetImageCount (imageList);
		ImageList_Add (imageList, bitmap, NULL);
		tvInsertStruct.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvInsertStruct.item.iImage = imageIndex;
		tvInsertStruct.item.iSelectedImage = imageIndex;
	}

	HTREEITEM groupItem = (HTREEITEM) SendMessage (listHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
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
		int imageIndex = ImageList_GetImageCount (imageList);
		ImageList_Add (imageList, bitmap, NULL);
		tvInsertStruct.item.mask |= TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvInsertStruct.item.iImage = imageIndex;
		tvInsertStruct.item.iSelectedImage = imageIndex;
	}

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
