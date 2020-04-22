#ifndef NE_VALUE_HPP
#define NE_VALUE_HPP

#include "NE_Serializable.hpp"
#include "NE_StringConverter.hpp"

#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace NE
{

class Value;
using ValuePtr = std::shared_ptr<Value>;
using ValueConstPtr = std::shared_ptr<const Value>;

class SingleValue;
using SingleValuePtr = std::shared_ptr<SingleValue>;
using SingleValueConstPtr = std::shared_ptr<const SingleValue>;

class ListValue;
using ListValuePtr = std::shared_ptr<ListValue>;
using ListValueConstPtr = std::shared_ptr<const ListValue>;

class IListValue;
using IListValuePtr = std::shared_ptr<IListValue>;
using IListValueConstPtr = std::shared_ptr<const IListValue>;

class Value : public DynamicSerializable
{
	SERIALIZABLE;

public:
	Value ();
	Value (const Value& src) = delete;
	virtual ~Value ();

	virtual ValuePtr		Clone () const = 0;
	virtual std::wstring	ToString (const StringConverter& stringConverter) const = 0;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

	template <class Type>
	static bool IsType (Value* val);

	template <class Type>
	static bool IsType (const ValuePtr& val);

	template <class Type>
	static bool IsType (const ValueConstPtr& val);

	template <class Type>
	static Type* Cast (Value* val);

	template <class Type>
	static const Type* Cast (const Value* val);

	template <class Type>
	static std::shared_ptr<Type> Cast (const ValuePtr& val);

	template <class Type>
	static std::shared_ptr<const Type> Cast (const ValueConstPtr& val);
};

template <class Type>
bool Value::IsType (Value* val)
{
	return dynamic_cast<Type*> (val) != nullptr;
}

template <class Type>
bool Value::IsType (const ValuePtr& val)
{
	return dynamic_cast<Type*> (val.get ()) != nullptr;
}

template <class Type>
bool Value::IsType (const ValueConstPtr& val)
{
	return dynamic_cast<const Type*> (val.get ()) != nullptr;
}

template <class Type>
Type* Value::Cast (Value* val)
{
	return dynamic_cast<Type*> (val);
}

template <class Type>
const Type* Value::Cast (const Value* val)
{
	return dynamic_cast<const Type*> (val);
}

template <class Type>
std::shared_ptr<Type> Value::Cast (const ValuePtr& val)
{
	return std::dynamic_pointer_cast<Type> (val);
}

template <class Type>
std::shared_ptr<const Type> Value::Cast (const ValueConstPtr& val)
{
	return std::dynamic_pointer_cast<const Type> (val);
}

class SingleValue : public Value
{
	SERIALIZABLE;

public:
	SingleValue ();
	virtual ~SingleValue ();

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class IListValue
{
public:
	IListValue ();
	virtual ~IListValue ();

	virtual size_t					GetSize () const = 0;
	virtual const ValueConstPtr&	GetValue (size_t index) const = 0;
	virtual void					Enumerate (const std::function<void (const ValueConstPtr&)>& processor) const = 0;
};

class ListValue :	public Value,
					public IListValue
{
	DYNAMIC_SERIALIZABLE (ListValue);

public:
	ListValue ();
	ListValue (const std::vector<ValueConstPtr>& values);
	virtual ~ListValue ();

	virtual ValuePtr				Clone () const override;
	virtual std::wstring			ToString (const StringConverter& stringConverter) const override;
	virtual Stream::Status			Read (InputStream& inputStream) override;
	virtual Stream::Status			Write (OutputStream& outputStream) const override;

	virtual size_t					GetSize () const override;
	virtual const ValueConstPtr&	GetValue (size_t index) const override;
	virtual void					Enumerate (const std::function<void (const ValueConstPtr&)>& processor) const override;

	void							Push (const ValueConstPtr& value);
	
private:
	std::vector<ValueConstPtr>	values;
};

class ValueToListValueAdapter : public IListValue
{
public:
	ValueToListValueAdapter (const ValueConstPtr& val);

	virtual size_t					GetSize () const override;
	virtual const ValueConstPtr&	GetValue (size_t index) const override;
	virtual void					Enumerate (const std::function<void (const ValueConstPtr&)>& processor) const override;

private:
	const ValueConstPtr& val;
};

template <class Type>
bool IsSingleType (const ValueConstPtr& val)
{
	if (Value::IsType<Type> (val)) {
		return true;
	}
	if (Value::IsType<ListValue> (val)) {
		const ListValue* listVal = Value::Cast<ListValue> (val.get ());
		if (listVal->GetSize () == 1 && Value::IsType<Type> (listVal->GetValue (0))) {
			return true;
		}
	}
	return false;
}

template <class Type>
bool IsComplexType (const ValueConstPtr& val)
{
	if (Value::IsType<Type> (val)) {
		return true;
	}
	if (Value::IsType<ListValue> (val)) {
		ListValueConstPtr listVal = Value::Cast<ListValue> (val);
		if (listVal->GetSize () == 0) {
			return false;
		}
		bool isType = true;
		listVal->Enumerate ([&] (const ValueConstPtr& innerVal) {
			if (!IsComplexType<Type> (innerVal)) {
				isType = false;
			}
		});
		return isType;
	}
	return false;
}

ValueConstPtr		CreateSingleValue (const ValueConstPtr& value);
IListValueConstPtr	CreateListValue (const ValueConstPtr& value);
void				FlatEnumerate (const ValueConstPtr& value, const std::function<void (const ValueConstPtr&)>& processor);
ValueConstPtr		FlattenValue (const ValueConstPtr& value);

}

#endif
