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

class Direct2DHandler
{
public:
	Direct2DHandler () :
		direct2DFactory (nullptr),
		directWriteFactory (nullptr)
	{
		HRESULT direct2DFactoryResult = D2D1CreateFactory (D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct2DFactory);
		if (!SUCCEEDED (direct2DFactoryResult)) {
			DBGBREAK ();
			return;
		}

		HRESULT directWriteFactoryResult = DWriteCreateFactory (DWRITE_FACTORY_TYPE_ISOLATED, __uuidof (directWriteFactory), reinterpret_cast<IUnknown**> (&directWriteFactory));
		if (!SUCCEEDED (directWriteFactoryResult)) {
			DBGBREAK ();
			return;
		}
	}

	~Direct2DHandler ()
	{
		SafeRelease (&direct2DFactory);
		SafeRelease (&directWriteFactory);
	}

	ID2D1Factory*		direct2DFactory;
	IDWriteFactory*		directWriteFactory;
};

static Direct2DHandler direct2DHandler;

static ID2D1SolidColorBrush* CreateBrush (ID2D1RenderTarget* renderTarget, const NUIE::Color& color)
{
	ID2D1SolidColorBrush* d2Brush = nullptr;
	renderTarget->CreateSolidColorBrush (D2D1::ColorF (color.GetR () / 255.0f, color.GetG () / 255.0f, color.GetB () / 255.0f), &d2Brush);
	return d2Brush;
}

static IDWriteTextFormat* CreateTextFormat (ID2D1RenderTarget*, const NUIE::Font& font)
{
	IDWriteTextFormat* textFormat = nullptr;
	direct2DHandler.directWriteFactory->CreateTextFormat (
		font.GetFamily ().c_str (),
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(float) font.GetSize (),
		L"",
		&textFormat
	);
	return textFormat;
}

static D2D1_POINT_2F CreatePoint (const NUIE::Point& point)
{
	return D2D1::Point2F ((float) point.GetX (), (float) point.GetY ());
}

static D2D1_RECT_F CreateRect (const NUIE::Rect& rect)
{
	return D2D1::RectF ((float) rect.GetLeft (), (float) rect.GetTop (), (float) rect.GetRight (), (float) rect.GetBottom ());
}

static D2D1_ELLIPSE CreateEllipse (const NUIE::Rect& rect)
{
	return D2D1::Ellipse (CreatePoint (rect.GetCenter ()), (float) rect.GetWidth () / 2.0f, (float) rect.GetHeight () / 2.0f);
}

static float GetPenThickness (const NUIE::Pen& pen)
{
	float penThickness = (float) pen.GetThickness ();
	if (penThickness < 1.0f) {
		penThickness = 1.0f;
	}
	return std::floor (penThickness);
}

Direct2DContext::Direct2DContext () :
	NUIE::NativeDrawingContext (),
	hwnd (NULL),
	width (0),
	height (0),
	renderTarget (nullptr)
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

void Direct2DContext::BeginDraw (Phase phase)
{
	if (phase == Phase::Draw) {
		renderTarget->BeginDraw ();
		renderTarget->SetAntialiasMode (D2D1_ANTIALIAS_MODE_ALIASED);
		renderTarget->SetTransform (D2D1::Matrix3x2F::Identity ());
		renderTarget->Clear (D2D1::ColorF (D2D1::ColorF::White));
	} else if (phase == Phase::DrawConnections) {
		renderTarget->SetAntialiasMode (D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	}
}

void Direct2DContext::EndDraw (Phase phase)
{
	if (phase == Phase::Draw) {
		HRESULT result = renderTarget->EndDraw ();
		if (result == D2DERR_RECREATE_TARGET) {
			CreateRenderTarget ();
		}
	} else if (phase == Phase::DrawConnections) {
		renderTarget->SetAntialiasMode (D2D1_ANTIALIAS_MODE_ALIASED);
	}
}

void Direct2DContext::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	renderTarget->DrawLine (CreatePoint (beg), CreatePoint (end), d2Brush, GetPenThickness (pen));
	SafeRelease (&d2Brush);
}

