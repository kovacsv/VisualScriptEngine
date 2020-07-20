#include "MAS_CocoaAppUtils.hpp"

@interface ContextMenu : NSMenu
{
@private int selectedItem;
}
@end

@implementation ContextMenu

- (id) init
{
	self = [super init];
	if (self) {
		selectedItem = -1;
	}
	return self;
}

- (ContextMenu*) addGroupMenuItem : (NSString*) name
{
	NSMenuItem* item = [self addItemWithTitle : name action : nil keyEquivalent : @""];
	ContextMenu* subMenu = [[[ContextMenu alloc] init] autorelease];
	[item setSubmenu : subMenu];
	return subMenu;
}

- (void) addMenuItem : (NSString*) name : (int) tag : (ContextMenu*) target
{
	NSMenuItem* item = [self addItemWithTitle : name action : @selector(onItemSelected:) keyEquivalent : @""];
	[item setTarget : target];
	[item setTag : tag];
}

- (int) getSelectedItem
{
	return selectedItem;
}

- (void) onItemSelected : (id) sender
{
	selectedItem = [(NSMenuItem*) sender tag];
}

@end

namespace MAS
{

NSString* StdStringToNSString (const std::wstring& str)
{
	return [[[NSString alloc] initWithBytes : str.data () length : str.length() * sizeof (wchar_t) encoding : NSUTF32LittleEndianStringEncoding] autorelease];
}
	
NUIE::ModifierKeys GetModifierKeysFromEvent (const NSEvent* event)
{
	NUIE::ModifierKeys keys;
	NSEventModifierFlags flags = [event modifierFlags];
	if (flags & NSEventModifierFlagControl) {
		keys.Insert (NUIE::ModifierKeyCode::Control);
	} else if (flags & NSEventModifierFlagShift) {
		keys.Insert (NUIE::ModifierKeyCode::Shift);
	}
	return keys;
}

NUIE::Point GetViewPositionFromEvent (NSView* view, const NSEvent* event)
{
	NSPoint position = [view convertPoint:[event locationInWindow] fromView:nil];
	return NUIE::Point (position.x, view.frame.size.height - position.y);
}
	
NSPoint CreatePoint (const NSView* view, const NUIE::Point& point)
{
	return NSMakePoint (point.GetX (), view.frame.size.height - point.GetY ());
}
	
NSPoint CreateScreenPoint (const NSView* view, const NUIE::Point& point)
{
	NSPoint viewPoint = MAS::CreatePoint (view, point);
	return [view convertPoint:viewPoint toView:nil];
}
	
NSRect CreateRect (const NSView* view, const NUIE::Rect& rect)
{
	return NSMakeRect (rect.GetX (), view.frame.size.height - rect.GetHeight () - rect.GetY (), rect.GetWidth (), rect.GetHeight ());
}

static void AddCommandToMenu (const NUIE::MenuCommandPtr& command, std::unordered_map<int, NUIE::MenuCommandPtr>& commandTable, ContextMenu* originalMenu, ContextMenu* currentMenu, int& currentCommandId)
{
	if (command->HasChildCommands ()) {
		ContextMenu* oldMenu = currentMenu;
		ContextMenu* newMenu = [currentMenu addGroupMenuItem:MAS::StdStringToNSString (command->GetName ())];
		currentMenu = newMenu;
		command->EnumerateChildCommands ([&] (const NUIE::MenuCommandPtr& childCommand) {
			AddCommandToMenu (childCommand, commandTable, originalMenu, currentMenu, currentCommandId);
		});
		currentMenu = oldMenu;
	} else {
		[currentMenu addMenuItem : MAS::StdStringToNSString (command->GetName ()) : currentCommandId : originalMenu];
		commandTable.insert ({ currentCommandId, command });
		currentCommandId += 1;
	}
}

NUIE::MenuCommandPtr SelectCommandFromContextMenu (const NSView* nsView, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	ContextMenu* contextMenu = [[[ContextMenu alloc] init] autorelease];
	int currentCommandId = 0;
	
	std::unordered_map<int, NUIE::MenuCommandPtr> commandTable;
	
	ContextMenu* originalMenu = contextMenu;
	ContextMenu* currentMenu = contextMenu;
	commands.EnumerateCommands ([&] (const NUIE::MenuCommandPtr& command) {
		AddCommandToMenu (command, commandTable, originalMenu, currentMenu, currentCommandId);
	});
	
	NSPoint screenPosition = MAS::CreateScreenPoint (nsView, position);
	[contextMenu setAutoenablesItems:YES];
	[contextMenu popUpMenuPositioningItem:nil atLocation:screenPosition inView:(NSView*) nsView];
	
	int selectedItem = [contextMenu getSelectedItem];
	if (selectedItem == -1) {
		return nullptr;
	}
	return commandTable[selectedItem];
}
	
}
