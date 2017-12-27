#include "Drawing.hpp"

namespace NUIE
{

Color::Color (unsigned char r, unsigned char g, unsigned char b) :
	r (r),
	g (g),
	b (b)
{

}

unsigned char Color::GetR () const
{
	return r;
}

unsigned char Color::GetG () const
{
	return g;
}

unsigned char Color::GetB () const
{
	return b;
}

bool Color::operator== (const Color& rhs) const
{
	return r == rhs.r && g == rhs.g && b == rhs.b;
}

bool Color::operator!= (const Color& rhs) const
{
	return !operator== (rhs);
}

Pen::Pen (const Color & color, double thickness) :
	color (color),
	thickness (thickness)
{

}

const Color& Pen::GetColor () const
{
	return color;
}

double Pen::GetThickness () const
{
	return thickness;
}

bool Pen::operator== (const Pen& rhs) const
{
	return color == rhs.color && thickness == rhs.thickness;
}

bool Pen::operator!= (const Pen& rhs) const
{
	return !operator== (rhs);
}

Font::Font (const std::wstring& family, double size) :
	family (family),
	size (size)
{

}

const std::wstring& Font::GetFamily () const
{
	return family;
}

const double Font::GetSize () const
{
	return size;
}

bool Font::operator== (const Font& rhs) const
{
	return family == rhs.family && size == rhs.size;
}

bool Font::operator!= (const Font& rhs) const
{
	return !operator== (rhs);
}

void AddColorToChecksum (NE::Checksum& checksum, const Color& color)
{
	checksum.Add (color.GetR ());
	checksum.Add (color.GetG ());
	checksum.Add (color.GetB ());
}

void AddPenToChecksum (NE::Checksum& checksum, const Pen& pen)
{
	AddColorToChecksum (checksum, pen.GetColor ());
	checksum.Add (pen.GetThickness ());
}

void AddFontToChecksum (NE::Checksum& checksum, const Font& font)
{
	checksum.Add (font.GetFamily ());
	checksum.Add (font.GetSize ());
}

}
