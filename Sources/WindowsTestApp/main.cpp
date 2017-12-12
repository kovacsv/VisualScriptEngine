#include "Application.hpp"
#include "CustomControl.hpp"
#include "BitmapContextGdi.hpp"
#include "BitmapContextGdiplus.hpp"
#include "WindowsAppUtilities.hpp"
#include "Debug.hpp"
#include "NodeEditorControl.hpp"
#include "DrawingControl.hpp"

#include <CommCtrl.h>
#include <iostream>
#include <fstream>

static int WindowWidth = 900;
static int WindowHeight = 600;

class ApplicationState
{
public:
	ApplicationState () :
		currentFileName ()
	{

	}

	void ClearCurrentFileName ()
	{
		currentFileName.clear ();
	}

	void SetCurrentFileName (const std::wstring& newCurrentFileName)
	{
		currentFileName = newCurrentFileName;
	}

	bool HasCurrentFileName () const
	{
		return !currentFileName.empty ();
	}

	const std::wstring& GetCurrentFileName () const
	{
		return currentFileName;
	}

private:
	std::wstring currentFileName;
};

class NodeEngineTestApplication : public Application
{
public:
	NodeEngineTestApplication (const std::shared_ptr<ResultImageEvaluationData>& evaluationData) :
		windowHandle (NULL),
		statusBarHandle (NULL),
		applicationState (),
		drawingControl (evaluationData->GetResultImage ()),
		nodeEditorControl (evaluationData)
	{

	}

	virtual void OnCreate (HWND hwnd) override
	{
		windowHandle = hwnd;
		nodeEditorControl.Init (hwnd, 0, 0, 0, 0);
		drawingControl.Init (hwnd, 0, 0, 0, 0);
		InitFileMenu (hwnd);
		InitStatusBar (hwnd);
	}

	virtual void OnResize (HWND hwnd, int newWidth, int newHeight) override
	{
		SendMessage (statusBarHandle, WM_SIZE, 0, 0);
		UpdateStatusBar ();
		RECT statusBarRect = {0, 0, 0, 0};
		GetClientRect (statusBarHandle, &statusBarRect);
		int clientHeight = newHeight - (statusBarRect.bottom - statusBarRect.top);
		nodeEditorControl.MoveResize (0, 0, newWidth, clientHeight / 2);
		drawingControl.MoveResize (0, clientHeight / 2, newWidth, clientHeight / 2);
	}

	virtual void OnIdle (HWND hwnd) override
	{
		drawingControl.Invalidate ();
	}

	virtual void OnMenuCommand (HWND hwnd, int commandId) override
	{
		if (commandId == FileMenuCommand::File_New) {
			drawingControl.Clear ();
			nodeEditorControl.New ();
			applicationState.ClearCurrentFileName ();
		} else if (commandId == FileMenuCommand::File_Open || commandId == FileMenuCommand::File_Save || commandId == FileMenuCommand::File_SaveAs) {
			OPENFILENAME openFileName;
			ZeroMemory (&openFileName, sizeof(openFileName));
			wchar_t fileName[MAX_PATH] = L"";
			openFileName.lStructSize = sizeof (openFileName); 
			openFileName.hwndOwner = windowHandle;
			openFileName.lpstrFile = (LPWSTR) fileName;
			openFileName.nMaxFile = MAX_PATH;
			openFileName.lpstrFilter = (LPCWSTR) L"Node Engine Files (*.ne)\0*.ne\0";
			openFileName.lpstrDefExt = (LPCWSTR) L"txt";
			if (commandId == FileMenuCommand::File_Open) {
				if (GetOpenFileName (&openFileName)) {
					drawingControl.Clear ();
					if (nodeEditorControl.Open (fileName)) {
						applicationState.SetCurrentFileName (fileName);
					}
				}
			} else if (commandId == FileMenuCommand::File_Save) {
				if (applicationState.HasCurrentFileName ()) {
					nodeEditorControl.Save (applicationState.GetCurrentFileName ());
				} else if (GetSaveFileName (&openFileName)) {
					nodeEditorControl.Save (fileName);
					applicationState.SetCurrentFileName (fileName);
				}
			} else if (commandId == FileMenuCommand::File_SaveAs) {
				if (GetSaveFileName (&openFileName)) {
					nodeEditorControl.Save (fileName);
					applicationState.SetCurrentFileName (fileName);
				}
			}
		} else if (commandId == FileMenuCommand::File_Quit) {
			Close ();
		}
		UpdateStatusBar ();
	}

private:
	enum FileMenuCommand
	{
		File_New		= 1000,
		File_Open		= 1001,
		File_Save		= 1002,
		File_SaveAs		= 1003,
		File_Quit		= 1004
	};

	void InitFileMenu (HWND hwnd)
	{
		HMENU hMenubar;
		HMENU hMenu;

		hMenubar = CreateMenu ();
		hMenu = CreateMenu ();

		AppendMenuW (hMenu, MF_STRING, FileMenuCommand::File_New, L"&New");
		AppendMenuW (hMenu, MF_STRING, FileMenuCommand::File_Open, L"&Open");
		AppendMenuW (hMenu, MF_STRING, FileMenuCommand::File_Save, L"&Save");
		AppendMenuW (hMenu, MF_STRING, FileMenuCommand::File_SaveAs, L"&Save As");
		AppendMenuW (hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenuW (hMenu, MF_STRING, FileMenuCommand::File_Quit, L"&Quit");

		AppendMenuW (hMenubar, MF_POPUP, (UINT_PTR) hMenu, L"&File");
		SetMenu (hwnd, hMenubar);
	}

	void InitStatusBar (HWND hwnd)
	{
		statusBarHandle = CreateWindowEx (0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hwnd, NULL, NULL, NULL);
		ShowWindow (statusBarHandle, SW_SHOW);
		UpdateStatusBar ();
	}

	void UpdateStatusBar ()
	{
		std::wstring currentFileText = L"No File";
		if (applicationState.HasCurrentFileName ()) {
			currentFileText = applicationState.GetCurrentFileName ();
		}
		int statusBarColumnWidths[] = {300, -1};
		SendMessage (statusBarHandle, SB_SETPARTS, 2, (LPARAM) statusBarColumnWidths);
		SendMessage (statusBarHandle, SB_SETTEXT, 0, (LPARAM) currentFileText.c_str ());
	}

	HWND							windowHandle;
	HWND							statusBarHandle;
	ApplicationState				applicationState;

	DrawingControl					drawingControl;
	NodeEditorControl				nodeEditorControl;
};

int wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	EnableLeakDetection ();

	std::shared_ptr<ResultImage> resultImage (new ResultImage ());
	std::shared_ptr<ResultImageEvaluationData> evaluationData (new ResultImageEvaluationData (resultImage));

	NodeEngineTestApplication app (evaluationData);
	app.Open (L"Node Engine Test App", WindowWidth, WindowHeight);

	return 0;
}
