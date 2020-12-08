#include "AppUIEnvironment.hpp"
#include "BI_BuiltInNodes.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "WAS_Direct2DContext.hpp"
#include "WAS_GdiplusUtils.hpp"

#include "ResourceIds.hpp"
#include "CommandIds.hpp"

static const WAS::NodeEditorNodeTreeHwndControl::Settings NodeTreeControlSettings (200, 2); 

static const NUIE::BasicSkinParams& GetAppSkinParams ()
{
	static const NUIE::BasicSkinParams skinParams (
		/*backgroundColor*/ NUIE::Color (255, 255, 255),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*connectionMarker */ NUIE::SkinParams::ConnectionMarker::Circle,
		/*connectionMarkerSize*/ NUIE::Size (8.0, 8.0),
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
		/*slotMarker*/ NUIE::SkinParams::SlotMarker::Circle,
		/*slotMarkerSize*/ NUIE::Size (8.0, 8.0),
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
	NUIE::IconId nodeIconId (iconId.GetId () + TREE_TO_NODE_ICON_OFFSET);
	nodeTree.AddItem (groupIndex, name, iconId, [=] (const NUIE::Point& position) {
		NUIE::UINodePtr node = creator (position);
		BI::BasicUINodePtr basicNode = std::dynamic_pointer_cast<BI::BasicUINode> (node);
		if (basicNode != nullptr) {
			basicNode->SetIconId (nodeIconId);
		}
		return node;
	});
}

class AppResourceImageLoader : public WAS::Direct2DImageLoaderFromResource
{
	virtual HRSRC GetImageResHandle (const NUIE::IconId& iconId) override
	{
		HRSRC resHandle = FindResource (NULL, MAKEINTRESOURCE (iconId.GetId ()), L"IMAGE");
		return resHandle;
	}
};

class NodeTreeImageLoader : public WAS::NodeEditorNodeTreeHwndControl::ImageLoader
{
public:
	virtual HBITMAP LoadGroupClosedImage () override
	{
		return LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (FOLDERCLOSED_ICON));
	}

	virtual HBITMAP LoadGroupOpenedImage () override
	{
		return LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (FOLDEROPENED_ICON));
	}

	virtual HBITMAP LoadImage (const NUIE::IconId& iconId) override
	{
		return LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (iconId.GetId ()));
	}
};

static NUIE::NativeDrawingContextPtr CreateDrawingContext ()
{
	WAS::Direct2DImageLoaderPtr imageLoader (new AppResourceImageLoader ());
	return NUIE::NativeDrawingContextPtr (new WAS::Direct2DContext (imageLoader));
}

AppUIEnvironment::AppUIEnvironment () :
	NUIE::NodeUIEnvironment (),
	stringConverter (NE::BasicStringConverter (WAS::GetStringSettingsFromSystem ())),
	skinParams (GetAppSkinParams ()),
	eventHandler (),
	clipboardHandler (),
	evaluationEnv (nullptr),
	nodeEditorControl (NodeTreeControlSettings, CreateDrawingContext ()),
	fileMenu (nullptr),
	toolbar (nullptr)
{

}

