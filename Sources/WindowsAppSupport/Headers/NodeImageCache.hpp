#ifndef NODEIMAGECACHE_HPP
#define NODEIMAGECACHE_HPP

#include "DrawingContext.hpp"
#include "NodeId.hpp"
#include "Checksum.hpp"

#include <memory>
#include <unordered_map>

class NodeImageCachedContext : public NUIE::DrawingContext
{
public:
	NodeImageCachedContext (const NE::Checksum& checksum, const NUIE::IntRect& nodeRect);

	bool							IsUpToDate () const;
	bool							IsUpToDate (const NE::Checksum& newChecksum) const;
	void							SetUpToDate (bool newIsUpToDate);

	const NUIE::IntRect&			GetRect () const;
	void							UpdateRect (const NUIE::IntRect& rect);

	virtual void					Resize (int newWidth, int newHeight) override;

	virtual double					GetWidth () const override;
	virtual double					GetHeight () const override;

	virtual void					BeginDraw () override;
	virtual void					EndDraw () override;

	virtual void					DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen) override;
	virtual void					DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen) override;

	virtual void					DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void					FillRect (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void					DrawEllipse (const NUIE::Rect& rect, const NUIE::Pen& pen) override;
	virtual void					FillEllipse (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void					DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& color) override;
	virtual NUIE::Size				MeasureText (const NUIE::Font& font, const std::wstring& text) override;

	virtual DrawingContext&			GetContext () = 0;
	virtual const DrawingContext&	GetContext () const = 0;

private:
	NUIE::Point		GetOffsettedPoint (const NUIE::Point& point);
	NUIE::Rect		GetOffsettedRect (const NUIE::Rect& rect);

	NE::Checksum	checksum;
	NUIE::IntRect	nodeRect;
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

	CachedContextType* GetImageCacheData (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::IntRect& rect)
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
	CachedContextType* Insert (const NE::NodeId& nodeId, const NE::Checksum& checksum, const NUIE::IntRect& rect)
	{
		ImageCacheDataPtr cacheData (new CachedContextType (checksum, rect));
		cache.insert ({ nodeId, cacheData });
		return cacheData.get ();
	}

	std::unordered_map<NE::NodeId, ImageCacheDataPtr> cache;
};

#endif
