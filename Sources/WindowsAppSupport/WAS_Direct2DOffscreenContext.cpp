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

Direct2DOffscreenContext::Direct2DOffscreenContext (const Direct2DImageLoaderPtr& imageLoader) :
	NUIE::NativeDrawingContext (),
	direct2DHandler (),
	width (0),
	height (0),
	imageLoader (imageLoader),
	renderTarget (nullptr),
	memoryDC (NULL),
	memoryBitmap (NULL)
{

}

Direct2DOffscreenContext::~Direct2DOffscreenContext ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);
	SafeRelease (&renderTarget);
}

void Direct2DOffscreenContext::Init (void* nativeHandle)
{
	HWND hwnd = (HWND) nativeHandle;

	RECT clientRect;
	GetClientRect (hwnd, &clientRect);
	width = clientRect.right - clientRect.left;
	height = clientRect.bottom - clientRect.top;
	
	CreateRenderTarget ();
	CreateOffscreenContext ();
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
	CreateOffscreenContext ();
}

int Direct2DOffscreenContext::GetWidth () const
{
	return width;
}

int Direct2DOffscreenContext::GetHeight () const
{
	return height;
}

void Direct2DOffscreenContext::BeginDraw ()
{
	renderTarget->BeginDraw ();
	renderTarget->SetAntialiasMode (D2D1_ANTIALIAS_MODE_ALIASED);
	renderTarget->SetTransform (D2D1::Matrix3x2F::Identity ());
	renderTarget->Clear (D2D1::ColorF (D2D1::ColorF::White));
}

void Direct2DOffscreenContext::EndDraw ()
{
	HRESULT result = renderTarget->EndDraw ();
	if (result == D2DERR_RECREATE_TARGET) {
		CreateRenderTarget ();
		CreateOffscreenContext ();
	}
}

bool Direct2DOffscreenContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void Direct2DOffscreenContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	D2D1DrawLine (renderTarget, beg, end, pen);
}

void Direct2DOffscreenContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	D2D1DrawBezier (direct2DHandler.direct2DFactory, renderTarget, p1, p2, p3, p4, pen);
}

void Direct2DOffscreenContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1DrawRect (renderTarget, rect, pen);
}

void Direct2DOffscreenContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1FillRect (renderTarget, rect, color);
}

void Direct2DOffscreenContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1DrawEllipse (renderTarget, rect, pen);
}

void Direct2DOffscreenContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1FillEllipse (renderTarget, rect, color);
}

void Direct2DOffscreenContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	D2D1DrawFormattedText (direct2DHandler.directWriteFactory, renderTarget, rect, font, text, hAnchor, vAnchor, textColor);
}

NUIE::Size Direct2DOffscreenContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	return D2D1MeasureText (direct2DHandler.directWriteFactory, renderTarget, font, text);
}

bool Direct2DOffscreenContext::CanDrawIcon ()
{
	return true;
}

void Direct2DOffscreenContext::DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId)
{
	if (DBGERROR (imageLoader == nullptr)) {
		return;
	}
	D2D1DrawIcon (renderTarget, &*imageLoader, rect, iconId);
}

void Direct2DOffscreenContext::CreateRenderTarget ()
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
	direct2DHandler.direct2DFactory->CreateDCRenderTarget (&renderTargetProperties, &renderTarget);
	DBGASSERT (renderTarget != nullptr);
}

void Direct2DOffscreenContext::CreateOffscreenContext ()
{
	DeleteObject (memoryBitmap);
	DeleteDC (memoryDC);

	HDC hdc = GetDC (NULL);
	memoryDC = CreateCompatibleDC (hdc);
	memoryBitmap = CreateCompatibleBitmap (hdc, width, height);
	SelectObject (memoryDC, memoryBitmap);

	RECT clientRect = { 0, 0, width, height };
	renderTarget->BindDC (memoryDC, &clientRect);
}

}
