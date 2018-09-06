#ifndef BUILTINFEATURES_HPP
#define BUILTINFEATURES_HPP

#include "NE_ValueCombination.hpp"
#include "NUIE_UINodeCommandRegistration.hpp"

namespace BI
{

class Feature
{
public:
	Feature ();
	virtual ~Feature ();

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) = 0;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const = 0;

	virtual void				RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const = 0;
	virtual void				RegisterFeatureParameters (NUIE::NodeParameterList& parameterList) const = 0;
};

class ValueCombinationFeature : public Feature
{
	SERIALIZABLE;

public:
	ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode);
	virtual ~ValueCombinationFeature ();

	void				SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode);
	bool				CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const;

	virtual void		RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void		RegisterFeatureParameters (NUIE::NodeParameterList& parameterList) const override;

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
	void				CreateDrawingEnvironment (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const;
	void				FeatureProcessValue (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;

	virtual void		RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void		RegisterFeatureParameters (NUIE::NodeParameterList& parameterList) const override;

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
