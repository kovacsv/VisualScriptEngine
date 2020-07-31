#include "NUIE_DrawingCacheKeys.hpp"

namespace NUIE
{

PenCacheKey::PenCacheKey () :
	thickness (0),
	r (0),
	g (0),
	b (0)
{

}

PenCacheKey::PenCacheKey (const NUIE::Pen& pen) :
	thickness ((int) pen.GetThickness ()),
	r (pen.GetColor ().GetR ()),
	g (pen.GetColor ().GetG ()),
	b (pen.GetColor ().GetB ())
{

}

bool PenCacheKey::operator== (const PenCacheKey& rhs) const
{
	return thickness == rhs.thickness && r == rhs.r && g == rhs.g && b == rhs.b;
}

bool PenCacheKey::operator!= (const PenCacheKey& rhs) const
{
	return !operator== (rhs);
}

ColorCacheKey::ColorCacheKey () :
	r (0),
	g (0),
	b (0)
{

}

ColorCacheKey::ColorCacheKey (const NUIE::Color& color) :
	r (color.GetR ()),
	g (color.GetG ()),
	b (color.GetB ())
{

}

bool ColorCacheKey::operator== (const ColorCacheKey& rhs) const
{
	return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool ColorCacheKey::operator != (const ColorCacheKey& rhs) const
{
	return !operator== (rhs);
}

FontCacheKey::FontCacheKey () :
	family (),
	size (0)
{

}

FontCacheKey::FontCacheKey (const NUIE::Font& font) :
	family (font.GetFamily ()),
	size ((int) (font.GetSize ()))
{

}

bool FontCacheKey::operator== (const FontCacheKey& rhs) const
{
	return family == rhs.family && size == rhs.size;
}

bool FontCacheKey::operator!= (const FontCacheKey& rhs) const
{
	return !operator== (rhs);
}

}
