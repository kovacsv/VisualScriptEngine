#include "WAS_NodeEditorNodeListHwndControl.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "NE_Debug.hpp"

#include "BI_InputUINodes.hpp" // TODO: remove

namespace WAS
{

static const short NodeListWidth = 200;

static LRESULT CALLBACK NodeEditorNodeListStaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	}

	NodeEditorNodeListHwndControl* control = (NodeEditorNodeListHwndControl*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (control == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_NOTIFY:
			{
				LPNMHDR header = (LPNMHDR) lParam;
				switch (header->code) {
					case TVN_BEGINDRAG:
						control->TreeViewBeginDrag ((LPNMTREEVIEW) lParam);
						break;
					case TVN_SELCHANGED:
						control->TreeViewSelectionChanged ((LPNMTREEVIEW) lParam);
						break;
					case NM_DBLCLK:
						control->TreeViewDoubleClick (header);
						break;
				}
			}
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

NodeTreeView::NodeTreeView () :
	hwnd (NULL)
{

}

NodeTreeView::~NodeTreeView ()
{

}

bool NodeTreeView::Init (HWND parentHandle, int x, int y, int width, int height)
{
	hwnd = CreateWindowEx (
		0, WC_TREEVIEW, NULL, WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		x, y, width, height, parentHandle, NULL, NULL, NULL
	);
	if (DBGERROR (hwnd == NULL)) {
		return false;
	}
	return true;
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
	memset (&tvInsertStruct, 0, sizeof (TVINSERTSTRUCT));
	tvInsertStruct.hParent = NULL;
	tvInsertStruct.hInsertAfter = NULL;
	tvInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsertStruct.item.pszText = (LPWSTR) group.c_str ();
	tvInsertStruct.item.cchTextMax = sizeof (tvInsertStruct.item.pszText) / sizeof (wchar_t);
	tvInsertStruct.item.lParam = (LPARAM) -1;

	HTREEITEM groupItem = (HTREEITEM) SendMessage (hwnd, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
	groups.insert ({ group, groupItem });
}

void NodeTreeView::AddItem (const std::wstring& group, const std::wstring& text, LPARAM lParam)
{
	auto found = groups.find (group);
	if (DBGERROR (found == groups.end ())) {
		return;
	}

	TVINSERTSTRUCT tvInsertStruct;
	memset (&tvInsertStruct, 0, sizeof (TVINSERTSTRUCT));
	tvInsertStruct.hParent = found->second;
	tvInsertStruct.hInsertAfter = NULL;
	tvInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsertStruct.item.pszText = (LPWSTR) text.c_str ();
	tvInsertStruct.item.cchTextMax = sizeof (tvInsertStruct.item.pszText) / sizeof (wchar_t);
	tvInsertStruct.item.lParam = lParam;
	SendMessage (hwnd, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
}

void NodeTreeView::ExpandAll ()
{
	for (const auto& it : groups) {
		TreeView_Expand (hwnd, it.second, TVM_EXPAND);
	}
}

HWND NodeTreeView::GetWindowHandle ()
{
	return hwnd;
}

NodeEditorNodeListHwndControl::NodeEditorNodeListHwndControl () :
	nodeTreeView (),
	nodeEditorControl (),
	mainHandle (NULL),
	selectedNode (-1),
	nextNodeId (0)
{

}

NodeEditorNodeListHwndControl::~NodeEditorNodeListHwndControl ()
{

}

bool NodeEditorNodeListHwndControl::Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height)
{
	mainHandle = CreateCustomControl (parentHandle, NodeEditorNodeListStaticWindowProc, L"NodeEditorNodeListHwndControl", this);
	if (DBGERROR (mainHandle == NULL)) {
		return false;
	}

	MoveWindow (mainHandle, x, y, width, height, TRUE);
	nodeTreeView.Init (mainHandle, 0, 0, NodeListWidth, height);
	nodeEditorControl.Init (nodeEditorPtr, mainHandle, NodeListWidth, 0, width - NodeListWidth, height);

	return true;
}

void NodeEditorNodeListHwndControl::Resize (int x, int y, int width, int height)
{
	MoveWindow (mainHandle, x, y, width, height, TRUE);
	nodeEditorControl.Resize (x + NodeListWidth, y, width - NodeListWidth, height);
}

HWND NodeEditorNodeListHwndControl::GetEditorHandle ()
{
	return nodeEditorControl.GetWindowHandle ();
}

NUIE::DrawingContext& NodeEditorNodeListHwndControl::GetEditorContext ()
{
	return nodeEditorControl.GetDrawingContext ();
}

void NodeEditorNodeListHwndControl::InvalidateEditor ()
{
	nodeEditorControl.Invalidate ();
}

void NodeEditorNodeListHwndControl::RegisterNode (const std::wstring& group, const std::wstring& text, const CreatorFunction& creator)
{
	if (!nodeTreeView.HasGroup (group)) {
		nodeTreeView.AddGroup (group);
	}

	nodeTreeView.AddItem (group, text, nextNodeId);
	nodeIdToCreator.insert ({ nextNodeId, creator });
	nextNodeId++;

	nodeTreeView.ExpandAll ();
}

void NodeEditorNodeListHwndControl::TreeViewSelectionChanged (LPNMTREEVIEW lpnmtv)
{
	if (lpnmtv->hdr.hwndFrom != nodeTreeView.GetWindowHandle ()) {
		return;
	}

	selectedNode = lpnmtv->itemNew.lParam;
}

void NodeEditorNodeListHwndControl::TreeViewDoubleClick (LPNMHDR lpnmhdr)
{
	if (lpnmhdr->hwndFrom != nodeTreeView.GetWindowHandle ()) {
		return;
	}

	if (selectedNode == -1) {
		return;
	}

	auto found = nodeIdToCreator.find (selectedNode);
	if (DBGERROR (found == nodeIdToCreator.end ())) {
		return;
	}

	RECT clientRect;
	GetClientRect (nodeEditorControl.GetWindowHandle (), &clientRect);

	NUIE::NodeEditor* nodeEditor = nodeEditorControl.GetNodeEditor ();
	NUIE::Rect viewRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (0.0, 0.0), NUIE::Size (clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
	NUIE::Point modelPosition = nodeEditor->ViewToModel (viewRect.GetCenter ());
	nodeEditor->AddNode (found->second (modelPosition));
}

void NodeEditorNodeListHwndControl::TreeViewBeginDrag (LPNMTREEVIEW lpnmtv)
{
	if (lpnmtv->hdr.hwndFrom != nodeTreeView.GetWindowHandle ()) {
		return;
	}

	if (lpnmtv->itemNew.lParam == (LPARAM) -1) {
		return;
	}

	HTREEITEM item = lpnmtv->itemOld.hItem;
	TreeView_SelectItem (lpnmtv->hdr.hwndFrom, item);
	(void) item;
}

}
