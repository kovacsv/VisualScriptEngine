#ifndef DIRECT2DCONTEXT_HPP
#define DIRECT2DCONTEXT_HPP

#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include <unordered_map>

#include "NUIE_DrawingContext.hpp"
#include "WAS_DrawingCacheKeys.hpp"

namespace WAS
{

template<class Interface>
inline void SafeRelease (Interface** interfaceToRelease)
{
	if (*interfaceToRelease != NULL) {
		(*interfaceToRelease)->Release ();
		(*interfaceToRelease) = NULL;
	}
}

template <typename KeyType, typename ValueType>
ValueType* CreateValue (ID2D1RenderTarget* renderTarget, const KeyType& key);

template <typename KeyType, typename ValueType>
class ObjectCache
{
public:
	ObjectCache ()
	{
	
	}

	~ObjectCache ()
	{
		for (auto it : cache) {
			SafeRelease (&it.second);
		}	
	}

	ValueType* Get (ID2D1RenderTarget* renderTarget, const KeyType& key)
	{
		KeyType cacheKey (key);
		auto found = cache.find (cacheKey);
		if (found != cache.end ()) {
			return found->second;
		}
		ValueType* value = CreateValue<KeyType, ValueType> (renderTarget, key);
		cache.insert ({ cacheKey, value });
		return value;
	}

private:
	std::unordered_map<KeyType, ValueType*> cache;
};

class Direct2DContext : public NUIE::NativeDrawingContext
{
public:
	Direct2DContext ();
	Direct2DContext (const Direct2DContext& rhs) = delete;
	virtual ~Direct2DContext ();

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
	int							width;
	int							height;
	ID2D1HwndRenderTarget*		renderTarget;

	ObjectCache<BrushCacheKey, ID2D1SolidColorBrush>	brushCache;
	ObjectCache<FontCacheKey, IDWriteTextFormat>		textFormatCache;
};

}

#endif
