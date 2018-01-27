#ifndef UINODEPARAMETERACCESSOR_HPP
#define UINODEPARAMETERACCESSOR_HPP

#include "Value.hpp"

namespace NUIE
{

enum class ParameterType
{
	Undefined,
	String,
	Integer
};

class NodeParameterAccessor
{
public:
	virtual size_t					GetParameterCount () const = 0;
	virtual const std::wstring&		GetParameterName (size_t index) const = 0;
	virtual NE::ValuePtr			GetParameterValue (size_t index) const = 0;
	virtual ParameterType			GetParameterType (size_t index) const = 0;
	virtual bool					SetParameterValue (size_t index, const NE::ValuePtr& value) = 0;
};

typedef std::shared_ptr<NodeParameterAccessor> NodeParameterAccessorPtr;
typedef std::shared_ptr<const NodeParameterAccessor> NodeParameterAccessorConstPtr;

}

#endif
