#ifndef DRAWING_HPP
#define DRAWING_HPP

#include "Checksum.hpp"
#include <string>

namespace NUIE
{

enum class HorizontalAnchor
{
	Left,
	Center,
	Right
};

enum class VerticalAnchor
{
	Top,
	Center,
	Bottom
};

class Color
{
public:
	Color (unsigned char r, unsigned char g, unsigned char b);

	unsigned char GetR () const;
	unsigned char GetG () const;
	unsigned char GetB () const;

private:
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Pen
{
public:
	Pen (const Color& color, double thickness);

	const Color&	GetColor () const;
	double			GetThickness () const;

private:
	Color	color;
	double	thickness;
};

class Font
{
public:
	Font (const std::wstring& family, double size);

	const std::wstring&		GetFamily () const;
	const double			GetSize () const;

private:
	std::wstring	family;
	double			size;
};

void AddColorToChecksum (NE::Checksum& checksum, const Color& color);
void AddPenToChecksum (NE::Checksum& checksum, const Pen& pen);
void AddFontToChecksum (NE::Checksum& checksum, const Font& font);

}

#endif
