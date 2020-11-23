#include "WAS_Toolbar.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

Toolbar::Toolbar () :
	toolbarHandle (NULL),
	imageList (NULL),
	disabledImageList (NULL)
{

}

Toolbar::~Toolbar ()
{
	if (imageList != NULL) {
		ImageList_Destroy (imageList);
	}
	if (disabledImageList != NULL) {
		ImageList_Destroy (disabledImageList);
	}
}

void Toolbar::Init (HWND parentHandle)
{
	toolbarHandle = CreateWindowEx (
		0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_FLAT | CCS_NODIVIDER,
		0, 0, 0, 0, parentHandle, NULL, GetModuleHandle (NULL), NULL
	);
	if (DBGERROR (toolbarHandle == NULL)) {
		return;
	}

	imageList = ImageList_Create (18, 18, ILC_COLOR32, 2, 1);
	if (DBGERROR (imageList == NULL)) {
		return;
	}

	disabledImageList = ImageList_Create (18, 18, ILC_COLOR32, 2, 1);
	if (DBGERROR (disabledImageList == NULL)) {
		return;
	}

	SendMessage (toolbarHandle, TB_SETPADDING, 0, MAKELPARAM (10, 10));
	SendMessage (toolbarHandle, TB_SETIMAGELIST, (WPARAM) 0, (LPARAM) imageList);
	SendMessage (toolbarHandle, TB_SETDISABLEDIMAGELIST, (WPARAM) 0, (LPARAM) disabledImageList);
	SendMessage (toolbarHandle, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof (TBBUTTON), 0);

	ShowWindow (toolbarHandle, TRUE);
}

void Toolbar::AddItem (HBITMAP bitmap, HBITMAP disabledBitmap, UINT commandId)
{
	int imageIndex = ImageList_Add (imageList, bitmap, NULL);
	int disabledImageIndex = ImageList_Add (disabledImageList, disabledBitmap, NULL);
	if (DBGERROR (imageIndex != disabledImageIndex)) {
		return;
	}

	TBBUTTON button;
	ZeroMemory (&button, sizeof (TBBUTTON));
	button.iBitmap = imageIndex;
	button.idCommand = (int) commandId;
	button.fsState = TBSTATE_ENABLED;
	button.fsStyle = BTNS_AUTOSIZE;

	SendMessage (toolbarHandle, TB_ADDBUTTONS, (WPARAM) 1, (LPARAM) &button);
	AutoSize ();
}

void Toolbar::AddSeparator ()
{
	TBBUTTON button;
	ZeroMemory (&button, sizeof (TBBUTTON));
	button.fsStyle = TBSTYLE_SEP;

	SendMessage (toolbarHandle, TB_ADDBUTTONS, (WPARAM) 1, (LPARAM) &button);
	AutoSize ();
}

void Toolbar::EnableItem (UINT id, bool enabled)
{
	SendMessage (toolbarHandle, TB_ENABLEBUTTON, (WPARAM) id, (LPARAM) enabled);
}

int Toolbar::GetHeight ()
{
	RECT rect;
	GetClientRect (toolbarHandle, &rect);
	return rect.bottom - rect.top;
}

void Toolbar::AutoSize ()
{
	SendMessage (toolbarHandle, TB_AUTOSIZE, 0, 0);
}

}
