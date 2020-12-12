#ifndef WAS_DIRECT2DOFFSCREENCONTEXT_HPP
#define WAS_DIRECT2DOFFSCREENCONTEXT_HPP

#include "WAS_Direct2DContextBase.hpp"
#include "WAS_OffscreenBitmap.hpp"

namespace WAS
{

class Direct2DOffscreenContext : public Direct2DContextBase
{
public:
	Direct2DOffscreenContext ();
	Direct2DOffscreenContext (const Direct2DImageLoaderPtr& imageLoader);
	Direct2DOffscreenContext (const Direct2DOffscreenContext& rhs) = delete;
	virtual ~Direct2DOffscreenContext ();

	virtual void				Init (void* nativeHandle) override;
	virtual void				BlitToWindow (void* nativeHandle) override;
	virtual void				BlitToContext (void* nativeContext) override;
	virtual void				Resize (int newWidth, int newHeight) override;

private:
	virtual void				InitRenderTarget () override;
	void						InitOffscreenContext ();

	OffscreenBitmap				bitmap;
};

}

#endif
