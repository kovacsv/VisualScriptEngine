#include "WAS_Direct2DContextBase.hpp"
#include "NE_Debug.hpp"
#include <float.h>

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


Direct2DHandler::Direct2DHandler () :
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

Direct2DHandler::~Direct2DHandler ()
{
	SafeRelease (&direct2DFactory);
	SafeRelease (&directWriteFactory);
}

class Direct2DAntialiasGuard
{
public:
	Direct2DAntialiasGuard (ID2D1RenderTarget* renderTarget, D2D1_ANTIALIAS_MODE newMode) :
		renderTarget (renderTarget),
		oldMode (renderTarget->GetAntialiasMode ())
	{
		renderTarget->SetAntialiasMode (newMode);
	}

	~Direct2DAntialiasGuard ()
	{
		renderTarget->SetAntialiasMode (oldMode);
	}

private:
	ID2D1RenderTarget*		renderTarget;
	D2D1_ANTIALIAS_MODE		oldMode;
};

static ID2D1SolidColorBrush* CreateBrush (ID2D1RenderTarget* renderTarget, const NUIE::Color& color)
{
	ID2D1SolidColorBrush* d2Brush = nullptr;
	renderTarget->CreateSolidColorBrush (D2D1::ColorF (color.GetR () / 255.0f, color.GetG () / 255.0f, color.GetB () / 255.0f), &d2Brush);
	return d2Brush;
}

static ID2D1StrokeStyle* CreateStrokeStyle (ID2D1Factory* direct2DFactory)
{
	static const D2D1_STROKE_STYLE_PROPERTIES properties = {
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_ROUND,
		0.0f,
		D2D1_DASH_STYLE_SOLID,
		0.0f
	};
	ID2D1StrokeStyle* strokeStyle = nullptr;
	direct2DFactory->CreateStrokeStyle (properties, nullptr, 0, &strokeStyle);
	return strokeStyle;
}

static IDWriteTextFormat* CreateTextFormat (IDWriteFactory* directWriteFactory, ID2D1RenderTarget*, const NUIE::Font& font)
{
	IDWriteTextFormat* textFormat = nullptr;
	directWriteFactory->CreateTextFormat (
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

Direct2DContextBase::Direct2DContextBase () :
	Direct2DContextBase (nullptr)
{

}

Direct2DContextBase::Direct2DContextBase (const Direct2DImageLoaderPtr& imageLoader) :
	NUIE::NativeDrawingContext (),
	width (0),
	height (0),
	direct2DHandler (),
	imageLoader (imageLoader),
	renderTarget (nullptr)
{

}

Direct2DContextBase::~Direct2DContextBase ()
{
	SafeRelease (&renderTarget);
}

int Direct2DContextBase::GetWidth () const
{
	return width;
}

int Direct2DContextBase::GetHeight () const
{
	return height;
}

void Direct2DContextBase::BeginDraw ()
{
	renderTarget->BeginDraw ();
	renderTarget->SetAntialiasMode (D2D1_ANTIALIAS_MODE_ALIASED);
	renderTarget->SetTransform (D2D1::Matrix3x2F::Identity ());
	renderTarget->Clear (D2D1::ColorF (D2D1::ColorF::White));
}

void Direct2DContextBase::EndDraw ()
{
	HRESULT result = renderTarget->EndDraw ();
	if ((unsigned int)result == D2DERR_RECREATE_TARGET) {
		InitRenderTarget ();
	}
}

bool Direct2DContextBase::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void Direct2DContextBase::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	Direct2DAntialiasGuard antialiasGuard (renderTarget, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	ID2D1StrokeStyle* d2StrokeStyle = CreateStrokeStyle (direct2DHandler.direct2DFactory);
	renderTarget->DrawLine (CreatePoint (beg), CreatePoint (end), d2Brush, GetPenThickness (pen), d2StrokeStyle);
	SafeRelease (&d2StrokeStyle);
	SafeRelease (&d2Brush);
}

void Direct2DContextBase::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	Direct2DAntialiasGuard antialiasGuard (renderTarget, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

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

void Direct2DContextBase::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	D2D1_RECT_F d2Rect = CreateRect (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	renderTarget->DrawRectangle (&d2Rect, d2Brush, GetPenThickness (pen));
	SafeRelease (&d2Brush);
}

void Direct2DContextBase::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	D2D1_RECT_F d2Rect = CreateRect (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, color);
	renderTarget->FillRectangle (&d2Rect, d2Brush);
	SafeRelease (&d2Brush);
}

void Direct2DContextBase::DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	Direct2DAntialiasGuard antialiasGuard (renderTarget, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	D2D1_ELLIPSE d2Ellipse = CreateEllipse (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, pen.GetColor ());
	renderTarget->DrawEllipse (&d2Ellipse, d2Brush, GetPenThickness (pen));
	SafeRelease (&d2Brush);
}

void Direct2DContextBase::FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color)
{
	Direct2DAntialiasGuard antialiasGuard (renderTarget, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);

	D2D1_ELLIPSE d2Ellipse = CreateEllipse (rect);
	ID2D1SolidColorBrush* d2Brush = CreateBrush (renderTarget, color);
	renderTarget->FillEllipse (&d2Ellipse, d2Brush);
	SafeRelease (&d2Brush);
}

void Direct2DContextBase::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	IDWriteTextFormat* textFormat = CreateTextFormat (direct2DHandler.directWriteFactory, renderTarget, font);

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

NUIE::Size Direct2DContextBase::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	IDWriteTextLayout* textLayout = nullptr;
	IDWriteTextFormat* textFormat = CreateTextFormat (direct2DHandler.directWriteFactory, renderTarget, font);
	direct2DHandler.directWriteFactory->CreateTextLayout (text.c_str (), (UINT32) text.length (), textFormat, FLT_MAX, FLT_MAX, &textLayout);
	DWRITE_TEXT_METRICS metrics;
	textLayout->GetMetrics (&metrics);
	SafeRelease (&textLayout);
	SafeRelease (&textFormat);
	return NUIE::Size (metrics.width * SafetyTextRatio, metrics.height * SafetyTextRatio);
}

bool Direct2DContextBase::CanDrawIcon ()
{
	return imageLoader != nullptr;
}

void Direct2DContextBase::DrawIcon (const NUIE::Rect& rect, const NUIE::IconId& iconId)
{
	if (DBGERROR (imageLoader == nullptr)) {
		return;
	}
	ID2D1Bitmap* bitmap = imageLoader->LoadDirect2DImage (iconId, renderTarget);
	if (DBGVERIFY (bitmap != nullptr)) {
		D2D1_RECT_F d2Rect = CreateRect (rect);
		renderTarget->DrawBitmap (bitmap, d2Rect);
	}
}

}
