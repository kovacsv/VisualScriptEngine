#ifndef SINGLEVALUES_HPP
#define SINGLEVALUES_HPP

#include "GenericValue.hpp"
#include "Serializable.hpp"

namespace NE
{

class NumberValue
{
public:
	virtual int		ToInteger () const = 0;
	virtual double	ToDouble () const = 0;

	static int		ToInteger (const ValuePtr& val);
	static int		ToInteger (Value* val);
	static double	ToDouble (const ValuePtr& val);
	static double	ToDouble (Value* val);
};

class IntValue : public NumberValue,
				 public GenericValue<int>
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
					public GenericValue<double>
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

class StringValue : public GenericValue<std::wstring>
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

}

#endif
