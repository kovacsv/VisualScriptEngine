#ifndef WAS_DIRECT2DCONTEXT_HPP
#define WAS_DIRECT2DCONTEXT_HPP

#include "WAS_Direct2DContextBase.hpp"

namespace WAS
{

class Direct2DContext : public Direct2DContextBase
{
public:
	Direct2DContext ();
	Direct2DContext (const Direct2DImageLoaderPtr& imageLoader);
	Direct2DContext (const Direct2DContext& rhs) = delete;
	virtual ~Direct2DContext ();

	virtual void				Init (void* nativeHandle) override;
	virtual void				BlitToWindow (void* nativeHandle) override;
	virtual void				BlitToContext (void* nativeContext) override;
	virtual void				Resize (int newWidth, int newHeight) override;

private:
	virtual void				InitRenderTarget () override;
	
	HWND						hwnd;
};

}

#endif
