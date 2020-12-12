#include "WAS_Direct2DContext.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

static const float SafetyTextRatio = 1.05f;

template<class Interface>
static void SafeRelease (Interface** interfaceToRelease)
{
	if (*interfaceToRelease != NULL) {
		(*interfaceToRelease)->Release ();
		(*interfaceToRelease) = NULL;
	}
}

Direct2DContext::Direct2DContext () :
	Direct2DContext (nullptr)
{

}

Direct2DContext::Direct2DContext (const Direct2DImageLoaderPtr& imageLoader) :
	Direct2DContextBase (imageLoader),
	hwnd (NULL)
{

}

Direct2DContext::~Direct2DContext ()
{
	
}

void Direct2DContext::Init (void* nativeHandle)
{
	hwnd = (HWND) nativeHandle;

	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;

	InitRenderTarget ();
}

void Direct2DContext::BlitToWindow (void*)
{

}

void Direct2DContext::BlitToContext (void*)
{

}

void Direct2DContext::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
	if (width > 0 && height > 0) {
		D2D1_SIZE_U size = D2D1::SizeU (width, height);
		ID2D1HwndRenderTarget* hwndRenderTarget = (ID2D1HwndRenderTarget*) renderTarget;
		hwndRenderTarget->Resize (size);
	}
}

void Direct2DContext::InitRenderTarget ()
{
	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;

	D2D1_SIZE_U size = D2D1::SizeU (width, height);
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties ();
	D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRenderTargetProperties = D2D1::HwndRenderTargetProperties (hwnd, size);

	SafeRelease (&renderTarget);
	if (imageLoader != nullptr) {
		imageLoader->ClearCache ();
	}

	ID2D1HwndRenderTarget** hwndRenderTarget = (ID2D1HwndRenderTarget**) (&renderTarget);
	direct2DHandler.direct2DFactory->CreateHwndRenderTarget (renderTargetProperties, hwndRenderTargetProperties, hwndRenderTarget);
	renderTarget->SetDpi (96.0f, 96.0f);
	DBGASSERT (renderTarget != nullptr);
}

}
