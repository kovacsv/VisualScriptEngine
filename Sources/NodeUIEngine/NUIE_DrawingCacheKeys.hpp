#ifndef NUIE_DRAWINGCACHEKEYS_HPP
#define NUIE_DRAWINGCACHEKEYS_HPP

#include "NUIE_Drawing.hpp"

namespace NUIE
{

class PenCacheKey
{
public:
	PenCacheKey ();
	PenCacheKey (const NUIE::Pen& pen);

	bool	operator== (const PenCacheKey& rhs) const;
	bool	operator!= (const PenCacheKey& rhs) const;

	int				thickness;
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
};

class ColorCacheKey
{
public:
	ColorCacheKey ();
	ColorCacheKey (const NUIE::Color& color);

	bool	operator== (const ColorCacheKey& rhs) const;
	bool	operator!= (const ColorCacheKey& rhs) const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class FontCacheKey
{
public:
	FontCacheKey ();
	FontCacheKey (const NUIE::Font& font);

	bool	operator== (const FontCacheKey& rhs) const;
	bool	operator!= (const FontCacheKey& rhs) const;

	std::wstring	family;
	int				size;
};

}

namespace std
{
	template <>
	struct hash<NUIE::PenCacheKey>
	{
		size_t operator() (const NUIE::PenCacheKey& key) const noexcept
		{
			return key.thickness + 12289 * key.r + 24593 * key.g + 49157 * key.b;
		}
	};

	template <>
	struct hash<NUIE::ColorCacheKey>
	{
		size_t operator() (const NUIE::ColorCacheKey& key) const noexcept
		{
			return key.r + 24593 * key.g + 49157 * key.b;
		}
	};

	template <>
	struct hash<NUIE::FontCacheKey>
	{
		size_t operator() (const NUIE::FontCacheKey& key) const noexcept
		{
			return std::hash<std::wstring> {} (key.family) + 49157 * std::hash<int> {} (key.size);
		}
	};
}

#endif
