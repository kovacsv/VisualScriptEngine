#ifndef NUIE_ICONID_HPP
#define NUIE_ICONID_HPP

#include <functional>

namespace NUIE
{

using IconIdType = int;

class IconId
{
public:
	IconId (const IconIdType& id);
	~IconId ();

	IconIdType		GetId () const;
	size_t			GenerateHashValue () const;

	bool			operator== (const IconId& rhs) const;
	bool			operator!= (const IconId& rhs) const;

private:
	IconIdType	id;
};

extern const IconId InvalidIconId;

}

namespace std
{
	template <>
	struct hash<NUIE::IconId>
	{
		size_t operator() (const NUIE::IconId& id) const noexcept
		{
			return id.GenerateHashValue ();
		}
	};
}

#endif
