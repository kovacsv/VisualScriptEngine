#include "TestUtils.hpp"
#include "NE_StringConverter.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_DrawingContext.hpp"

const NE::StringConverter& TestDrawingEnvironment::GetStringConverter ()
{
	return GetDefaultStringConverter ();
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
