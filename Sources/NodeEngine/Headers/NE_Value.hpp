#ifndef VALUE_HPP
#define VALUE_HPP

#include "NE_Serializable.hpp"

#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace NE
{

class Value;
typedef std::shared_ptr<Value> ValuePtr;

class SingleValue;
typedef std::shared_ptr<SingleValue> SingleValuePtr;

class ListValue;
typedef std::shared_ptr<ListValue> ListValuePtr;

class IListValue;
typedef std::shared_ptr<IListValue> IListValuePtr;

class Value : public DynamicSerializable
{
	SERIALIZABLE;

public:
	Value ();
	Value (const Value& src) = delete;
	virtual ~Value ();

	virtual ValuePtr		Clone () const = 0;
	virtual std::wstring	ToString () const = 0;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

	template <class Type>
	static bool IsType (Value* val);

	template <class Type>
	static bool IsType (const ValuePtr& val);

	template <class Type>
	static Type* Cast (Value* val);

	template <class Type>
	static std::shared_ptr<Type> Cast (const ValuePtr& val);
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
Type* Value::Cast (Value* val)
{
	return dynamic_cast<Type*> (val);
}

template <class Type>
std::shared_ptr<Type> Value::Cast (const ValuePtr& val)
{
	return std::dynamic_pointer_cast<Type> (val);
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

	virtual size_t				GetSize () const = 0;
	virtual const ValuePtr&		GetValue (size_t index) const = 0;
	virtual void				Enumerate (const std::function<void (const ValuePtr&)>& processor) const = 0;
};

class ListValue :	public Value,
					public IListValue
{
	DYNAMIC_SERIALIZABLE (ListValue);

public:
	ListValue ();
	ListValue (const std::vector<ValuePtr>& values);
	virtual ~ListValue ();

	virtual ValuePtr			Clone () const override;
	virtual std::wstring		ToString () const override;
	virtual Stream::Status		Read (InputStream& inputStream) override;
	virtual Stream::Status		Write (OutputStream& outputStream) const override;

	virtual size_t				GetSize () const override;
	virtual const ValuePtr&		GetValue (size_t index) const override;
	virtual void				Enumerate (const std::function<void (const ValuePtr&)>& processor) const override;

	void						Push (const ValuePtr& value);
	
private:
	std::vector<ValuePtr> values;
};

class ValueToListValueAdapter : public IListValue
{
public:
	ValueToListValueAdapter (const ValuePtr& val);

	virtual size_t				GetSize () const override;
	virtual const ValuePtr&		GetValue (size_t index) const override;
	virtual void				Enumerate (const std::function<void (const ValuePtr&)>& processor) const override;

private:
	const ValuePtr& val;
};

template <class Type>
bool IsSingleType (const ValuePtr& val)
{
	if (Value::IsType<Type> (val)) {
		return true;
	}
	if (Value::IsType<ListValue> (val)) {
		ListValue* listVal = Value::Cast<ListValue> (val.get ());
		if (listVal->GetSize () == 1) {
			return true;
		}
	}
	return false;
}

template <class Type>
bool IsComplexType (const ValuePtr& val)
{
	if (Value::IsType<Type> (val)) {
		return true;
	}
	if (Value::IsType<ListValue> (val)) {
		ListValuePtr listVal = Value::Cast<ListValue> (val);
		if (listVal->GetSize () == 0) {
			return false;
		}
		bool isType = true;
		listVal->Enumerate ([&] (const ValuePtr& innerVal) {
			if (!Value::IsType<Type> (innerVal)) {
				isType = false;
			}
		});
		return isType;
	}
	return false;
}

ValuePtr		CreateSingleValue (const ValuePtr& value);
IListValuePtr	CreateListValue (const ValuePtr& value);

}

#endif
