#include "NUIE_ParameterInterface.hpp"

namespace NUIE
{

ParameterType::ParameterType (const std::string& id) :
	id (id)
{

}

const std::string& ParameterType::GetId () const
{
	return id;
}

bool ParameterType::operator== (const ParameterType& rhs) const
{
	return id == rhs.id;
}

bool ParameterType::operator!= (const ParameterType& rhs) const
{
	return !operator== (rhs);
}

ParameterInterface::ParameterInterface ()
{

}

ParameterInterface::~ParameterInterface ()
{

}

ParameterType ParameterType::Undefined ("undefined");
ParameterType ParameterType::String ("string");
ParameterType ParameterType::Integer ("integer");
ParameterType ParameterType::Double ("double");

}
