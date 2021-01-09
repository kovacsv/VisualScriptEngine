#include "MAS_ParameterDialog.hpp"
#include "MAS_CocoaAppUtils.hpp"

@interface ParameterWindow : NSWindow
{
	
}
@end

@implementation ParameterWindow

- (void) addStaticControl : (NSString*) text : (NSRect) rect
{
	// TODO: align to center vertically
	NSTextField* textField = [[NSTextField alloc] initWithFrame : rect];
	[textField setStringValue : text];
    [textField setBezeled : NO];
    [textField setDrawsBackground : NO];
    [textField setEditable : NO];
    [textField setSelectable : NO];
    [[self contentView] addSubview : textField];
}

- (void) addEditControl : (size_t) id : (NSString*) text : (NSRect) rect
{
	NSTextField* textField = [[NSTextField alloc] initWithFrame : rect];
	[textField setStringValue : text];
	[textField setIdentifier : (NSUserInterfaceItemIdentifier ([@(id) stringValue]))];
	[textField setDelegate : [self windowController]];
	[[self contentView] addSubview : textField];
}

- (void) addComboBox : (size_t) id : (int) selectedChoice : (const std::vector<std::wstring>&) choices : (NSRect) rect
{
	NSPopUpButton* popup = [[NSPopUpButton alloc] initWithFrame : rect];
	for (const std::wstring& choice : choices) {
		[popup addItemWithTitle : MAS::StdWStringToNSString (choice)];
	}
	[popup selectItemAtIndex : selectedChoice];
	[popup setIdentifier : (NSUserInterfaceItemIdentifier ([@(id) stringValue]))];
	[popup setTarget : [self windowController]];
	[popup setAction : @selector (popupDidChange:)];
	[[self contentView] addSubview : popup];
}

- (void) addSeparator : (NSRect) rect
{
	NSBox* separator = [[NSBox alloc] initWithFrame : rect];
	[separator setBoxType : NSBoxSeparator];
	[[self contentView] addSubview : separator];
}

- (void) addButton : (NSUserInterfaceItemIdentifier) identifier : (NSString*) text : (NSRect) rect
{
	NSButton* button = [[NSButton alloc] initWithFrame : rect];
	[button setIdentifier : identifier];
	[button setTitle : text];
	[button setBezelStyle : NSBezelStyleRounded];
	[button setTarget : [self windowController]];
	[button setAction : @selector (buttonClicked:)];
	[[self contentView] addSubview : button];
}

- (NSString*) getEditControlValue : (size_t) id
{
	for (NSView* view in [[self contentView] subviews]) {
		if ([[view identifier] length] == 0) {
			continue;
		}
		size_t itemId = [[view identifier] intValue];
		if (itemId == id) {
			NSTextField* textField = (NSTextField*) view;
			return [textField stringValue];
		}
	}
	return nil;
}

- (NSString*) setEditControlValue : (size_t) id : (NSString*) value
{
	for (NSView* view in [[self contentView] subviews]) {
		if ([[view identifier] length] == 0) {
			continue;
		}
		size_t itemId = [[view identifier] intValue];
		if (itemId == id) {
			NSTextField* textField = (NSTextField*) view;
			[textField setStringValue : value];
		}
	}
	return nil;
}

- (int) getComboboxSelectedItem : (size_t) id
{
	for (NSView* view in [[self contentView] subviews]) {
		if ([[view identifier] length] == 0) {
			continue;
		}
		size_t itemId = [[view identifier] intValue];
		if (itemId == id) {
			NSPopUpButton* popupButton = (NSPopUpButton*) view;
			return [popupButton indexOfSelectedItem];
		}
	}
	return 0;
}

@end

@interface ParameterDialogController : NSWindowController<NSWindowDelegate>
{
@private MAS::ParameterDialog* paramDialog;
@private NSModalResponse modalResponse;
}
@end

@implementation ParameterDialogController

- (id) init : (MAS::ParameterDialog*) newParamDialog
{
	paramDialog = newParamDialog;
	modalResponse = NSModalResponseCancel;
	return self;
}

- (void) popupDidChange : (id) sender
{
	NSPopUpButton* popup = sender;
	size_t paramIndex = [[popup identifier] intValue];
	paramDialog->SetParameterChanged (paramIndex);
}

