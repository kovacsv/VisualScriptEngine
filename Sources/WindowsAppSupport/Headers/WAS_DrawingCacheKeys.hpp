#ifndef DRAWINGCACHEKEYS_HPP
#define DRAWINGCACHEKEYS_HPP

#include "NUIE_Drawing.hpp"

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

class BrushCacheKey
{
public:
	BrushCacheKey ();
	BrushCacheKey (const NUIE::Color& color);

	bool	operator== (const BrushCacheKey& rhs) const;
	bool	operator!= (const BrushCacheKey& rhs) const;

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

namespace std
{
	template <>
	struct hash<PenCacheKey>
	{
		size_t operator() (const PenCacheKey& key) const noexcept
		{
			return key.thickness + 12289 * key.r + 24593 * key.g + 49157 * key.b;
		}
	};

	template <>
	struct hash<BrushCacheKey>
	{
		size_t operator() (const BrushCacheKey& key) const noexcept
		{
			return key.r + 24593 * key.g + 49157 * key.b;
		}
	};

	template <>
	struct hash<FontCacheKey>
	{
		size_t operator() (const FontCacheKey& key) const noexcept
		{
			return std::hash<std::wstring> {} (key.family) + 49157 * std::hash<int> {} (key.size);
		}
	};
}

#endif
