#ifndef NE_SINGLEVALUES_HPP
#define NE_SINGLEVALUES_HPP

#include "NE_GenericValue.hpp"
#include "NE_Serializable.hpp"

namespace NE
{

class BooleanValue : public GenericValue<bool>
{
	DYNAMIC_SERIALIZABLE (BooleanValue);

public:
	BooleanValue ();
	BooleanValue (bool val);
	virtual ~BooleanValue ();

	virtual ValuePtr		Clone () const override;
	virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class NumberValue
{
public:
	NumberValue ();
	virtual ~NumberValue ();

	virtual int		ToInteger () const = 0;
	virtual float	ToFloat () const = 0;
	virtual double	ToDouble () const = 0;

	static int		ToInteger (const ValueConstPtr& val);
	static int		ToInteger (const ValuePtr& val);
	static int		ToInteger (Value* val);
	static float	ToFloat (const ValueConstPtr& val);
	static float	ToFloat (const ValuePtr& val);
	static float	ToFloat (Value* val);
	static double	ToDouble (const ValueConstPtr& val);
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

	virtual ValuePtr		Clone () const override;
	virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

	virtual int				ToInteger () const override;
	virtual float			ToFloat () const override;
	virtual double			ToDouble () const override;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

class FloatValue :	public NumberValue,
					public GenericValue<float>
{
	DYNAMIC_SERIALIZABLE (FloatValue);

public:
	FloatValue ();
	FloatValue (float val);
	virtual ~FloatValue ();

	virtual ValuePtr		Clone () const override;
	virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

	virtual int				ToInteger () const override;
	virtual float			ToFloat () const override;
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

	virtual ValuePtr		Clone () const override;
	virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

	virtual int				ToInteger () const override;
	virtual float			ToFloat () const override;
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

	virtual ValuePtr		Clone () const override;
	virtual std::wstring	ToString (const StringConverter& stringConverter) const override;

	virtual Stream::Status	Read (InputStream& inputStream) override;
	virtual Stream::Status	Write (OutputStream& outputStream) const override;
};

}

#endif
