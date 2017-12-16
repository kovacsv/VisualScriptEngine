#ifndef BITMAPCONTEXTGDI_HPP
#define BITMAPCONTEXTGDI_HPP

#include <windows.h>
#include <gdiplus.h>
#include <unordered_map>

#include "HandleCache.hpp"
#include "DrawingContext.hpp"
#include "Drawing.hpp"

class PenCacheKey
{
public:
	PenCacheKey ();
	PenCacheKey (const NUIE::Pen& pen);

	HANDLE	CreateHandle ();
	bool	operator== (const PenCacheKey& rhs) const;

	int				thickness;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
};

class BrushCacheKey
{
public:
	BrushCacheKey ();
	BrushCacheKey (const NUIE::Color& color);

	HANDLE	CreateHandle ();
	bool	operator== (const BrushCacheKey& rhs) const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class FontCacheKey
{
public:
	FontCacheKey ();
	FontCacheKey (const NUIE::Font& font);

	HANDLE	CreateHandle ();
	bool	operator== (const FontCacheKey& rhs) const;

	std::wstring	family;
	int				size;
};

namespace std
{
	template <>
	struct hash<PenCacheKey>
	{
		size_t operator() (const PenCacheKey& key) const noexcept
		{
			return key.thickness + 12289 * key.r + 24593 * key.g + 49157 * key.b;
		}
	};

	template <>
	struct hash<BrushCacheKey>
	{
		size_t operator() (const BrushCacheKey& key) const noexcept
		{
			return key.r + 24593 * key.g + 49157 * key.b;
		}
	};

	template <>
	struct hash<FontCacheKey>
	{
		size_t operator() (const FontCacheKey& key) const noexcept
		{
			return std::hash<std::wstring> {} (key.family) + 49157 * std::hash<int> {} (key.size);
		}
	};
}

class BitmapContextGdi : public NUIE::DrawingContext
{
public:
	BitmapContextGdi (int width, int height);
	BitmapContextGdi (const BitmapContextGdi& rhs) = delete;
	virtual ~BitmapContextGdi ();

	virtual void				Resize (int newWidth, int newHeight) override;

	virtual double				GetWidth () const override;
	virtual double				GetHeight () const override;

	virtual void				BeginDraw () override;
	virtual void				EndDraw () override;

	virtual void				DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen) override;
	virtual void				DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen) override;

	virtual void				DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void				FillRect (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void				DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void				FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void				DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& backgroundColor, const NUIE::Color& textColor) override;
	virtual NUIE::Size			MeasureText (const NUIE::Font& font, const std::wstring& text) override;

	void						StretchToContext (BitmapContextGdi& targetContext, int x, int y, int targetWidth, int targetHeight);
	void						DrawToContext (BitmapContextGdi& targetContext, int x, int y);

	void						DrawToHDC (HDC targetDC, int x, int y);
	void						StretchToHDC (HDC targetDC, int x, int y, int targetWidth, int targetHeight);

private:
	POINT						CreatePoint (const NUIE::Point& point) const;
	RECT						CreateRect (const NUIE::Rect& rect) const;
	RECT						CreateRect (const NUIE::Rect& rect, const NUIE::Pen& pen) const;

	int							width;
	int							height;
	HDC							memoryDC;
	HBITMAP						memoryBitmap;

	HandleCache<PenCacheKey>	penCache;
	HandleCache<BrushCacheKey>	brushCache;
	HandleCache<FontCacheKey>	fontCache;
};

#endif
