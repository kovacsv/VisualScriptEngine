#include "NUIE_NodeEditor.hpp"
#include "WAS_BitmapContextGdi.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_HwndEventHandlers.hpp"
#include "WAS_HwndNodeUIEnvironment.hpp"
#include "WAS_ParameterDialog.hpp"
#include "BI_BuiltInNodes.hpp"

#include <windows.h>
#include <windowsx.h>

class MyEventHandlers : public WAS::HwndEventHandlers
{
public:
	MyEventHandlers () :
		WAS::HwndEventHandlers (),
		nodeEditor (nullptr),
		nodeTree ()
	{
		size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
		nodeTree.AddItem (inputNodes, L"Boolean", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::BooleanNode (NE::String (L"Boolean"), position, true));
		});
		nodeTree.AddItem (inputNodes, L"Integer", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::String (L"Integer"), position, 0, 5));
		});
		nodeTree.AddItem (inputNodes, L"Number", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::String (L"Number"), position, 0.0, 5.0));
		});
		nodeTree.AddItem (inputNodes, L"Integer Increment", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::String (L"Integer Increment"), position));
		});
		nodeTree.AddItem (inputNodes, L"Number Increment", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::String (L"Number Increment"), position));
		});
		nodeTree.AddItem (inputNodes, L"Number Distribution", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::String (L"Number Distribution"), position));
		});
		nodeTree.AddItem (inputNodes, L"List Builder", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::ListBuilderNode (NE::String (L"List Builder"), position));
		});
		size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
		nodeTree.AddItem (arithmeticNodes, L"Addition", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::AdditionNode (NE::String (L"Addition"), position));
		});
		nodeTree.AddItem (arithmeticNodes, L"Subtraction", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::SubtractionNode (NE::String (L"Subtraction"), position));
		});
		nodeTree.AddItem (arithmeticNodes, L"Multiplication", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiplicationNode (NE::String (L"Multiplication"), position));
		});
		nodeTree.AddItem (arithmeticNodes, L"Division", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::DivisionNode (NE::String (L"Division"), position));
		});
		size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
		nodeTree.AddItem (otherNodes, L"Viewer", [&] (const NUIE::Point& position) {
			return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::String (L"Viewer"), position, 5));
		});
	}

	void SetNodeEditor (NUIE::NodeEditor* nodeEditorPtr)
	{
		nodeEditor = nodeEditorPtr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override
	{
		class CreateNodeCommand : public NUIE::SingleMenuCommand
		{
		public:
			CreateNodeCommand (NUIE::NodeEditor* nodeEditor, const std::wstring& name, const NUIE::Point& position, const WAS::CreatorFunction& creator) :
				NUIE::SingleMenuCommand (name, false),
				nodeEditor (nodeEditor),
				position (position),
				creator (creator)
			{
			}

			virtual void Do () override
			{
				NUIE::UINodePtr uiNode = creator (nodeEditor->ViewToModel (position));
				nodeEditor->AddNode (uiNode);
			}

		private:
			NUIE::NodeEditor*		nodeEditor;
			NUIE::Point				position;
			WAS::CreatorFunction	creator;
		};

		NUIE::MenuCommandStructure finalCommands = commands;
		for (const WAS::NodeTree::Group& group : nodeTree.GetGroups ()) {
			NUIE::GroupMenuCommandPtr groupCommand (new NUIE::GroupMenuCommand (group.GetName ()));
			for (const WAS::NodeTree::Item& item : group.GetItems ()) {
				groupCommand->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (nodeEditor, item.GetName (), position, item.GetCreator ())));
			}
			finalCommands.AddCommand (groupCommand);
		}

		return WAS::SelectCommandFromContextMenu ((HWND) control->GetEditorNativeHandle (), position, finalCommands);
	}

private:
	NUIE::NodeEditor*	nodeEditor;
	WAS::NodeTree		nodeTree;
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::BasicStringConverter (WAS::GetStringSettingsFromSystem ())),
		skinParams (NUIE::GetDefaultSkinParams ()),
		eventHandlers (),
		evaluationEnv (nullptr),
		nodeEditorControl ()
	{

	}

	void Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle)
	{
		RECT clientRect;
		GetClientRect (parentHandle, &clientRect);
		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		nodeEditorControl.Init (nodeEditorPtr, parentHandle, 0, 0, width, height);
		eventHandlers.Init (&nodeEditorControl);
		eventHandlers.SetNodeEditor (nodeEditorPtr);
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

	virtual NUIE::EventHandlers& GetEventHandlers () override
	{
		return eventHandlers;
	}

private:
	NE::BasicStringConverter	stringConverter;
	NUIE::BasicSkinParams		skinParams;
	MyEventHandlers				eventHandlers;
	NE::EvaluationEnv			evaluationEnv;
	WAS::NodeEditorHwndControl	nodeEditorControl;
};

static std::shared_ptr<WAS::NodeEditorHwndControl> nodeEditorControl (new WAS::NodeEditorHwndControl ());
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
			break;
	}

	return DefWindowProc (hwnd, msg, wParam, lParam);
}

int wWinMain (HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nCmdShow*/)
{
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
