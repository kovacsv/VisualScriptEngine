#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_HwndEventHandler.hpp"
#include "WAS_ClipboardHandler.hpp"
#include "WAS_NodeEditorNodeTreeHwndControl.hpp"
#include "WAS_ParameterDialog.hpp"

#include "BI_BuiltInNodes.hpp"
#include "NUIE_Localization.hpp"

#include <windows.h>
#include <windowsx.h>

#include "WAS_Direct2DContext.hpp"

#pragma comment (lib, "windowscodecs.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

class MyResourceImageLoader : public WAS::Direct2DImageLoaderFromResource
{
	virtual HRSRC GetImageResHandle (const NUIE::IconId& iconId) override
	{
		HRSRC resHandle = FindResource (NULL, MAKEINTRESOURCE (iconId.GetId ()), L"IMAGE");
		return resHandle;
	}
};

static void AddNodeTreeItem (WAS::NodeTree& nodeTree, size_t groupIndex, const std::wstring& name, int iconIndex, const WAS::CreatorFunction& creator)
{
	nodeTree.AddItem (groupIndex, name, [=] (const NUIE::Point& position) {
		NUIE::UINodePtr node = creator (position);
		BI::BasicUINodePtr basicNode = std::dynamic_pointer_cast<BI::BasicUINode> (node);
		if (basicNode != nullptr) {
			basicNode->SetIconId (NUIE::IconId (iconIndex));
		}
		return node;
	});
}

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::BasicStringConverter (WAS::GetStringSettingsFromSystem ())),
		skinParams (NUIE::GetDefaultSkinParams ()),
		eventHandler (),
		clipboardHandler (),
		evaluationEnv (nullptr),
		nodeEditorControl (NUIE::NativeDrawingContextPtr (new WAS::Direct2DContext (WAS::Direct2DImageLoaderPtr (new MyResourceImageLoader ()))))
	{
	
	}

	void Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle)
	{
		WAS::NodeTree nodeTree;

		size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
		AddNodeTreeItem (nodeTree, inputNodes, L"Integer", 101, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 5));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Number", 102, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 5.0));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Integer Increment", 101, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Number Increment", 102, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position));
		});
		size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Addition", 103, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Subtraction", 104, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Multiplication", 105, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Division", 106, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position));
		});
		size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
		AddNodeTreeItem (nodeTree, otherNodes, L"Viewer", 107, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ViewerNode (NE::LocString (L"Viewer"), position));
		});
		AddNodeTreeItem (nodeTree, otherNodes, L"Multiline Viewer", 107, [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Multiline Viewer"), position, 5));
		});

		RECT clientRect;
		GetClientRect (parentHandle, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		nodeEditorControl.Init (nodeEditorPtr, parentHandle, 0, 0, width, height);
		nodeEditorControl.FillNodeTree (nodeTree);
		eventHandler.Init (&nodeEditorControl);
	}

	void OnResize (int x, int y, int width, int height)
	{
		nodeEditorControl.Resize (x, y, width, height);
	}

	virtual const NE::StringConverter& GetStringConverter () override
	{
		return stringConverter;
	}

	virtual const NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return nodeEditorControl.GetDrawingContext ();
	}

	virtual double GetWindowScale () override
	{
		return 1.0;
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
	}

	virtual void OnEvaluationBegin () override
	{

	}

	virtual void OnEvaluationEnd () override
	{

	}

	virtual void OnValuesRecalculated () override
	{
		
	}

	virtual void OnRedrawRequested () override
	{
		nodeEditorControl.Invalidate ();
	}

	virtual NUIE::EventHandler& GetEventHandler () override
	{
		return eventHandler;
	}

	virtual NUIE::ClipboardHandler& GetClipboardHandler () override
	{
		return clipboardHandler;
	}

	virtual double GetMouseMoveMinOffset () override
	{
		return 2.0;
	}

private:
	NE::BasicStringConverter			stringConverter;
	NUIE::BasicSkinParams				skinParams;
	WAS::HwndEventHandler				eventHandler;
	WAS::ClipboardHandler				clipboardHandler;
	NE::EvaluationEnv					evaluationEnv;
	WAS::NodeEditorNodeTreeHwndControl	nodeEditorControl;
};

static MyNodeUIEnvironment uiEnvironment;
static NUIE::NodeEditor nodeEditor (uiEnvironment);

LRESULT CALLBACK ApplicationWindowProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		case WM_CREATE:
			{
				uiEnvironment.Init (&nodeEditor, hwnd);
			}
			break;
		case WM_CLOSE:
			DestroyWindow (hwnd);
			break;
		case WM_SIZE:
			{
				int newWidth = LOWORD (lParam);
				int newHeight = HIWORD (lParam);
				uiEnvironment.OnResize (0, 0, newWidth, newHeight);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage (0);
			DestroyWindow (hwnd);
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

int wWinMain (HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
#ifdef LOCALIZATION_TEST
	std::wstring poContent = LR"str(
		msgid "Integer"
		msgstr "LocInteger"
		msgid "Group"
		msgstr "LocGroup"
		msgid "Input"
		msgstr "LocInput"
		msgid "Output"
		msgstr "LocOutput"
		msgid "Add To Group \"%ls\""
		msgstr "Loc Add To Group \"%ls\""
		msgid "%ls (%ls)"
		msgstr "Loc %ls (%ls)"
		msgid "Blue"
		msgstr "Loc Blue"
	)str";

	NUIE::PoDictionarySource poDictionarySource (poContent);
	FillDictionary (poDictionarySource);
#endif

	WNDCLASSEX windowClass;
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = 0;
	windowClass.lpfnWndProc = ApplicationWindowProc;
	windowClass.style = CS_DBLCLKS;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon (NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = L"WindowsEmbeddingDemo";
	windowClass.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

	if (!RegisterClassEx (&windowClass)) {
		return false;
	}

	RECT requiredRect = { 0, 0, 900, 500 };
	AdjustWindowRect (&requiredRect, WS_OVERLAPPEDWINDOW, false);

	HWND windowHandle = CreateWindowEx (
		WS_EX_WINDOWEDGE | WS_CLIPCHILDREN, windowClass.lpszClassName, L"VisualScriptEngine Embedding Demo", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, requiredRect.right - requiredRect.left, requiredRect.bottom - requiredRect.top, NULL, NULL, NULL, nullptr
	);

	if (windowHandle == NULL) {
		return 1;
	}

	ShowWindow (windowHandle, SW_SHOW);
	UpdateWindow (windowHandle);

	MSG msg;
	while (GetMessage (&msg, NULL, 0, 0)) {
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}

	return 0;
}
