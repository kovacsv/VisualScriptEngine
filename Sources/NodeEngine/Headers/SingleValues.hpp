#ifndef SINGLEVALUES_HPP
#define SINGLEVALUES_HPP

#include "GenericValue.hpp"
#include "Serializable.hpp"

namespace NE
{

class NumberValue : public SingleValue
{
	SERIALIZABLE;

public:
	NumberValue ();
	virtual ~NumberValue ();

	virtual int				ToInteger () const = 0;
	virtual double			ToDouble () const = 0;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;

	static int				ToInteger (const ValuePtr& val);
	static int				ToInteger (Value* val);
	static double			ToDouble (const ValuePtr& val);
	static double			ToDouble (Value* val);
};

class IntValue : public NumberValue,
				 public GenericValueBase<int>
{
	DYNAMIC_SERIALIZABLE (IntValue);

public:
	IntValue ();
	IntValue (int val);
	virtual ~IntValue ();

	virtual std::wstring	ToString () const override;
	virtual int				ToInteger () const override;
	virtual double			ToDouble () const override;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class DoubleValue : public NumberValue,
					public GenericValueBase<double>
{
	DYNAMIC_SERIALIZABLE (DoubleValue);

public:
	DoubleValue ();
	DoubleValue (double val);
	virtual ~DoubleValue ();

	virtual std::wstring	ToString () const override;
	virtual int				ToInteger () const override;
	virtual double			ToDouble () const override;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class StringValue : public SingleValue,
					public GenericValueBase<std::wstring>
{
	DYNAMIC_SERIALIZABLE (StringValue);

public:
	StringValue ();
	StringValue (const std::wstring& val);
	virtual ~StringValue ();

	virtual std::wstring	ToString () const override;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

template <class Type>
class GenericValue : public SingleValue,
					 public GenericValueBase<Type>
{
public:
	GenericValue (const Type& val);
	virtual ~GenericValue ();
};

template <class Type>
GenericValue<Type>::GenericValue (const Type& val) :
	SingleValue (),
	GenericValueBase<Type> (val)
{

}

template <class Type>
GenericValue<Type>::~GenericValue ()
{

}

}

#endif
