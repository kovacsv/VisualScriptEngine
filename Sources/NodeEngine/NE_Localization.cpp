#include "NE_Localization.hpp"
#include "NE_Debug.hpp"

namespace NE
{

DictionarySource::DictionarySource ()
{

}

DictionarySource::~DictionarySource ()
{

}

NonLocalizedCollector::NonLocalizedCollector ()
{

}

NonLocalizedCollector::~NonLocalizedCollector ()
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

static Dictionary				globalDictionary;
static NonLocalizedCollector*	globalNonLocalizedCollector = nullptr;

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

std::wstring Localize (const Dictionary& dictionary, const std::wstring& str)
{
	if (!dictionary.HasLocalizedString (str)) {
		return str;
	}
	return dictionary.GetLocalizedString (str);
}

void SetNonLocalizedCollector (NonLocalizedCollector* collector)
{
	globalNonLocalizedCollector = collector;
}

bool FillDictionary (DictionarySource& source)
{
	return FillDictionary (globalDictionary, source);
}

std::wstring Localize (const std::wstring& str)
{
	if (!globalDictionary.HasLocalizedString (str)) {
		if (globalNonLocalizedCollector != nullptr) {
			globalNonLocalizedCollector->ProcessNonLocalizedId (str);
		}
		return str;
	}
	return Localize (globalDictionary, str);
}

}
