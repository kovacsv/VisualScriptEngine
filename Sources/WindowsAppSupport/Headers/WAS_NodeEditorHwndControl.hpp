#ifndef NODEEDITORHWNDCONTROL_HPP
#define NODEEDITORHWNDCONTROL_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_BitmapContextGdi.hpp"

namespace WAS
{

class NodeEditorHwndControl
{
public:
	NodeEditorHwndControl ();
	~NodeEditorHwndControl ();

	bool					Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle, int x, int y, int width, int height);

	NUIE::NodeEditor*		GetNodeEditor ();
	NUIE::DrawingContext&	GetDrawingContext ();

	HWND					GetWindowHandle ();
	void					Resize (int x, int y, int width, int height);
	void					Invalidate ();
	void					Draw ();

private:
	NUIE::NodeEditor*		nodeEditor;
	WAS::BitmapContextGdi	bitmapContext;
	HWND					hwnd;
};

}

#endif
