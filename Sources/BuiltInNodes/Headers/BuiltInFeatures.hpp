#ifndef BUILTINFEATURES_HPP
#define BUILTINFEATURES_HPP

#include "UINodeCommands.hpp"
#include "ValueCombination.hpp"

namespace NUIE
{

class ValueCombinationFeature
{
	SERIALIZABLE;

public:
	ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode);
	~ValueCombinationFeature ();

	void				SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode);
	bool				CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const;
	void				RegisterFeatureCommands (NodeCommandRegistrator& commandRegistrator) const;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	NE::ValueCombinationMode	valueCombinationMode;
};

}

#endif