void Direct2DContext::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	ID2D1PathGeometry* path = nullptr;
	direct2DHandler.direct2DFactory->CreatePathGeometry (&path);

	ID2D1GeometrySink *sink = nullptr;
	path->Open (&sink);

	sink->BeginFigure (CreatePoint (p1), D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddBezier (D2D1::BezierSegment (CreatePoint (p2), CreatePoint (p3), CreatePoint (p4)));
	sink->EndFigure (D2D1_FIGURE_END_OPEN);
	sink->Close ();

	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	renderTarget->DrawGeometry (path, d2Brush, GetPenThickness (pen));
	SafeRelease (&d2Brush);

	SafeRelease (&sink);
	SafeRelease (&path);
}

void Direct2DContext::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1_RECT_F d2Rect = CreateRect (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	renderTarget->DrawRectangle (&d2Rect, d2Brush, GetPenThickness (pen));
	SafeRelease (&d2Brush);
}

void Direct2DContext::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1_RECT_F d2Rect = CreateRect (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, color);
	renderTarget->FillRectangle (&d2Rect, d2Brush);
	SafeRelease (&d2Brush);
}

void Direct2DContext::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1_ELLIPSE d2Ellipse = CreateEllipse (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	renderTarget->DrawEllipse (&d2Ellipse, d2Brush, GetPenThickness (pen));
	SafeRelease (&d2Brush);
}

void Direct2DContext::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1_ELLIPSE d2Ellipse = CreateEllipse (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, color);
	renderTarget->FillEllipse (&d2Ellipse, d2Brush);
	SafeRelease (&d2Brush);
}

void Direct2DContext::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	IDWriteTextFormat* textFormat = CreateTextFormat (renderTarget, font);

	IDWriteTextLayout* textLayout = nullptr;
	direct2DHandler.directWriteFactory->CreateTextLayout (text.c_str (), (UINT32) text.length (), textFormat, (float) rect.GetWidth (), (float) rect.GetHeight (), &textLayout);

	switch (hAnchor) {
		case NUIE::HorizontalAnchor::Left:
			textLayout->SetTextAlignment (DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case NUIE::HorizontalAnchor::Center:
			textLayout->SetTextAlignment (DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		case NUIE::HorizontalAnchor::Right:
			textLayout->SetTextAlignment (DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
	}

	switch (vAnchor) {
		case NUIE::VerticalAnchor::Top:
			textLayout->SetParagraphAlignment (DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			break;
		case NUIE::VerticalAnchor::Center:
			textLayout->SetParagraphAlignment (DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			break;
		case NUIE::VerticalAnchor::Bottom:
			textLayout->SetParagraphAlignment (DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			break;
	}

	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, textColor);
	renderTarget->DrawTextLayout (CreatePoint (rect.GetTopLeft ()), textLayout, d2Brush);
	SafeRelease (&d2Brush);

	SafeRelease (&textLayout);
	SafeRelease (&textFormat);
}

NUIE::Size Direct2DContext::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	IDWriteTextLayout* textLayout = nullptr;
	IDWriteTextFormat* textFormat = CreateTextFormat (renderTarget, font);
	direct2DHandler.directWriteFactory->CreateTextLayout (text.c_str (), (UINT32) text.length (), textFormat, 1000.0, 1000.0, &textLayout);
	DWRITE_TEXT_METRICS metrics;
	textLayout->GetMetrics (&metrics);
	SafeRelease (&textLayout);
	SafeRelease (&textFormat);
	return NUIE::Size (metrics.width * SafetyTextRatio, metrics.height * SafetyTextRatio);
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
	direct2DHandler.direct2DFactory->CreateHwndRenderTarget (renderTargetProperties, hwndRenderTargetProperties, &renderTarget);
	renderTarget->SetDpi (96.0f, 96.0f);
	DBGASSERT (renderTarget != nullptr);
}

}
