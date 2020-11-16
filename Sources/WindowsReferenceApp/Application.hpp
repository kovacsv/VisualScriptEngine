#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "AppUIEnvironment.hpp"

#include <windows.h>

class Application
{
public:
	Application ();

	void	Init (HWND hwnd);

	void	New (HWND hwnd);
	void	Open (HWND hwnd);
	void	Save (HWND hwnd);
	bool	Close (HWND hwnd);
	void	ExecuteCommand (NUIE::CommandCode command);
	void	OnResize (int x, int y, int width, int height);
	void	OnClipboardChanged ();

private:
	void	InitFileMenu (HWND hwnd);
	void	InitToolbar (HWND hwnd);
	void	AddToolbarItem (WORD imageResourceId, UINT commandId);

	AppUIEnvironment	uiEnvironment;
	NUIE::NodeEditor	nodeEditor;
	WAS::FileMenu		fileMenu;
	WAS::Toolbar		toolbar;
};

#endif
