#ifndef WAS_BITMAPCONTEXTGDI_HPP
#define WAS_BITMAPCONTEXTGDI_HPP

#include <unordered_map>

#include "NUIE_DrawingContext.hpp"
#include "NUIE_Drawing.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"
#include "WAS_DrawingCacheKeys.hpp"

namespace WAS
{

template <typename KeyType>
HANDLE CreateHandle (const KeyType& key);

template <typename KeyType>
class HandleCache
{
public:
	HandleCache ()
	{
	
	}

	~HandleCache ()
	{
		for (auto& it: cache) {
			DeleteObject (it.second);
		}	
	}

	HANDLE Get (const KeyType& key)
	{
		KeyType cacheKey (key);
		auto found = cache.find (cacheKey);
		if (found != cache.end ()) {
			return found->second;
		}
		HANDLE keyHandle = CreateHandle (cacheKey);
		cache.insert ({ cacheKey, keyHandle });
		return keyHandle;
	}

private:
	std::unordered_map<KeyType, HANDLE> cache;
};

class BitmapContextGdi : public NUIE::NativeDrawingContext
{
public:
	BitmapContextGdi ();
	BitmapContextGdi (const BitmapContextGdi& rhs) = delete;
	virtual ~BitmapContextGdi ();

	virtual void				Init (void* nativeHandle) override;
	virtual void				BlitToWindow (void* nativeHandle) override;
	virtual void				BlitToContext (void* nativeContext) override;

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

	virtual void				DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor) override;
	virtual NUIE::Size			MeasureText (const NUIE::Font& font, const std::wstring& text) override;

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

}

#endif
