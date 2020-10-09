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
#include "WAS_Direct2DOffscreenContext.hpp"

#pragma comment (lib, "windowscodecs.lib")
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")

static const int ControlPadding = 5;

static const NUIE::BasicSkinParams& GetAppSkinParams ()
{
	static const NUIE::BasicSkinParams skinParams (
		/*backgroundColor*/ NUIE::Color (255, 255, 255),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodeHeaderTextFont*/ NUIE::Font (L"Arial", 16.0),
		/*nodeHeaderTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderErrorTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderBackgroundColor*/ NUIE::Color (41, 127, 255),
		/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (199, 80, 80),
		/*nodeContentTextFont*/ NUIE::Font (L"Arial", 14.0),
		/*nodeContentTextColor*/ NUIE::Color (0, 0, 0),
		/*nodeContentBackgroundColor*/ NUIE::Color (236, 236, 236),
		/*slotTextColor*/ NUIE::Color (0, 0, 0),
		/*slotTextBackgroundColor*/ NUIE::Color (246, 246, 246),
		/*needToDrawSlotCircles*/ true,
		/*slotCircleSize*/ NUIE::Size (8.0, 8.0),
		/*selectionBlendColor*/ NUIE::BlendColor (NUIE::Color (41, 127, 255), 0.25),
		/*disabledBlendColor*/ NUIE::BlendColor (NUIE::Color (0, 138, 184), 0.2),
		/*selectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 1.0),
		/*nodeSelectionRectPen*/ NUIE::Pen (NUIE::Color (41, 127, 255), 3.0),
		/*buttonBorderPen*/ NUIE::Pen (NUIE::Color (146, 152, 155), 1.0),
		/*buttonBackgroundColor*/ NUIE::Color (217, 217, 217),
		/*textPanelTextColor*/ NUIE::Color (0, 0, 0),
		/*textPanelBackgroundColor*/ NUIE::Color (236, 236, 236),
		/*groupNameFont*/ NUIE::Font (L"Arial", 16.0),
		/*groupNameColor*/ NUIE::Color (0, 0, 0),
		/*groupBackgroundColors*/ NUIE::NamedColorSet ({
			{ NE::LocalizeString (L"Blue"), NUIE::Color (160, 200, 240) },
			{ NE::LocalizeString (L"Green"), NUIE::Color (160, 239, 160) },
			{ NE::LocalizeString (L"Red"), NUIE::Color (239, 189, 160) }
			}),
		/*groupPadding*/ 10.0
	);
	return skinParams;
}

static void AddNodeTreeItem (NUIE::NodeTree& nodeTree, size_t groupIndex, const std::wstring& name, const NUIE::IconId& iconId, const NUIE::CreatorFunction& creator)
{
	nodeTree.AddItem (groupIndex, name, [=] (const NUIE::Point& position) {
		NUIE::UINodePtr node = creator (position);
		BI::BasicUINodePtr basicNode = std::dynamic_pointer_cast<BI::BasicUINode> (node);
		if (basicNode != nullptr) {
			basicNode->SetIconId (iconId);
		}
		return node;
	});
}

class MyResourceImageLoader : public WAS::Direct2DImageLoaderFromResource
{
	virtual HRSRC GetImageResHandle (const NUIE::IconId& iconId) override
	{
		HRSRC resHandle = FindResource (NULL, MAKEINTRESOURCE (iconId.GetId ()), L"IMAGE");
		return resHandle;
	}
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::BasicStringConverter (WAS::GetStringSettingsFromSystem ())),
		skinParams (GetAppSkinParams ()),
		eventHandler (),
		clipboardHandler (),
		evaluationEnv (nullptr),
		nodeEditorControl (NUIE::NativeDrawingContextPtr (new WAS::Direct2DContext (WAS::Direct2DImageLoaderPtr (new MyResourceImageLoader ()))))
	{
	
	}

	void Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle)
	{
		NUIE::NodeTree nodeTree;

		size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
		AddNodeTreeItem (nodeTree, inputNodes, L"Boolean", NUIE::IconId (101), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Integer", NUIE::IconId (102), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 5));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Number", NUIE::IconId (103), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 5.0));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Integer Increment", NUIE::IconId (104), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Number Increment", NUIE::IconId (105), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position));
		});
		AddNodeTreeItem (nodeTree, inputNodes, L"Number Distribution", NUIE::IconId (106), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position));
		});
		size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Addition", NUIE::IconId (107), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Subtraction", NUIE::IconId (108), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Multiplication", NUIE::IconId (109), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position));
		});
		AddNodeTreeItem (nodeTree, arithmeticNodes, L"Division", NUIE::IconId (110), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position));
		});
		size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
		AddNodeTreeItem (nodeTree, otherNodes, L"List Builder", NUIE::IconId (111), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position));
		});
		AddNodeTreeItem (nodeTree, otherNodes, L"Multiline Viewer", NUIE::IconId (112), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Multiline Viewer"), position, 5));
		});
		AddNodeTreeItem (nodeTree, otherNodes, L"Viewer", NUIE::IconId (112), [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ViewerNode (NE::LocString (L"Viewer"), position));
		});

		RECT clientRect;
		GetClientRect (parentHandle, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		nodeEditorControl.Init (nodeEditorPtr, parentHandle, ControlPadding, ControlPadding, width - ControlPadding * 2, height - ControlPadding * 2);
		nodeEditorControl.FillNodeTree (nodeTree);
		eventHandler.Init ((HWND) nodeEditorControl.GetEditorNativeHandle ());
	}

	void OnResize (int x, int y, int width, int height)
	{
		nodeEditorControl.Resize (x + ControlPadding, y + ControlPadding, width - ControlPadding * 2, height - ControlPadding * 2);
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

	virtual void OnSelectionChanged (const NUIE::Selection&) override
	{

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
	windowClass.hbrBackground = (HBRUSH) COLOR_WINDOW;
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
