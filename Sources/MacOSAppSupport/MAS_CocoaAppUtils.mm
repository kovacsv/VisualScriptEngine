#include "MAS_CocoaAppUtils.hpp"
#include "NE_StringUtils.hpp"

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

NSString* StdWStringToNSString (const std::wstring& str)
{
	@autoreleasepool {
		@try {
			return [[[NSString alloc] initWithBytes : str.data () length : str.length() * sizeof (wchar_t) encoding : NSUTF32LittleEndianStringEncoding] autorelease];
		} @catch (NSException*) {
			
		}
	}
	return nullptr;
}

NUIE::ModifierKeys GetModifierKeysFromEvent (const NSEvent* event)
{
	NUIE::ModifierKeys keys;
	@autoreleasepool {
		@try {
			if ([event modifierFlags] & NSEventModifierFlagControl) {
				keys.Insert (NUIE::ModifierKeyCode::Control);
			}
			if ([event modifierFlags] & NSEventModifierFlagShift) {
				keys.Insert (NUIE::ModifierKeyCode::Shift);
			}
		} @catch (NSException*) {
			
		}
	}
	return keys;
}

NUIE::Point GetViewPositionFromEvent (const NSView* view, const NSEvent* event)
{
	@autoreleasepool {
		@try {
			NSPoint position = [view convertPoint:[event locationInWindow] fromView:nil];
			return NUIE::Point (position.x, view.frame.size.height - position.y);
		} @catch (NSException*) {
			
		}
	}
	return NUIE::Point ();
}
	
NSPoint CreatePoint (const NSView* view, const NUIE::Point& point)
{
	@autoreleasepool {
		@try {
			NUIE::IntPoint intPoint (point);
			int height = (int) std::floor (view.frame.size.height) - 1;
			return NSMakePoint (intPoint.GetX (), height - intPoint.GetY ());
		} @catch (NSException*) {
			
		}
	}
	return NSPoint ();
}
	
NSPoint CreateScreenPoint (const NSView* view, const NUIE::Point& point)
{
	@autoreleasepool {
		@try {
			NSPoint viewPoint = CreatePoint (view, point);
			return [view convertPoint:viewPoint toView:nil];
		} @catch (NSException*) {
			
		}
	}
	return NSPoint ();
}
	
NSRect CreateRect (const NSView* view, const NUIE::Rect& rect)
{
	@autoreleasepool {
		@try {
			NUIE::IntRect intRect (rect);
			int height = (int) std::floor (view.frame.size.height) - 1;
			return NSMakeRect (intRect.GetX (), height - intRect.GetHeight () - intRect.GetY (), intRect.GetWidth (), intRect.GetHeight ());
		} @catch (NSException*) {
			
		}
	}
	return NSRect ();
}

NSColor* CreateColor (const NUIE::Color& color)
{
	return [NSColor colorWithRed:color.GetR () / 255.0f green:color.GetG () / 255.0f blue:color.GetB () / 255.0f alpha:1.0f];
}
	
static void AddCommandToMenu (const NUIE::MenuCommandPtr& command, std::unordered_map<int, NUIE::MenuCommandPtr>& commandTable, ContextMenu* originalMenu, ContextMenu* currentMenu, int& currentCommandId)
{
	if (command->HasChildCommands ()) {
		ContextMenu* oldMenu = currentMenu;
		ContextMenu* newMenu = [currentMenu addGroupMenuItem:MAS::StdWStringToNSString (command->GetName ())];
		currentMenu = newMenu;
		command->EnumerateChildCommands ([&] (const NUIE::MenuCommandPtr& childCommand) {
			AddCommandToMenu (childCommand, commandTable, originalMenu, currentMenu, currentCommandId);
		});
		currentMenu = oldMenu;
	} else {
		[currentMenu addMenuItem:MAS::StdWStringToNSString (command->GetName ()) : currentCommandId : originalMenu];
		commandTable.insert ({ currentCommandId, command });
		currentCommandId += 1;
	}
}

NUIE::MenuCommandPtr SelectCommandFromContextMenu (const NSView* nsView, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands)
{
	@autoreleasepool {
		@try {
			ContextMenu* contextMenu = [[[ContextMenu alloc] init] autorelease];
			int currentCommandId = 0;
			
			std::unordered_map<int, NUIE::MenuCommandPtr> commandTable;
			
			ContextMenu* originalMenu = contextMenu;
			ContextMenu* currentMenu = contextMenu;
			commands.EnumerateCommands ([&] (const NUIE::MenuCommandPtr& command) {
				AddCommandToMenu (command, commandTable, originalMenu, currentMenu, currentCommandId);
			});
			
			NSPoint screenPosition = CreateScreenPoint (nsView, position);
			[contextMenu setAutoenablesItems:YES];
			[contextMenu popUpMenuPositioningItem:nil atLocation:screenPosition inView:(NSView*) nsView];
			
			int selectedItem = [contextMenu getSelectedItem];
			if (selectedItem == -1) {
				return nullptr;
			}
			return commandTable[selectedItem];
			
		} @catch (NSException*) {
			
		}
	}
	return nullptr;
}
	
}