void AppUIEnvironment::Init (NUIE::NodeEditor* nodeEditorPtr, WAS::FileMenu* fileMenuPtr, WAS::Toolbar* toolbarPtr, HWND parentHandle)
{
	NUIE::NodeTree nodeTree;

	size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
	AddNodeTreeItem (nodeTree, inputNodes, L"Boolean", NUIE::IconId (TREE_BOOLEAN_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::BooleanNode (NE::LocString (L"Boolean"), position, true));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Integer", NUIE::IconId (TREE_INTEGER_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Number", NUIE::IconId (TREE_DOUBLE_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Integer Increment", NUIE::IconId (TREE_INTEGERINCREMENTED_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Number Increment", NUIE::IconId (TREE_DOUBLEINCREMENTED_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Number Distribution", NUIE::IconId (TREE_DOUBLEDISTRIBUTED_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position));
	});
	size_t mathematicalNodes = nodeTree.AddGroup (L"Mathematical Nodes");
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Addition", NUIE::IconId (TREE_ADDITION_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Subtraction", NUIE::IconId (TREE_SUBTRACTION_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Multiplication", NUIE::IconId (TREE_MULTIPLICATION_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Division", NUIE::IconId (TREE_DIVISION_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Floor", NUIE::IconId (TREE_FLOOR_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::FloorNode (NE::LocString (L"Floor"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Ceil", NUIE::IconId (TREE_CEIL_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::CeilNode (NE::LocString (L"Ceil"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Abs", NUIE::IconId (TREE_ABS_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::AbsNode (NE::LocString (L"Abs"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Negative", NUIE::IconId (TREE_NEGATIVE_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::NegativeNode (NE::LocString (L"Negative"), position));
	});
	AddNodeTreeItem (nodeTree, mathematicalNodes, L"Sqrt", NUIE::IconId (TREE_SQRT_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::SqrtNode (NE::LocString (L"Sqrt"), position));
	});
	size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
	AddNodeTreeItem (nodeTree, otherNodes, L"List Builder", NUIE::IconId (TREE_LISTBUILDER_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::ListBuilderNode (NE::LocString (L"List Builder"), position));
	});
	AddNodeTreeItem (nodeTree, otherNodes, L"Viewer", NUIE::IconId (TREE_VIEWER_ICON), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5));
	});

	RECT clientRect;
	GetClientRect (parentHandle, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	NodeTreeImageLoader imageLoader;
	nodeEditorControl.Init (nodeEditorPtr, parentHandle, 0, 0, width, height);
	nodeEditorControl.FillNodeTree (nodeTree, &imageLoader);
	eventHandler.Init ((HWND) nodeEditorControl.GetEditorNativeHandle ());

	fileMenu = fileMenuPtr;
	toolbar = toolbarPtr;

	EnableCommand (EDIT_SETTINGS, false);
	EnableCommand (EDIT_COPY, false);
	EnableCommand (EDIT_DELETE, false);
	EnableCommand (EDIT_GROUP, false);
	EnableCommand (EDIT_UNGROUP, false);
	EnableCommand (EDIT_UNDO, false);
	EnableCommand (EDIT_REDO, false);
	EnableCommand (EDIT_PASTE, clipboardHandler.HasClipboardContent ());
}

void AppUIEnvironment::OnResize (int x, int y, int width, int height)
{
	int toolbarHeight = 0;
	if (toolbar != nullptr) {
		toolbarHeight = toolbar->GetHeight ();
	}
	nodeEditorControl.Resize (x, y + toolbarHeight, width, height - toolbarHeight);
}

void AppUIEnvironment::OnClipboardChanged ()
{
	EnableCommand (EDIT_PASTE, clipboardHandler.HasClipboardContent ());
}

const NE::StringConverter& AppUIEnvironment::GetStringConverter ()
{
	return stringConverter;
}

const NUIE::SkinParams& AppUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NUIE::DrawingContext& AppUIEnvironment::GetDrawingContext ()
{
	return nodeEditorControl.GetDrawingContext ();
}

double AppUIEnvironment::GetWindowScale ()
{
	return 1.0;
}

NE::EvaluationEnv& AppUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void AppUIEnvironment::OnEvaluationBegin ()
{

}

void AppUIEnvironment::OnEvaluationEnd ()
{

}

void AppUIEnvironment::OnValuesRecalculated ()
{
		
}

void AppUIEnvironment::OnRedrawRequested ()
{
	nodeEditorControl.Invalidate ();
}

NUIE::EventHandler& AppUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

NUIE::ClipboardHandler& AppUIEnvironment::GetClipboardHandler ()
{
	return clipboardHandler;
}

void AppUIEnvironment::OnSelectionChanged (const NUIE::Selection& selection)
{
	bool hasSelection = !selection.IsEmpty ();
	EnableCommand (EDIT_SETTINGS, hasSelection);
	EnableCommand (EDIT_COPY, hasSelection);
	EnableCommand (EDIT_DELETE, hasSelection);
	EnableCommand (EDIT_GROUP, hasSelection);
	EnableCommand (EDIT_UNGROUP, hasSelection);
}

void AppUIEnvironment::OnUndoStateChanged (const NUIE::UndoState& undoState)
{
	EnableCommand (EDIT_UNDO, undoState.CanUndo ());
	EnableCommand (EDIT_REDO, undoState.CanRedo ());
}

void AppUIEnvironment::OnClipboardStateChanged (const NUIE::ClipboardState& clipboardState)
{
	EnableCommand (EDIT_PASTE, clipboardState.HasContent ());
}

void AppUIEnvironment::OnIncompatibleVersionPasted (const NUIE::Version&)
{

}

void AppUIEnvironment::EnableCommand (UINT commandId, bool isEnabled)
{
	fileMenu->EnableItem (commandId, isEnabled);
	toolbar->EnableItem (commandId, isEnabled);
}
