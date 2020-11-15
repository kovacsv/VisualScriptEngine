#include "WAS_NodeEditorNodeTreeHwndControl.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

static const LPARAM InvalidNode = -1;

static LRESULT CALLBACK NodeEditorNodeListStaticWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		LPCREATESTRUCT createStruct = LPCREATESTRUCT (lParam);
		SetWindowLongPtr (hwnd, GWLP_USERDATA, (LONG_PTR) createStruct->lpCreateParams);
	} else if (msg == WM_DESTROY) {
		SetWindowLongPtr (hwnd, GWLP_USERDATA, NULL);
	}

	NodeEditorNodeTreeHwndControl* control = (NodeEditorNodeTreeHwndControl*) GetWindowLongPtr (hwnd, GWLP_USERDATA);
	if (control == nullptr) {
		return DefWindowProc (hwnd, msg, wParam, lParam);
	}

	switch (msg) {
		case WM_LBUTTONUP:
			{
				ReleaseCapture ();
				int x = GET_X_LPARAM (lParam);
				int y = GET_Y_LPARAM (lParam);
				control->TreeViewEndDrag (x, y);
			}
			break;
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
				case TVN_ITEMEXPANDED:
					control->TreeViewItemExpanded ((LPNMTREEVIEW) lParam);
				case NM_DBLCLK:
					control->TreeViewDoubleClick (header);
					break;
				}
			}
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

static HBITMAP LoadTreeImage (NodeEditorNodeTreeHwndControl::ImageLoader* imageLoader, const NUIE::IconId& iconId, COLORREF bgColor)
{
	if (imageLoader == nullptr) {
		return NULL;
	}
	if (iconId == NUIE::InvalidIconId) {
		return NULL;
	}
	return imageLoader->LoadImage (iconId, bgColor);
}

NodeEditorNodeTreeHwndControl::Settings::Settings () :
	treeWidth (200),
	treeRightMargin (2)
{

}

NodeEditorNodeTreeHwndControl::ImageLoader::ImageLoader ()
{

}

NodeEditorNodeTreeHwndControl::ImageLoader::~ImageLoader ()
{

}

NodeEditorNodeTreeHwndControl::NodeEditorNodeTreeHwndControl (const Settings& settings) :
	NUIE::NativeNodeEditorControl (),
	settings (settings),
	nodeTreeView (),
	nodeEditorControl (),
	mainControl (),
	selectedNode (InvalidNode),
	draggedNode (InvalidNode)
{

}

NodeEditorNodeTreeHwndControl::NodeEditorNodeTreeHwndControl (const Settings& settings, const NUIE::NativeDrawingContextPtr& nativeContext) :
	NUIE::NativeNodeEditorControl (),
	settings (settings),
	nodeTreeView (),
	nodeEditorControl (nativeContext),
	mainControl (),
	selectedNode (InvalidNode),
	draggedNode (InvalidNode)
{

}

NodeEditorNodeTreeHwndControl::~NodeEditorNodeTreeHwndControl ()
{

}

bool NodeEditorNodeTreeHwndControl::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	bool controlInited = mainControl.Init ((HWND) nativeParentHandle, NodeEditorNodeListStaticWindowProc, L"NodeEditorNodeListHwndControl", this);
	if (DBGERROR (!controlInited)) {
		return false;
	}

	HWND mainHandle = mainControl.GetWindowHandle ();
	if (DBGERROR (mainHandle == NULL)) {
		return false;
	}

	MoveWindow (mainHandle, x, y, width, height, TRUE);
	nodeTreeView.Init (mainHandle, 0, 0, settings.treeWidth, height);
	nodeEditorControl.Init (nodeEditorPtr, mainHandle, settings.treeWidth + settings.treeRightMargin, 0, width - settings.treeWidth - settings.treeRightMargin, height);

	return true;
}

void* NodeEditorNodeTreeHwndControl::GetEditorNativeHandle () const
{
	return nodeEditorControl.GetEditorNativeHandle ();
}

bool NodeEditorNodeTreeHwndControl::IsMouseOverEditorWindow () const
{
	return nodeEditorControl.IsMouseOverEditorWindow ();
}

void NodeEditorNodeTreeHwndControl::Resize (int x, int y, int width, int height)
{
	HWND mainHandle = mainControl.GetWindowHandle ();
	if (mainHandle == NULL) {
		return;
	}
	MoveWindow (mainHandle, x, y, width, height, TRUE);
	nodeTreeView.Resize (0, 0, settings.treeWidth, height);
	nodeEditorControl.Resize (settings.treeWidth + settings.treeRightMargin, 0, width - settings.treeWidth - settings.treeRightMargin, height);
}

void NodeEditorNodeTreeHwndControl::Invalidate ()
{
	nodeEditorControl.Invalidate ();
}

void NodeEditorNodeTreeHwndControl::Draw ()
{
	nodeEditorControl.Draw ();
}

