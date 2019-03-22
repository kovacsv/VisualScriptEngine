#ifndef NUIE_LOCALIZATION_HPP
#define NUIE_LOCALIZATION_HPP

#include "NE_Localization.hpp"

namespace NUIE
{

class PoDictionarySource : public NE::DictionarySource
{
public:
	PoDictionarySource (const std::wstring& poContent);
	virtual ~PoDictionarySource ();

	virtual bool EnumerateEntries (const std::function<bool (const std::wstring&, const std::wstring&)>& processor) override;

private:
	std::wstring poContent;
};

}

#endif