- (void) controlTextDidChange : (NSNotification*) notification
{
	NSTextField* textField = [notification object];
	size_t paramIndex = [[textField identifier] intValue];
	paramDialog->SetParameterChanged (paramIndex);
	#pragma unused (notification)
}

- (void) buttonClicked : (id) sender
{
	NSButton* button = sender;
	if ([[button identifier] isEqualToString : @"cancel"]) {
		[[self window] close];
	} else if ([[button identifier] isEqualToString : @"ok"]) {
		if (paramDialog->ApplyParameterChanges ()) {
			modalResponse = NSModalResponseOK;
			[[self window] close];
		}
	}
}

- (void) windowDidLoad
{

}

- (void) windowWillClose : (NSNotification*) notification
{
	#pragma unused (notification)
	[NSApp stopModalWithCode : modalResponse];
}

@end

namespace MAS
{

static NSSize CreateDialogSize (int width, int height)
{
	return NSMakeSize (width * 2, height * 2);
}

static NSRect CreateDialogRect (const NSWindow* window, const NUIE::IntRect& rect)
{
	int height = [[window contentView] frame].size.height;
	return NSMakeRect (rect.GetLeft () * 2, height - rect.GetTop () * 2 - rect.GetHeight () * 2, rect.GetWidth () * 2, rect.GetHeight () * 2);
}

ParameterDialog::ParameterDialog (const std::wstring& dialogTitle, NUIE::ParameterInterfacePtr& paramInterface) :
	NUIE::ParameterDialogBase (dialogTitle, paramInterface),
	paramDialog (nil)
{
	NSRect windowRect = NSMakeRect (0.0f, 0.0f, 200.0f, 300.0f);
	paramDialog = [
		[ParameterWindow alloc]
			initWithContentRect : windowRect
			styleMask :
				NSWindowStyleMaskTitled |
				NSWindowStyleMaskClosable
			backing : NSBackingStoreBuffered
			defer : NO
	];
	
	ParameterDialogController* paramDialogController = [[[ParameterDialogController alloc] init : this] autorelease];
	[paramDialogController setWindow : paramDialog];
	[paramDialog setDelegate : paramDialogController];
	
	[paramDialog setTitle : StdWStringToNSString (dialogTitle)];
}
	
void ParameterDialog::SetDialogRect (const NUIE::IntRect& rect)
{
	[paramDialog setContentSize : CreateDialogSize (rect.GetWidth (), rect.GetHeight ())];
}

void ParameterDialog::AddParamNameStatic (size_t, const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addStaticControl : StdWStringToNSString (controlText) : CreateDialogRect (paramDialog, rect)];
}

void ParameterDialog::AddParamEditText (size_t paramIndex, const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addEditControl : paramIndex : StdWStringToNSString (controlText) : CreateDialogRect (paramDialog, rect)];
}

void ParameterDialog::AddParamComboBox (size_t paramIndex, int selectedChoice, const std::vector<std::wstring>& choices, const NUIE::IntRect& rect)
{
	[paramDialog addComboBox : paramIndex : selectedChoice : choices : CreateDialogRect (paramDialog, rect)];
}

void ParameterDialog::AddHorizontalSeparator (int x, int y, int width)
{
	[paramDialog addSeparator : CreateDialogRect (paramDialog, NUIE::IntRect (x, y, width, 1))];
}

void ParameterDialog::AddCancelButton (const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addButton : @"cancel" : StdWStringToNSString (controlText) : CreateDialogRect (paramDialog, rect)];
}

void ParameterDialog::AddOkButton (const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addButton : @"ok" : StdWStringToNSString (controlText) : CreateDialogRect (paramDialog, rect)];
}

bool ParameterDialog::ShowDialog ()
{
	[paramDialog center];
	[paramDialog makeKeyAndOrderFront : nil];
	NSModalResponse response = [NSApp runModalForWindow : paramDialog];
	if (response == NSModalResponseOK) {
		return true;
	}
	return false;
}

std::wstring ParameterDialog::GetEditTextValue (size_t paramIndex)
{
	return NSStringToStdWString ([paramDialog getEditControlValue:paramIndex]);
}

void ParameterDialog::SetEditTextValue (size_t paramIndex, const std::wstring& text)
{
	[paramDialog setEditControlValue:paramIndex:StdWStringToNSString (text)];
}

int ParameterDialog::GetComboboxSelectedItem (size_t paramIndex)
{
	return ([paramDialog getComboboxSelectedItem:paramIndex]);
}
	
}
