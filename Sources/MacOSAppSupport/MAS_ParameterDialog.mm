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
	NSTextField* textField = [[NSTextField alloc] initWithFrame:rect];
	[textField setStringValue:text];
    [textField setBezeled:NO];
    [textField setDrawsBackground:NO];
    [textField setEditable:NO];
    [textField setSelectable:NO];
    [[self contentView] addSubview:textField];
}

- (void) addEditControl : (NSString*) text : (NSRect) rect
{
	NSTextField* textField = [[NSTextField alloc] initWithFrame:rect];
	[textField setStringValue:text];
	[textField setDelegate:[self windowController]];
	[[self contentView] addSubview:textField];
}

- (void) addComboBox : (int) selectedChoice : (const std::vector<std::wstring>&) choices : (NSRect) rect
{
	NSPopUpButton* popup = [[NSPopUpButton alloc] initWithFrame:rect];
	for (const std::wstring& choice : choices) {
		[popup addItemWithTitle:MAS::StdWStringToNSString(choice)];
	}
	[popup selectItemAtIndex:selectedChoice];
	[[self contentView] addSubview:popup];
}

- (void) addSeparator : (NSRect) rect
{
	NSBox* separator = [[NSBox alloc] initWithFrame:rect];
	[separator setBoxType:NSBoxSeparator];
	[[self contentView] addSubview:separator];
}

- (void) addButton : (NSString*) text : (NSRect) rect
{
	NSButton* button = [[NSButton alloc] initWithFrame:rect];
	[button setTitle:text];
	[button setBezelStyle:NSBezelStyleRounded];
	[[self contentView] addSubview:button];
}


@end

@interface ParameterDialogController : NSWindowController<NSWindowDelegate>
{
@private MAS::ParameterDialog* paramDialog;
}
@end

@implementation ParameterDialogController

- (id) init : (MAS::ParameterDialog*) newParamDialog
{
	paramDialog = newParamDialog;
	return self;
}

- (void) controlTextDidChange:(NSNotification*) obj
{
	#pragma unused (obj)
}

- (void) windowDidLoad
{

}

- (void) windowWillClose : (NSNotification*) notification
{
	#pragma unused (notification)
	[NSApp stopModal];
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
			initWithContentRect:windowRect
			styleMask:
				NSWindowStyleMaskTitled |
				NSWindowStyleMaskClosable
			backing:NSBackingStoreBuffered
			defer:NO
	];
	
	ParameterDialogController* paramDialogController = [[[ParameterDialogController alloc] init:this] autorelease];
	[paramDialogController setWindow:paramDialog];
	[paramDialog setDelegate:paramDialogController];
	
	[paramDialog setTitle:StdWStringToNSString (dialogTitle)];
}
	
void ParameterDialog::SetDialogRect (const NUIE::IntRect& rect)
{
	[paramDialog setContentSize:CreateDialogSize (rect.GetWidth (), rect.GetHeight ())];
}

void ParameterDialog::AddParamNameStatic (size_t, const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addStaticControl:StdWStringToNSString(controlText):CreateDialogRect (paramDialog, rect)];
}

void ParameterDialog::AddParamEditText (size_t paramIndex, const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addEditControl:StdWStringToNSString(controlText):CreateDialogRect (paramDialog, rect)];
	(void) paramIndex;
}

void ParameterDialog::AddParamComboBox (size_t paramIndex, int selectedChoice, const std::vector<std::wstring>& choices, const NUIE::IntRect& rect)
{
	[paramDialog addComboBox:selectedChoice:choices:CreateDialogRect (paramDialog, rect)];
	(void) paramIndex;
}

void ParameterDialog::AddHorizontalSeparator (int x, int y, int width)
{
	[paramDialog addSeparator:CreateDialogRect (paramDialog, NUIE::IntRect (x, y, width, 1))];
}

void ParameterDialog::AddCancelButton (const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addButton:StdWStringToNSString(controlText):CreateDialogRect (paramDialog, rect)];
}

void ParameterDialog::AddOkButton (const std::wstring& controlText, const NUIE::IntRect& rect)
{
	[paramDialog addButton:StdWStringToNSString(controlText):CreateDialogRect (paramDialog, rect)];
}

bool ParameterDialog::ShowDialog ()
{
	[paramDialog center];
	[paramDialog makeKeyAndOrderFront:nil];
	[NSApp runModalForWindow:paramDialog];
	return false;
}

std::wstring ParameterDialog::GetEditTextValue (size_t paramIndex)
{
	(void) paramIndex;
	return L"";
}

void ParameterDialog::SetEditTextValue (size_t paramIndex, const std::wstring& text)
{
	(void) paramIndex;
	(void) text;
}

int ParameterDialog::GetComboboxSelectedItem (size_t paramIndex)
{
	(void) paramIndex;
	return 0;
}
	
}
