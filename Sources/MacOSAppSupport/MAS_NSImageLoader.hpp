#ifndef MAS_NSIMAGELOADER_HPP
#define MAS_NSIMAGELOADER_HPP

#include <unordered_map>

#include "NUIE_DrawingContext.hpp"

#ifdef __cplusplus
#ifdef __OBJC__	
	@class NSImage;
#else
	struct NSImage;
#endif
#endif

namespace MAS
{

class NSImageLoader
{
public:
	NSImageLoader ();
	virtual ~NSImageLoader ();

	NSImage*				LoadNSImage (const NUIE::IconId& iconId);
	void					ClearCache ();

	virtual NSImage*		CreateNSImage (const NUIE::IconId& iconId) = 0;

protected:
	std::unordered_map<NUIE::IconId, NSImage*>	imageCache;
};

using NSImageLoaderPtr = std::shared_ptr<NSImageLoader>;
using NSImageLoaderConstPtr = std::shared_ptr<const NSImageLoader>;

}

#endif
