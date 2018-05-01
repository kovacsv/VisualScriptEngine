#ifndef PARAMETERINTERFACE_HPP
#define PARAMETERINTERFACE_HPP

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

private:
	std::string id;
};

class ParameterInterface
{
public:
	virtual size_t					GetParameterCount () const = 0;
	virtual const std::wstring&		GetParameterName (size_t index) const = 0;
	virtual NE::ValuePtr			GetParameterValue (size_t index) const = 0;
	virtual const ParameterType&	GetParameterType (size_t index) const = 0;
	virtual bool					SetParameterValue (size_t index, const NE::ValuePtr& value) = 0;
};

typedef std::shared_ptr<ParameterInterface> ParameterInterfacePtr;
typedef std::shared_ptr<const ParameterInterface> ParameterInterfaceConstPtr;

}

#endif
