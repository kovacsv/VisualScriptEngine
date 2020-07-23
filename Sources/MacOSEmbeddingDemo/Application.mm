#include "Application.hpp"
#include "MAS_CocoaAppUtils.hpp"
#include "MAS_MacOSAppUtils.hpp"
#include "MAS_NSImageLoader.hpp"
#include "MAS_NSViewContext.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

#include "NUIE_NodeTree.hpp"
#include "BI_BuiltInNodes.hpp"

@interface WindowController : NSWindowController<NSWindowDelegate>
{
@private AppNodeUIEnvironment* uiEnvironment;
}
@end

@implementation WindowController

- (id) init
{
	self = [super init];
	if (self) {
		uiEnvironment = nil;
	}
	return self;
}

-(void) setUIEnvironment : (AppNodeUIEnvironment*) newUIEnvironment
{
	uiEnvironment = newUIEnvironment;
}

-(void) windowDidLoad
{
    
}

-(void) windowWillClose : (NSNotification*) notification
{
	#pragma unused (notification)
	[NSApp terminate:self];
}

-(void) windowDidResize:(NSNotification *) notification
{
	#pragma unused (notification)
	NSRect clientRect = [[[self window] contentView] frame];
	uiEnvironment->Resize (0, 0, clientRect.size.width, clientRect.size.height);
}

@end

static const NUIE::BasicSkinParams& GetAppSkinParams ()
{
	static const NUIE::BasicSkinParams skinParams (
		/*backgroundColor*/ NUIE::Color (250, 250, 250),
		/*connectionLinePen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodePadding*/ 5.0,
		/*nodeBorderPen*/ NUIE::Pen (NUIE::Color (38, 50, 56), 1.0),
		/*nodeHeaderTextFont*/ NUIE::Font (L"Helvetica", 16.0),
		/*nodeHeaderTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderErrorTextColor*/ NUIE::Color (250, 250, 250),
		/*nodeHeaderBackgroundColor*/ NUIE::Color (41, 127, 255),
		/*nodeHeaderErrorBackgroundColor*/ NUIE::Color (199, 80, 80),
		/*nodeContentTextFont*/ NUIE::Font (L"Helvetica", 14.0),
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
		/*groupNameFont*/ NUIE::Font (L"Helvetica", 16.0),
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

static const std::vector<NSString*> IconResourceNames = {
	@"Integer.png",
	@"Number.png",
	@"Addition.png",
	@"Subtraction.png",
	@"Multiplication.png",
	@"Division.png",
	@"Viewer.png"
};

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

static void InitNodeTree (NUIE::NodeTree& nodeTree)
{
	size_t inputNodes = nodeTree.AddGroup (L"Input Nodes");
	AddNodeTreeItem (nodeTree, inputNodes, L"Integer", NUIE::IconId (1), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), position, 0, 1));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Number", NUIE::IconId (2), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleUpDownNode (NE::LocString (L"Number"), position, 0.0, 1.0));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Integer Increment", NUIE::IconId (1), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::IntegerIncrementedNode (NE::LocString (L"Integer Increment"), position));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Number Increment", NUIE::IconId (2), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleIncrementedNode (NE::LocString (L"Number Increment"), position));
	});
	AddNodeTreeItem (nodeTree, inputNodes, L"Number Distribution", NUIE::IconId (2), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DoubleDistributedNode (NE::LocString (L"Number Distribution"), position));
	});
	size_t arithmeticNodes = nodeTree.AddGroup (L"Arithmetic Nodes");
	AddNodeTreeItem (nodeTree, arithmeticNodes, L"Addition", NUIE::IconId (3), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::AdditionNode (NE::LocString (L"Addition"), position));
	});
	AddNodeTreeItem (nodeTree, arithmeticNodes, L"Subtraction", NUIE::IconId (4), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::SubtractionNode (NE::LocString (L"Subtraction"), position));
	});
	AddNodeTreeItem (nodeTree, arithmeticNodes, L"Multiplication", NUIE::IconId (5), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::MultiplicationNode (NE::LocString (L"Multiplication"), position));
	});
	AddNodeTreeItem (nodeTree, arithmeticNodes, L"Division", NUIE::IconId (6), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::DivisionNode (NE::LocString (L"Division"), position));
	});
	size_t otherNodes = nodeTree.AddGroup (L"Other Nodes");
	AddNodeTreeItem (nodeTree, otherNodes, L"Viewer", NUIE::IconId (7), [&] (const NUIE::Point& position) {
		return NUIE::UINodePtr (new BI::MultiLineViewerNode (NE::LocString (L"Viewer"), position, 5));
	});
}

class AppImageLoader : public MAS::NSImageLoader
{
public:
	AppImageLoader () :
		MAS::NSImageLoader ()
	{
		
	}
	
private:
	virtual NSImage* CreateNSImage (const NUIE::IconId& iconId) override
	{
		return [NSImage imageNamed : IconResourceNames[iconId.GetId () - 1]];
	}
};

AppEventHandler::AppEventHandler () :
	nodeTree (),
	nodeEditor (nullptr),
	nsView (nullptr)
{
	InitNodeTree (nodeTree);
}

AppEventHandler::~AppEventHandler ()
{
	
}

