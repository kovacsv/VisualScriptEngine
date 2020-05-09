#ifndef NE_LOCALIZATION_HPP
#define NE_LOCALIZATION_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace NE
{

class DictionarySource
{
public:
	DictionarySource ();
	virtual ~DictionarySource ();

	virtual bool EnumerateEntries (const std::function<bool (const std::wstring&, const std::wstring&)>& processor) = 0;
};

class Dictionary
{
public:
	Dictionary ();

	void			Clear ();
	bool			IsEmpty () const;
					
	bool			AddLocalizedString (const std::wstring& id, const std::wstring& str);
	bool			HasLocalizedString (const std::wstring& id) const;
	std::wstring	GetLocalizedString (const std::wstring& id) const;

private:
	std::unordered_map<std::wstring, std::wstring>	dictionary;
};

class NonLocalizedCollector
{
public:
	NonLocalizedCollector ();
	virtual ~NonLocalizedCollector ();

	virtual void NonLocalizedStringFound (const std::wstring& str) = 0;
};

class NonLocalizedCollectorGuard
{
public:
	NonLocalizedCollectorGuard (NonLocalizedCollector* collector);
	~NonLocalizedCollectorGuard ();
};

template<typename... Args>
std::wstring FormatString (const std::wstring& format, Args... args)
{
	wchar_t resultString[2048];
	swprintf (resultString, 2048, format.c_str (), args...);
	return std::wstring (resultString);
}

void			SetNonLocalizedCollector (NonLocalizedCollector* collector);

bool			FillDictionary (Dictionary& dictionary, DictionarySource& source);
std::wstring	LocalizeString (const Dictionary& dictionary, const std::wstring& str);

bool			FillDictionary (DictionarySource& source);
std::wstring	LocalizeString (const std::wstring& str);

}

#endif
