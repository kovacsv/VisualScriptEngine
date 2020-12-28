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

- (void) addMenuItem : (NSString*) name : (bool) isChecked : (int) tag : (ContextMenu*) target
{
	NSMenuItem* item = [self addItemWithTitle : name action : @selector(onItemSelected:) keyEquivalent : @""];
	[item setState : isChecked ? NSControlStateValueOn : NSControlStateValueOff];
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
	return [[[NSString alloc] initWithBytes : str.data () length : str.length() * sizeof (wchar_t) encoding : NSUTF32LittleEndianStringEncoding] autorelease];
}

NUIE::ModifierKeys GetModifierKeysFromEvent (const NSEvent* event)
{
	NUIE::ModifierKeys keys;
	if ([event modifierFlags] & NSEventModifierFlagCommand) {
		keys.Insert (NUIE::ModifierKeyCode::Command);
	}
	if ([event modifierFlags] & NSEventModifierFlagShift) {
		keys.Insert (NUIE::ModifierKeyCode::Shift);
	}
	return keys;
}

NUIE::Point GetViewPositionFromEvent (const NSView* view, const NSEvent* event)
{
	NSPoint position = [view convertPoint:[event locationInWindow] fromView:nil];
	return NUIE::Point (position.x, view.frame.size.height - position.y);
}

NSPoint CreateScreenPoint (const NSView* view, const NUIE::Point& point)
{
	NUIE::IntPoint intPoint (point);
	int height = (int) std::floor (view.frame.size.height) - 1;
	NSPoint viewPoint = NSMakePoint (intPoint.GetX (), height - intPoint.GetY ());
	return [view convertPoint:viewPoint toView:nil];
}

NSImage* FlipImageVertically (const NSImage* image)
{
	NSImage *tmpImage;
	NSAffineTransform *transform = [NSAffineTransform transform];

	NSSize dimensions = [image size];
	NSAffineTransformStruct flip = {1.0, 0.0, 0.0, -1.0, 0.0, dimensions.height};
	tmpImage = [[NSImage alloc] initWithSize:dimensions];
	[tmpImage lockFocus];
	[transform setTransformStruct:flip];
	[transform concat];
	[image drawAtPoint:NSMakePoint(0,0) fromRect:NSMakeRect(0,0, dimensions.width, dimensions.height) operation:NSCompositingOperationCopy fraction:1.0];
	[tmpImage unlockFocus];

	return [tmpImage autorelease];
}

NSImage* FlipImageHorizontally (const NSImage* image)
{
	NSImage *tmpImage;
	NSAffineTransform *transform = [NSAffineTransform transform];

	NSSize dimensions = [image size];
	NSAffineTransformStruct flip = {-1.0, 0.0, 0.0, 1.0, 0.0, dimensions.width};
	tmpImage = [[NSImage alloc] initWithSize:dimensions];
	[tmpImage lockFocus];
	[transform setTransformStruct:flip];
	[transform concat];
	[image drawAtPoint:NSMakePoint(0,0) fromRect:NSMakeRect(0,0, dimensions.width, dimensions.height) operation:NSCompositingOperationCopy fraction:1.0];
	[tmpImage unlockFocus];

	return [tmpImage autorelease];
}

static void AddCommandToMenu (const NUIE::MenuCommandPtr& command, std::unordered_map<int, NUIE::MenuCommandPtr>& commandTable, ContextMenu* originalMenu, ContextMenu* currentMenu, int& currentCommandId)
{
	if (command->HasChildCommands ()) {
		ContextMenu* oldMenu = currentMenu;
		ContextMenu* newMenu = [currentMenu addGroupMenuItem:StdWStringToNSString (command->GetName ())];
		currentMenu = newMenu;
		command->EnumerateChildCommands ([&] (const NUIE::MenuCommandPtr& childCommand) {
			AddCommandToMenu (childCommand, commandTable, originalMenu, currentMenu, currentCommandId);
		});
		currentMenu = oldMenu;
	} else {
		[currentMenu addMenuItem:StdWStringToNSString (command->GetName ()) : command->IsChecked () : currentCommandId : originalMenu];
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
