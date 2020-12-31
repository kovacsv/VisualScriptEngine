#include "Application.hpp"
#include "NUIE_FeatureToggle.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_GdiplusUtils.hpp"

#include "ResourceIds.hpp"
#include "CommandIds.hpp"

static bool ContinueWithNoSave (HWND hwnd, LPCWSTR caption)
{
	std::wstring text = L"You have made some changes that are not saved.\nWould you like to continue?";
	int result = MessageBox (hwnd, text.c_str (), caption, MB_ICONWARNING | MB_YESNO);
	return (result == IDYES);
}

Application::Application () :
	uiEnvironment (),
	nodeEditor (uiEnvironment),
	fileMenu (),
	toolbar ()
{

}

void Application::Init (HWND hwnd)
{
	InitFileMenu (hwnd);
	InitToolbar (hwnd);
	NUIE::EnableFeature (NUIE::Feature::ConnectionDisplay, true);
	uiEnvironment.Init (&nodeEditor, &fileMenu, &toolbar, hwnd);
}

void Application::New (HWND hwnd)
{
	if (nodeEditor.NeedToSave ()) {
		bool result = ContinueWithNoSave (hwnd, L"New File");
		if (!result) {
			return;
		}
	}
	nodeEditor.New ();
}

void Application::Open (HWND hwnd)
{
	if (nodeEditor.NeedToSave ()) {
		bool result = ContinueWithNoSave (hwnd, L"Open File");
		if (!result) {
			return;
		}
	}
	std::wstring fileName;
	WAS::FileFilter fileFilter = WAS::GetDefaultFileFilter ();
	if (WAS::OpenFileDialog (hwnd, fileFilter, fileName)) {
		nodeEditor.Open (fileName);
		nodeEditor.AlignToWindow ();
	}
}

void Application::Save (HWND hwnd)
{
	std::wstring fileName;
	WAS::FileFilter fileFilter = WAS::GetDefaultFileFilter ();
	if (WAS::SaveFileDialog (hwnd, fileFilter, fileName)) {
		nodeEditor.Save (fileName);
	}
}

bool Application::Close (HWND hwnd)
{
	if (nodeEditor.NeedToSave ()) {
		return ContinueWithNoSave (hwnd, L"Quit");
	}
	return true;
}

void Application::ExecuteCommand (NUIE::CommandCode command)
{
	nodeEditor.ExecuteCommand (command);
}

void Application::OnResize (int x, int y, int width, int height)
{
	uiEnvironment.OnResize (x, y, width, height);
	toolbar.AutoSize ();
}

void Application::OnClipboardChanged ()
{
	uiEnvironment.OnClipboardChanged ();
}

void Application::InitFileMenu (HWND hwnd)
{
	HMENU file = fileMenu.AddPopupMenu (L"File");
	fileMenu.AddPopupMenuItem (file, FILE_NEW, L"New");
	fileMenu.AddPopupMenuItem (file, FILE_OPEN, L"Open");
	fileMenu.AddPopupMenuItem (file, FILE_SAVE, L"Save");
	fileMenu.AddPopupMenuSeparator (file);
	fileMenu.AddPopupMenuItem (file, FILE_QUIT, L"Quit");

	HMENU edit = fileMenu.AddPopupMenu (L"Edit");
	fileMenu.AddPopupMenuItem (edit, EDIT_UNDO, L"Undo");
	fileMenu.AddPopupMenuItem (edit, EDIT_REDO, L"Redo");
	fileMenu.AddPopupMenuSeparator (edit);
	fileMenu.AddPopupMenuItem (edit, EDIT_SETTINGS, L"Settings");
	fileMenu.AddPopupMenuItem (edit, EDIT_COPY, L"Copy");
	fileMenu.AddPopupMenuItem (edit, EDIT_PASTE, L"Paste");
	fileMenu.AddPopupMenuItem (edit, EDIT_DELETE, L"Delete");
	fileMenu.AddPopupMenuSeparator (edit);
	fileMenu.AddPopupMenuItem (edit, EDIT_GROUP, L"Group");
	fileMenu.AddPopupMenuItem (edit, EDIT_UNGROUP, L"Ungroup");

	SetMenu (hwnd, fileMenu.GetMenuBar ());
}

void Application::InitToolbar (HWND hwnd)
{
	toolbar.Init (hwnd);

	AddToolbarItem (TOOLBAR_ENABLED_NEW_ICON, FILE_NEW);
	AddToolbarItem (TOOLBAR_ENABLED_OPEN_ICON, FILE_OPEN);
	AddToolbarItem (TOOLBAR_ENABLED_SAVE_ICON, FILE_SAVE);
	toolbar.AddSeparator ();

	AddToolbarItem (TOOLBAR_ENABLED_UNDO_ICON, EDIT_UNDO);
	AddToolbarItem (TOOLBAR_ENABLED_REDO_ICON, EDIT_REDO);
	toolbar.AddSeparator ();

	AddToolbarItem (TOOLBAR_ENABLED_SETTINGS_ICON, EDIT_SETTINGS);
	AddToolbarItem (TOOLBAR_ENABLED_COPY_ICON, EDIT_COPY);
	AddToolbarItem (TOOLBAR_ENABLED_PASTE_ICON, EDIT_PASTE);
	AddToolbarItem (TOOLBAR_ENABLED_DELETE_ICON, EDIT_DELETE);
	toolbar.AddSeparator ();

	AddToolbarItem (TOOLBAR_ENABLED_GROUP_ICON, EDIT_GROUP);
	AddToolbarItem (TOOLBAR_ENABLED_UNGROUP_ICON, EDIT_UNGROUP);
}

void Application::AddToolbarItem (WORD imageResourceId, UINT commandId)
{
	HBITMAP bitmap = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (imageResourceId));
	HBITMAP disabledBitmap = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (imageResourceId + ENABLED_TO_DISABLED_ICON_OFFSET));
	toolbar.AddItem (bitmap, disabledBitmap, commandId);
}
