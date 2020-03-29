#ifndef NE_STRINGSETTINGS_HPP
#define NE_STRINGSETTINGS_HPP

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

const BasicStringSettings& GetDefaultStringSettings ();

}

#endif
