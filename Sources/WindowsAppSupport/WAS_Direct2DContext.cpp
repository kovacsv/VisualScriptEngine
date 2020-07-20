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

Direct2DContext::Direct2DContext (const Direct2DImageLoaderPtr& imageLoader) :
	NUIE::NativeDrawingContext (),
	direct2DHandler (),
	width (0),
	height (0),
	imageLoader (imageLoader),
	renderTarget (nullptr),
	hwnd (NULL)
{

}

Direct2DContext::~Direct2DContext ()
{
	SafeRelease (&renderTarget);
}

void Direct2DContext::Init (void* nativeHandle)
{
	hwnd = (HWND) nativeHandle;
	CreateRenderTarget ();
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
		renderTarget->Resize (size);
	}
}

double Direct2DContext::GetWidth () const
{
	return width;
}

double Direct2DContext::GetHeight () const
{
	return height;
}

void Direct2DContext::BeginDraw ()
{
	renderTarget->BeginDraw ();
	renderTarget->SetAntialiasMode (D2D1_ANTIALIAS_MODE_ALIASED);
	renderTarget->SetTransform (D2D1::Matrix3x2F::Identity ());
	renderTarget->Clear (D2D1::ColorF (D2D1::ColorF::White));
}

void Direct2DContext::EndDraw ()
{
	HRESULT result = renderTarget->EndDraw ();
	if (result == D2DERR_RECREATE_TARGET) {
		CreateRenderTarget ();
	}
}

bool Direct2DContext::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void Direct2DContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	D2D1DrawLine (renderTarget, beg, end, pen);
}

void Direct2DContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	D2D1DrawBezier (direct2DHandler.direct2DFactory, renderTarget, p1, p2, p3, p4, pen);
}

void Direct2DContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1DrawRect (renderTarget, rect, pen);
}

void Direct2DContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1FillRect (renderTarget, rect, color);
}

void Direct2DContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1DrawEllipse (renderTarget, rect, pen);
}

void Direct2DContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1FillEllipse (renderTarget, rect, color);
}

void Direct2DContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	D2D1DrawFormattedText (direct2DHandler.directWriteFactory, renderTarget, rect, font, text, hAnchor, vAnchor, textColor);
}

NUIE::Size Direct2DContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	return D2D1MeasureText (direct2DHandler.directWriteFactory, renderTarget, font, text);
}

bool Direct2DContext::CanDrawIcon ()
{
	return true;
}

void Direct2DContext::DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId)
{
	if (DBGERROR (imageLoader == nullptr)) {
		return;
	}
	D2D1DrawIcon (renderTarget, &*imageLoader, rect, iconId);
}

void Direct2DContext::CreateRenderTarget ()
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

	direct2DHandler.direct2DFactory->CreateHwndRenderTarget (renderTargetProperties, hwndRenderTargetProperties, &renderTarget);
	renderTarget->SetDpi (96.0f, 96.0f);
	DBGASSERT (renderTarget != nullptr);
}

}
