#ifndef NE_VALUEBASE_HPP
#define NE_VALUEBASE_HPP

#include "NE_Value.hpp"
#include <vector>
#include <functional>

namespace NE
{

template <class Type>
class GenericValue : public SingleValue
{
public:
	GenericValue (const Type& val);
	GenericValue (const GenericValue&) = delete;
	virtual ~GenericValue ();

	GenericValue&			operator= (const GenericValue&) = delete;

	void					SetValue (const Type& newVal);
	const Type&				GetValue () const;

	static const Type&		Get (const ValueConstPtr& val);
	static const Type&		Get (const ValuePtr& val);
	static const Type&		Get (Value* val);

protected:
	Type val;
};

template <class Type>
GenericValue<Type>::GenericValue (const Type& val) :
	val (val)
{

}

template <class Type>
GenericValue<Type>::~GenericValue ()
{

}

template <class Type>
void GenericValue<Type>::SetValue (const Type& newVal)
{
	val = newVal;
}

template <class Type>
const Type& GenericValue<Type>::GetValue () const
{
	return val;
}

template <class Type>
const Type& GenericValue<Type>::Get (const ValueConstPtr& val)
{
	return Value::Cast<GenericValue<Type>> (val.get ())->GetValue ();
}

template <class Type>
const Type& GenericValue<Type>::Get (const ValuePtr& val)
{
	return Value::Cast<GenericValue<Type>> (val.get ())->GetValue ();
}

template <class Type>
const Type& GenericValue<Type>::Get (Value* val)
{
	return Value::Cast<GenericValue<Type>> (val)->GetValue ();
}

}

#endif
