#include "NUIE_IconId.hpp"

namespace NUIE
{

IconId::IconId (const IconIdType& id) :
	id (id)
{

}

IconId::~IconId ()
{

}

IconIdType IconId::GetId () const
{
	return id;
}

size_t IconId::GenerateHashValue () const
{
	return std::hash<IconIdType> {} (id);
}

bool IconId::operator== (const IconId& rhs) const
{
	return id == rhs.id;
}

bool IconId::operator!= (const IconId& rhs) const
{
	return !operator== (rhs);
}

const IconId InvalidIconId (-1);

}
