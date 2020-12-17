#ifndef MAS_NSVIEWCONTEXT_HPP
#define MAS_NSVIEWCONTEXT_HPP

#include "MAS_NSViewContextBase.hpp"

namespace MAS
{

class NSViewContext : public NSViewContextBase
{
public:
	NSViewContext ();
	NSViewContext (const NSImageLoaderPtr& imageLoader);
	NSViewContext (const NSViewContext& rhs) = delete;
	virtual ~NSViewContext ();

	virtual void				Init (void* nativeHandle) override;
	virtual void				BlitToWindow (void* nativeHandle) override;
	virtual void				BlitToContext (void* nativeContext) override;

	virtual void				Resize (int newWidth, int newHeight) override;

private:
	NSView*						nsView;
};

}

#endif
