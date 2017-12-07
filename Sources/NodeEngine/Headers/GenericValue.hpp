#ifndef VALUEBASE_HPP
#define VALUEBASE_HPP

#include "Value.hpp"
#include <vector>
#include <functional>

namespace NE
{

template <class Type>
class GenericValueBase
{
public:
	GenericValueBase (const Type& val);
	virtual ~GenericValueBase ();

	void				SetValue (const Type& newVal);
	const Type&			GetValue () const;

	static const Type&	Get (const ValuePtr& val);
	static const Type&	Get (Value* val);

private:
	GenericValueBase (const GenericValueBase&) = delete;
	GenericValueBase& operator= (const GenericValueBase&) = delete;

protected:
	Type val;
};

template <class Type>
GenericValueBase<Type>::GenericValueBase (const Type& val) :
	val (val)
{

}

template <class Type>
GenericValueBase<Type>::~GenericValueBase ()
{

}

template <class Type>
void GenericValueBase<Type>::SetValue (const Type& newVal)
{
	val = newVal;
}

template <class Type>
const Type& GenericValueBase<Type>::GetValue () const
{
	return val;
}

template <class Type>
const Type& GenericValueBase<Type>::Get (const ValuePtr& val)
{
	return Value::Cast<GenericValueBase<Type>> (val.get ())->GetValue ();
}

template <class Type>
const Type& GenericValueBase<Type>::Get (Value* val)
{
	return Value::Cast<GenericValueBase<Type>> (val)->GetValue ();
}

}

#endif