void AppEventHandler::Init (NUIE::NodeEditor* nodeEditorPtr, void* nsViewPtr)
{
	nodeEditor = nodeEditorPtr;
	nsView = nsViewPtr;
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	NUIE::MenuCommandStructure finalCommands = commands;
	NUIE::AddNodeTreeToMenuStructure (nodeTree, position, nodeEditor, finalCommands);
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, finalCommands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UINodePtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);
}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UIInputSlotConstPtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);

}

NUIE::MenuCommandPtr AppEventHandler::OnContextMenu(const NUIE::Point& position, const NUIE::UINodeGroupPtr&, const NUIE::MenuCommandStructure& commands)
{
	return MAS::SelectCommandFromContextMenu ((NSView*) nsView, position, commands);

}

void AppEventHandler::OnDoubleClick(NUIE::MouseButton mouseButton, const NUIE::Point& position)
{
	#pragma unused (mouseButton)
	#pragma unused (position)
}

bool AppEventHandler::OnParameterSettings(NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodePtr& uiNode)
{
	#pragma unused (paramAccessor)
	#pragma unused (uiNode)
	return false;
}

bool AppEventHandler::OnParameterSettings(NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodeGroupPtr& uiGroup)
{
	#pragma unused (paramAccessor)
	#pragma unused (uiGroup)
	return false;
}

AppNodeUIEnvironment::AppNodeUIEnvironment () :
	stringConverter (NE::BasicStringConverter (MAS::GetStringSettingsFromSystem ())),
	skinParams (GetAppSkinParams ()),
	evaluationEnv (NE::EmptyEvaluationEnv),
	eventHandler (),
	clipboardHandler (),
	nodeEditorControl (NUIE::NativeDrawingContextPtr (new MAS::NSViewContext (MAS::NSImageLoaderPtr (new AppImageLoader ()))))
{
	
}

AppNodeUIEnvironment::~AppNodeUIEnvironment ()
{

}

void AppNodeUIEnvironment::Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height)
{
	nodeEditorControl.Init (nodeEditorPtr, nativeParentHandle, x, y, width, height);
	eventHandler.Init (nodeEditorPtr, nativeParentHandle);
}

void AppNodeUIEnvironment::Resize (int x, int y, int width, int height)
{
	nodeEditorControl.Resize (x, y, width, height);
}

const NE::StringConverter& AppNodeUIEnvironment::GetStringConverter ()
{
	return stringConverter;
}

const NUIE::SkinParams& AppNodeUIEnvironment::GetSkinParams ()
{
	return skinParams;
}

NUIE::DrawingContext& AppNodeUIEnvironment::GetDrawingContext( )
{
	return nodeEditorControl.GetDrawingContext ();
}

double AppNodeUIEnvironment::GetWindowScale ()
{
	return 1.0;
}

NE::EvaluationEnv& AppNodeUIEnvironment::GetEvaluationEnv ()
{
	return evaluationEnv;
}

void AppNodeUIEnvironment::OnEvaluationBegin ()
{

}

void AppNodeUIEnvironment::OnEvaluationEnd ()
{

}

void AppNodeUIEnvironment::OnValuesRecalculated ()
{

}

void AppNodeUIEnvironment::OnRedrawRequested ()
{
	nodeEditorControl.Invalidate ();
}

NUIE::EventHandler& AppNodeUIEnvironment::GetEventHandler ()
{
	return eventHandler;
}

NUIE::ClipboardHandler& AppNodeUIEnvironment::GetClipboardHandler ()
{
	return clipboardHandler;
}

double AppNodeUIEnvironment::GetMouseMoveMinOffset ()
{
	return 2.0;
}

Application::Application () :
	uiEnvironment (),
	nodeEditor (uiEnvironment)
{

}

void Application::Run ()
{
	NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	NSApp = [NSApplication sharedApplication];

	NSRect windowRect = NSMakeRect (0.0f, 0.0f, 900.0f, 500.0f);
	NSWindow* myWindow = [
	[NSWindow alloc]
		initWithContentRect:windowRect
		styleMask:
			NSWindowStyleMaskTitled |
			NSWindowStyleMaskClosable |
			NSWindowStyleMaskMiniaturizable |
			NSWindowStyleMaskResizable
		backing:NSBackingStoreBuffered
		defer:NO
	];

	[myWindow setTitle:@"VisualScriptEngine Embedding Demo"];

	WindowController* myWindowController = [[WindowController alloc] autorelease];
	NSView* contentView = [[[NSView alloc] initWithFrame:windowRect] autorelease];

	[myWindowController setWindow:myWindow];
	[myWindowController setUIEnvironment:&uiEnvironment];

	[myWindow setContentView:contentView];
	[myWindow setDelegate:myWindowController];

	// TODO: temporary
	nodeEditor.AddNode (NUIE::UINodePtr (new BI::IntegerUpDownNode (NE::LocString (L"Integer"), NUIE::Point (100, 100), 5, 1)));
	nodeEditor.AddNode (NUIE::UINodePtr (new BI::ViewerNode (NE::LocString (L"Viewer"), NUIE::Point (300, 200))));

	uiEnvironment.Init (&nodeEditor, contentView, 0, 0, windowRect.size.width, windowRect.size.height);
	[myWindow center];
	[myWindow makeKeyAndOrderFront:nil];

	NSImage* image = [NSImage imageNamed:@"Additi2on.png"];
#pragma unused (image)
	[NSApp run];

	[NSApp release];
	[pool release];
}
