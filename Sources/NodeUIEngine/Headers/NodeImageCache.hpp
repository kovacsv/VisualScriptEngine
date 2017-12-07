#ifndef NODEIMAGECACHE_HPP
#define NODEIMAGECACHE_HPP

#include "DrawingContext.hpp"
#include "NodeId.hpp"
#include "Checksum.hpp"

#include <memory>
#include <unordered_map>

namespace NUIE
{

class NodeImageCachedContext : public DrawingContext
{
public:
	NodeImageCachedContext (const NE::Checksum& checksum, const IntRect& nodeRect);

	bool							IsUpToDate () const;
	bool							IsUpToDate (const NE::Checksum& newChecksum) const;
	void							SetUpToDate (bool newIsUpToDate);

	const IntRect&					GetRect () const;
	void							UpdateRect (const IntRect& rect);

	virtual void					Resize (int newWidth, int newHeight) override;

	virtual double					GetWidth () const override;
	virtual double					GetHeight () const override;

	virtual void					BeginDraw () override;
	virtual void					EndDraw () override;

	virtual void					DrawLine (const Point& beg, const Point& end, const Pen& pen) override;
	virtual void					DrawBezier (const Point& p1, const Point& p2, const Point& p3, const Point& p4, const Pen& pen) override;

	virtual void					DrawRect (const Rect& rect, const Pen& pen) override;
	virtual void					FillRect (const Rect& rect, const Color& color) override;

	virtual void					DrawEllipse (const Rect& rect, const Pen& pen) override;
	virtual void					FillEllipse (const Rect& rect, const Color& color) override;

	virtual void					DrawFormattedText (const Rect& rect, const Font& font, const std::wstring& text, HorizontalAnchor hAnchor, VerticalAnchor vAnchor, const Color& color) override;
	virtual Size					MeasureText (const Font& font, const std::wstring& text) override;

	virtual DrawingContext&			GetContext () = 0;
	virtual const DrawingContext&	GetContext () const = 0;

private:
	Point			GetOffsettedPoint (const Point& point);
	Rect			GetOffsettedRect (const Rect& rect);

	NE::Checksum	checksum;
	IntRect			nodeRect;
	bool			isUpToDate;
};

template <typename CachedContextType>
class NodeImageCache
{
public:
	using ImageCacheDataPtr = std::shared_ptr<CachedContextType>;

	NodeImageCache ()
	{

	}

	~NodeImageCache ()
	{

	}

	CachedContextType* GetImageCacheData (const NE::NodeId& nodeId, const NE::Checksum& checksum, const IntRect& rect)
	{
		auto found = cache.find (nodeId);
		if (found != cache.end ()) {
			ImageCacheDataPtr& cacheData = found->second;
			if (cacheData->IsUpToDate (checksum)) {
				cacheData->UpdateRect (rect);
				return cacheData.get ();
			} else {
				cache.erase (found);
			}
		}
		return Insert (nodeId, checksum, rect);
	}

private:
	CachedContextType* Insert (const NE::NodeId& nodeId, const NE::Checksum& checksum, const IntRect& rect)
	{
		ImageCacheDataPtr cacheData (new CachedContextType (checksum, rect));
		cache.insert ({ nodeId, cacheData });
		return cacheData.get ();
	}

	std::unordered_map<NE::NodeId, ImageCacheDataPtr> cache;
};

}

#endif
