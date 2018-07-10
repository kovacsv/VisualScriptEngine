#ifndef STRINGSETTINGS_HPP
#define STRINGSETTINGS_HPP

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

class BasicStringSettings : public StringSettings
{
public:
	BasicStringSettings (wchar_t decimalSeparator, wchar_t listSeparator, int precision);
	virtual ~BasicStringSettings ();

	virtual wchar_t		GetDecimalSeparator () const override;
	virtual wchar_t		GetListSeparator () const override;
	virtual int			GetPrecision () const override;

private:
	wchar_t		decimalSeparator;
	wchar_t		listSeparator;
	int			precision;
};

extern BasicStringSettings DefaultStringSettings;

}

#endif
