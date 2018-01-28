#ifndef UINODEPARAMETERACCESSOR_HPP
#define UINODEPARAMETERACCESSOR_HPP

#include "Value.hpp"

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

private:
	std::string id;
};

class NodeParameterAccessor
{
public:
	virtual size_t					GetParameterCount () const = 0;
	virtual const std::wstring&		GetParameterName (size_t index) const = 0;
	virtual NE::ValuePtr			GetParameterValue (size_t index) const = 0;
	virtual const ParameterType&	GetParameterType (size_t index) const = 0;
	virtual bool					SetParameterValue (size_t index, const NE::ValuePtr& value) = 0;
};

typedef std::shared_ptr<NodeParameterAccessor> NodeParameterAccessorPtr;
typedef std::shared_ptr<const NodeParameterAccessor> NodeParameterAccessorConstPtr;

}

#endif
