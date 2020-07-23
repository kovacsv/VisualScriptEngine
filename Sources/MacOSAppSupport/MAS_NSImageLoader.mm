#include "MAS_NSImageLoader.hpp"
#include "NE_Debug.hpp"
#include "MAS_IncludeCocoaHeaders.hpp"

namespace MAS
{

NSImageLoader::NSImageLoader () :
	imageCache ()
{
}

NSImageLoader::~NSImageLoader ()
{
	ClearCache ();
}

NSImage* NSImageLoader::LoadNSImage (const NUIE::IconId& iconId)
{
	auto found = imageCache.find (iconId);
	if (found != imageCache.end ()) {
		return found->second;
	}
	@autoreleasepool {
		@try {
			NSImage* image = CreateNSImage (iconId);
			imageCache.insert ({ iconId, image });
			return image;
		} @catch (NSException*) {
		}
	}
	return nullptr;
}

void NSImageLoader::ClearCache ()
{
	imageCache.clear ();
}

}
