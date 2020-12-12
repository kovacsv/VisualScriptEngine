#include "WAS_Direct2DOffscreenContext.hpp"
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

Direct2DOffscreenContext::Direct2DOffscreenContext () :
	Direct2DOffscreenContext (nullptr)
{

}

Direct2DOffscreenContext::Direct2DOffscreenContext (const Direct2DImageLoaderPtr& imageLoader) :
	Direct2DContextBase (imageLoader),
	memoryDC (NULL),
	memoryBitmap (NULL)
{

}

Direct2DOffscreenContext::~Direct2DOffscreenContext ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
}

void Direct2DOffscreenContext::Init (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;

	InitRenderTarget ();
}

void Direct2DOffscreenContext::BlitToWindow (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);
	BlitToContext (hdc);
	EndPaint (hwnd, &ps);
}

void Direct2DOffscreenContext::BlitToContext (void* nativeContext)
{
	HDC hdc = (HDC) nativeContext;
	BitBlt (hdc, 0, 0, width, height, memoryDC, 0, 0, SRCCOPY);
}

void Direct2DOffscreenContext::Resize (int newWidth, int newHeight)
{
	if (newWidth == 0 || newHeight == 0 || renderTarget == nullptr) {
		return;
	}

	width = newWidth;
	height = newHeight;
	InitOffscreenContext ();
}

void Direct2DOffscreenContext::InitRenderTarget ()
{
	SafeRelease (&renderTarget);
	if (imageLoader != nullptr) {
		imageLoader->ClearCache ();
	}

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties = D2D1::RenderTargetProperties (
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat (DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
		96.0f, 96.0f,
		D2D1_RENDER_TARGET_USAGE_NONE,
		D2D1_FEATURE_LEVEL_DEFAULT
	);

	ID2D1DCRenderTarget** dcRenderTarget = (ID2D1DCRenderTarget**) &renderTarget;
	direct2DHandler.direct2DFactory->CreateDCRenderTarget (&renderTargetProperties, dcRenderTarget);
	DBGASSERT (renderTarget != nullptr);

	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
	SelectObject (memoryDC, memoryBitmap);

	RECT clientRect = { 0, 0, width, height };
	(*dcRenderTarget)->BindDC (memoryDC, &clientRect);

	InitOffscreenContext ();
}

void Direct2DOffscreenContext::InitOffscreenContext ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
	SelectObject (memoryDC, memoryBitmap);

	RECT clientRect = { 0, 0, width, height };
	ID2D1DCRenderTarget* dcRenderTarget = (ID2D1DCRenderTarget*) renderTarget;
	dcRenderTarget->BindDC (memoryDC, &clientRect);
}

}
