#ifndef BUILTINFEATURES_HPP
#define BUILTINFEATURES_HPP

#include "UINodeCommands.hpp"
#include "ValueCombination.hpp"

namespace BIN
{

class Feature
{
public:
	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) = 0;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const = 0;
};

class ValueCombinationFeature : public Feature
{
	SERIALIZABLE;

public:
	ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode);
	virtual ~ValueCombinationFeature ();

	void				SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode);
	bool				CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const;
	void				RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	NE::ValueCombinationMode	valueCombinationMode;
};


class EnableDisableFeature : public Feature
{
	SERIALIZABLE;

public:
	EnableDisableFeature (bool nodeEnabled);
	virtual ~EnableDisableFeature ();

	void				SetEnableState (bool isNodeEnabled, const NE::ValuePtr& value, NE::EvaluationEnv& env);
	void				RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const;
	void				CreateDrawingEnvironment (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const;
	void				FeatureCalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;

	virtual void		OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const = 0;
	virtual void		OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const = 0;
	virtual void		OnDisabled (NE::EvaluationEnv& env) const = 0;

	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

private:
	bool	nodeEnabled;
};

}

#endif
