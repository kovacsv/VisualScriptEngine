#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

namespace NE
{

static NonLocalizedCollector* nonLocalizedCollector = nullptr;

DictionarySource::DictionarySource ()
{

}

DictionarySource::~DictionarySource ()
{

}

Dictionary::Dictionary ()
{

}

void Dictionary::Clear ()
{
	dictionary.clear ();
}

bool Dictionary::IsEmpty () const
{
	return dictionary.empty ();
}

bool Dictionary::AddLocalizedString (const std::wstring& id, const std::wstring& str)
{
	if (DBGERROR (dictionary.find (id) != dictionary.end ())) {
		return false;
	}
	dictionary.insert ({ id, str });
	return true;
}

bool Dictionary::HasLocalizedString (const std::wstring& id) const
{
	return dictionary.find (id) != dictionary.end ();
}

std::wstring Dictionary::GetLocalizedString (const std::wstring& id) const
{
	if (dictionary.find (id) == dictionary.end ()) {
		return id;
	}
	return dictionary.at (id);
}

NonLocalizedCollector::NonLocalizedCollector ()
{

}

NonLocalizedCollector::~NonLocalizedCollector ()
{

}

NonLocalizedCollectorGuard::NonLocalizedCollectorGuard (NonLocalizedCollector* collector)
{
	SetNonLocalizedCollector (collector);
}

NonLocalizedCollectorGuard::~NonLocalizedCollectorGuard ()
{
	SetNonLocalizedCollector (nullptr);
}

static Dictionary& GetGlobalDictionary ()
{
	static Dictionary globalDictionary;
	return globalDictionary;
}

void SetNonLocalizedCollector (NonLocalizedCollector* collector)
{
	nonLocalizedCollector = collector;
}

bool FillDictionary (Dictionary& dictionary, DictionarySource& source)
{
	dictionary.Clear ();
	bool success = source.EnumerateEntries ([&] (const std::wstring& id, const std::wstring& str) {
		if (!dictionary.AddLocalizedString (id, str)) {
			return false;
		}
		return true;
	});
	if (!success) {
		dictionary.Clear ();
	}
	return success;
}

std::wstring LocalizeString (const Dictionary& dictionary, const std::wstring& str)
{
	if (!dictionary.HasLocalizedString (str)) {
		return str;
	}
	return dictionary.GetLocalizedString (str);
}

bool FillDictionary (DictionarySource& source)
{
	return FillDictionary (GetGlobalDictionary (), source);
}

std::wstring LocalizeString (const std::wstring& str)
{
	const Dictionary& globalDictionary = GetGlobalDictionary ();
	if (nonLocalizedCollector != nullptr) {
		if (!globalDictionary.HasLocalizedString (str)) {
			nonLocalizedCollector->NonLocalizedStringFound (str);
		}
	}
	return LocalizeString (globalDictionary, str);
}

}
