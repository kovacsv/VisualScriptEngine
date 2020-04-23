#ifndef NE_STRINGCONVERTER_HPP
#define NE_STRINGCONVERTER_HPP

#include <string>
#include <memory>

namespace NE
{

class StringSettings
{
public:
	StringSettings ();
	virtual ~StringSettings ();

	virtual wchar_t		GetDecimalSeparator () const = 0;
	virtual wchar_t		GetListSeparator () const = 0;
	virtual int			GetPrecision () const = 0;
};

using StringSettingsPtr = std::shared_ptr<StringSettings>;
using StringSettingsConstPtr = std::shared_ptr<const StringSettings>;

class BasicStringSettings : public StringSettings
{
public:
	BasicStringSettings () = delete;
	BasicStringSettings (wchar_t decimalSeparator, wchar_t listSeparator, int precision);
	virtual ~BasicStringSettings ();

	virtual wchar_t		GetDecimalSeparator () const override;
	virtual wchar_t		GetListSeparator () const override;
	virtual int			GetPrecision () const override;

	void				SetDecimalSeparator (wchar_t newDecimalSeparator);
	void				SetListSeparator (wchar_t newListSeparator);
	void				SetPrecision (int newPrecision);

private:
	wchar_t		decimalSeparator;
	wchar_t		listSeparator;
	int			precision;
};

class StringConverter
{
public:
	enum class Measure
	{
		Number,
		Length,
		Area,
		Volume,
		Angle
	};

	class ListEnumerator
	{
	public:
		ListEnumerator ();
		virtual ~ListEnumerator ();

		virtual size_t			GetSize () const = 0;
		virtual std::wstring	GetItem (size_t index) const = 0;
	};

	StringConverter ();
	virtual ~StringConverter ();

	virtual std::wstring	IntegerToString (int val) const = 0;
	virtual std::wstring	NumberToString (double val, Measure measure) const = 0;
	virtual std::wstring	ListToString (const ListEnumerator& enumerator) const = 0;
};

class BasicStringConverter : public StringConverter
{
public:
	BasicStringConverter (const BasicStringSettings& settings);
	virtual ~BasicStringConverter ();

	virtual std::wstring	IntegerToString (int val) const override;
	virtual std::wstring	NumberToString (double val, Measure measure) const override;
	virtual std::wstring	ListToString (const ListEnumerator& enumerator) const override;

private:
	BasicStringSettings		settings;
};

using StringConverterPtr = std::shared_ptr<StringConverter>;
using StringConverterConstPtr = std::shared_ptr<const StringConverter>;

std::wstring ConvertNumberToString (double val, int precision, wchar_t decimalSeparator);
std::wstring ConvertListToString (const StringConverter::ListEnumerator& enumerator, wchar_t listSeparator);

const BasicStringSettings& GetDefaultStringSettings ();
const BasicStringConverter& GetDefaultStringConverter ();

}

#endif
