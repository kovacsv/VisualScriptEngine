#include "TestUtils.hpp"
#include "NE_StringSettings.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_DrawingContext.hpp"

const NE::StringSettings& TestDrawingEnvironment::GetStringSettings ()
{
	return GetDefaultStringSettings ();
}

const SkinParams& TestDrawingEnvironment::GetSkinParams ()
{
	return GetDefaultSkinParams ();
}

DrawingContext& TestDrawingEnvironment::GetDrawingContext ()
{
	static NullDrawingContext context;
	return context;
}

double TestDrawingEnvironment::GetWindowScale ()
{
	return 1.0;
}
