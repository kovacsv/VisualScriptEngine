#ifndef BI_BUILTINFEATURES_HPP
#define BI_BUILTINFEATURES_HPP

#include "NE_ValueCombination.hpp"
#include "NUIE_NodeFeatureSet.hpp"
#include "NUIE_NodeMenuCommandRegistrator.hpp"

namespace BI
{

extern const NUIE::FeatureId EnableDisableFeatureId;
extern const NUIE::FeatureId ValueCombinationFeatureId;

class EnableDisableFeature : public NUIE::NodeFeature
{
	DYNAMIC_SERIALIZABLE (EnableDisableFeature);

public:
	EnableDisableFeature ();
	EnableDisableFeature (bool nodeEnabled);
	virtual ~EnableDisableFeature ();

	bool				GetEnableState () const;
	void				SetEnableState (bool isNodeEnabled);
	void				DrawInplace (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const;

	virtual void		RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void		RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	bool	nodeEnabled;
};

class ValueCombinationFeature : public NUIE::NodeFeature
{
	DYNAMIC_SERIALIZABLE (ValueCombinationFeature);

public:
	ValueCombinationFeature ();
	ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode);
	virtual ~ValueCombinationFeature ();

	NE::ValueCombinationMode	GetValueCombinationMode () const;
	void						SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode);
	bool						CombineValues (const std::vector<NE::ValueConstPtr>& values, const std::function<bool (const NE::ValueCombination&)>& processor) const;

	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::ValueCombinationMode	valueCombinationMode;
};

std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const NUIE::UINode* uiNode);
std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const NUIE::UINodeConstPtr& uiNode);
std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const NUIE::UINode* uiNode);
std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const NUIE::UINodeConstPtr& uiNode);

}

#endif
