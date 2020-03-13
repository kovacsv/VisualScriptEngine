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

const ParameterType ParameterType::Undefined ("undefined");
const ParameterType ParameterType::Boolean ("boolean");
const ParameterType ParameterType::Integer ("integer");
const ParameterType ParameterType::Float ("float");
const ParameterType ParameterType::Double ("double");
const ParameterType ParameterType::String ("string");
const ParameterType ParameterType::Enumeration ("enumeration");

}
