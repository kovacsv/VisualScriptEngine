#ifndef MAS_NSVIEWOFFSCREENCONTEXT_HPP
#define MAS_NSVIEWOFFSCREENCONTEXT_HPP

#include "MAS_NSViewContextBase.hpp"

namespace MAS
{

class NSViewOffscreenContext : public NSViewContextBase
{
public:
	enum class Orientation
	{
		Normal,
		FlippedVertically
	};
	
	NSViewOffscreenContext (Orientation orientation);
	NSViewOffscreenContext (Orientation orientation, const NSImageLoaderPtr& imageLoader);
	NSViewOffscreenContext (const NSViewOffscreenContext& rhs) = delete;
	virtual ~NSViewOffscreenContext ();

	virtual void				Init (void* nativeHandle) override;
	virtual void				BlitToWindow (void* nativeHandle) override;
	virtual void				BlitToContext (void* nativeContext) override;

	virtual void				Resize (int newWidth, int newHeight) override;

	virtual void				BeginDraw () override;
	virtual void				EndDraw () override;

private:
	Orientation					orientation;
	NSImage*					image;
};

}

#endif
