#include "WAS_SetCaptureHandler.hpp"

namespace WAS
{

SetCaptureHandler::SetCaptureHandler () :
	counter (0)
{
	
}

void SetCaptureHandler::HandleMouseDown (HWND hwnd)
{
	if (counter == 0) {
		SetCapture (hwnd);
	}
	counter += 1;
}

void SetCaptureHandler::HandleMouseUp ()
{
	counter -= 1;
	if (counter <= 0) { // sometimes down and up are not in pair
		ReleaseCapture ();
		counter = 0;
	}
}

}
