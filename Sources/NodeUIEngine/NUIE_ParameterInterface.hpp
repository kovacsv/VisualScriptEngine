#ifndef NUIE_PARAMETERINTERFACE_HPP
#define NUIE_PARAMETERINTERFACE_HPP

#include "NE_Value.hpp"

namespace NUIE
{

class ParameterType
{
public:
	ParameterType (const std::string& id);

	const std::string&	GetId () const;
	bool				operator== (const ParameterType& rhs) const;
	bool				operator!= (const ParameterType& rhs) const;

	static ParameterType Undefined;
	static ParameterType String;
	static ParameterType Integer;
	static ParameterType Double;
	static ParameterType Enumeration;

private:
	std::string id;
};

class ParameterInterface
{
public:
	ParameterInterface ();
	virtual ~ParameterInterface ();

	virtual size_t						GetParameterCount () const = 0;
	virtual const std::wstring&			GetParameterName (size_t index) const = 0;
	virtual NE::ValuePtr				GetParameterValue (size_t index) const = 0;
	virtual std::vector<std::wstring>	GetParameterValueChoices (size_t index) const = 0;
	virtual const ParameterType&		GetParameterType (size_t index) const = 0;
	virtual bool						IsValidParameterValue (size_t index, const NE::ValuePtr& value) const = 0;
	virtual bool						SetParameterValue (size_t index, const NE::ValuePtr& value) = 0;
};

using ParameterInterfacePtr = std::shared_ptr<ParameterInterface>;
using ParameterInterfaceConstPtr = std::shared_ptr<const ParameterInterface>;

}

#endif