NUIE::DrawingContext& NodeEditorNodeTreeHwndControl::GetDrawingContext ()
{
	return nodeEditorControl.GetDrawingContext ();
}

void NodeEditorNodeTreeHwndControl::TreeViewSelectionChanged (LPNMTREEVIEW lpnmtv)
{
	if (lpnmtv->hdr.hwndFrom != nodeTreeView.GetTreeHandle ()) {
		return;
	}

	selectedNode = lpnmtv->itemNew.lParam;
}

void NodeEditorNodeTreeHwndControl::TreeViewItemExpanded (LPNMTREEVIEW lpnmtv)
{
	if (lpnmtv->hdr.hwndFrom != nodeTreeView.GetTreeHandle ()) {
		return;
	}

	nodeTreeView.GroupExpanded (lpnmtv->itemNew);
}

void NodeEditorNodeTreeHwndControl::FillNodeTree (const NUIE::NodeTree& nodeTree, ImageLoader* imageLoader)
{
	COLORREF bgColor = (COLORREF) TreeView_GetBkColor (nodeTreeView.GetTreeHandle ());
	if (imageLoader != nullptr) {
		nodeTreeView.InitImageList (
			imageLoader->LoadGroupClosedImage (bgColor),
			imageLoader->LoadGroupOpenedImage (bgColor)
		);
	}
	LPARAM nextNodeId = 0;
	for (const NUIE::NodeTree::Group& group : nodeTree.GetGroups ()) {
		nodeTreeView.AddGroup (group.GetName ());
		for (const NUIE::NodeTree::Item& item : group.GetItems ()) {
			HBITMAP itemIcon = LoadTreeImage (imageLoader, item.GetIconId (), bgColor);
			nodeTreeView.AddItem (group.GetName (), item.GetName (), itemIcon, nextNodeId);
			nodeIdToCreator.insert ({ nextNodeId, item.GetCreator () });
			nextNodeId++;
		}
	}
	nodeTreeView.ExpandAll ();
}

void NodeEditorNodeTreeHwndControl::TreeViewDoubleClick (LPNMHDR lpnmhdr)
{
	if (lpnmhdr->hwndFrom != nodeTreeView.GetTreeHandle ()) {
		return;
	}

	if (selectedNode == InvalidNode) {
		return;
	}

	RECT clientRect;
	GetClientRect ((HWND) nodeEditorControl.GetEditorNativeHandle (), &clientRect);
	NUIE::Rect viewRect = NUIE::Rect::FromPositionAndSize (NUIE::Point (0.0, 0.0), NUIE::Size (clientRect.right - clientRect.left, clientRect.bottom - clientRect.top));
	NUIE::Point center = viewRect.GetCenter ();
	CreateNode (selectedNode, (int) center.GetX (), (int) center.GetY ());
}

void NodeEditorNodeTreeHwndControl::TreeViewBeginDrag (LPNMTREEVIEW lpnmtv)
{
	if (lpnmtv->hdr.hwndFrom != nodeTreeView.GetTreeHandle ()) {
		return;
	}

	if (lpnmtv->itemNew.lParam == InvalidNode) {
		return;
	}

	HWND mainHandle = mainControl.GetWindowHandle ();

	HTREEITEM item = lpnmtv->itemNew.hItem;
	TreeView_SelectItem (lpnmtv->hdr.hwndFrom, item);
	SetCapture (mainHandle);
	SetCursor (LoadCursor (NULL, IDC_CROSS));

	draggedNode = lpnmtv->itemNew.lParam;
}

void NodeEditorNodeTreeHwndControl::TreeViewEndDrag (int x, int y)
{
	if (draggedNode == InvalidNode) {
		return;
	}

	HWND mainHandle = mainControl.GetWindowHandle ();
	HWND editorHandle = (HWND) nodeEditorControl.GetEditorNativeHandle ();

	RECT editorRect;
	GetClientRect (editorHandle, &editorRect);
	MapWindowPoints (editorHandle, mainHandle, (LPPOINT) &editorRect, 2);

	if (x < editorRect.left || x > editorRect.right || y < editorRect.top || y > editorRect.bottom) {
		draggedNode = InvalidNode;
		return;
	}

	CreateNode (draggedNode, x - editorRect.left, y - editorRect.top);
	draggedNode = InvalidNode;
}

void NodeEditorNodeTreeHwndControl::CreateNode (LPARAM nodeId, int screenX, int screenY)
{
	auto found = nodeIdToCreator.find (nodeId);
	if (DBGERROR (found == nodeIdToCreator.end ())) {
		return;
	}

	NUIE::NodeEditor* nodeEditor = nodeEditorControl.GetNodeEditor ();
	NUIE::Point modelPosition = nodeEditor->ViewToModel (NUIE::Point (screenX, screenY));
	nodeEditor->AddNode (found->second (modelPosition));
}

}
