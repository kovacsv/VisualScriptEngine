#ifndef BI_BUILTINFEATURES_HPP
#define BI_BUILTINFEATURES_HPP

#include "NE_ValueCombination.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_NodeFeatureSet.hpp"

namespace BI
{

extern const FeatureId EnableDisableFeatureId;
extern const FeatureId ValueCombinationFeatureId;

class EnableDisableFeature : public NodeFeature
{
	DYNAMIC_SERIALIZABLE (EnableDisableFeature);

public:
	enum class State
	{
		Disabled	= 0,
		Enabled		= 1
	};

	enum class Mode
	{
		DoNotInvalidate		= 0,
		Invalidate			= 1
	};

	EnableDisableFeature ();
	EnableDisableFeature (State state, Mode mode);
	virtual ~EnableDisableFeature ();

	State						GetState () const;
	Mode						GetMode () const;
	void						SetState (State newState);
	void						DrawInplace (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const;

	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	State						state;
	Mode						mode;
};

class ValueCombinationFeature : public NodeFeature
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

	static bool					CombineValues (const BasicUINode* uiNode, const std::vector<NE::ValueConstPtr>& values, const std::function<bool (const NE::ValueCombination&)>& processor);

private:
	NE::ValueCombinationMode	valueCombinationMode;
};

std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const BasicUINode* uiNode);
std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const BasicUINodeConstPtr& uiNode);
std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const BasicUINode* uiNode);
std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const BasicUINodeConstPtr& uiNode);

}

#endif
