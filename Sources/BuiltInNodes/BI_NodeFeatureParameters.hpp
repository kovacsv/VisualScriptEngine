#ifndef BI_NODEFEATUREPARAMETERS_HPP
#define BI_NODEFEATUREPARAMETERS_HPP

#include "BI_NodeFeatureSet.hpp"
#include "BI_BasicUINode.hpp"
#include "NUIE_NodeCommonParameters.hpp"

namespace BI
{

template <typename FeatureType, typename ValueType>
class TypedFeatureParameter : public NUIE::TypedParameter<ValueType>
{
public:
	TypedFeatureParameter (const std::wstring& name, const NUIE::ParameterType& type, const FeatureId& featureId) :
		TypedParameter<ValueType> (name, type),
		featureId (featureId)
	{

	}

	virtual ~TypedFeatureParameter ()
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodeConstPtr& uiNode) const
	{
		if (!NE::Node::IsTypeConst<BasicUINode> (uiNode)) {
			return false;
		}
		return NE::Node::CastConst<BasicUINode> (uiNode)->HasFeature (featureId);
	}

private:
	FeatureId featureId;
};

template <typename FeatureType>
class EnumerationFeatureParameter : public TypedFeatureParameter<FeatureType, NE::IntValue>
{
public:
	EnumerationFeatureParameter (const std::wstring& name, const std::vector<std::wstring>& valueChoices, const FeatureId& featureId) :
		TypedFeatureParameter<FeatureType, NE::IntValue> (name, NUIE::ParameterType::Enumeration, featureId),
		valueChoices (valueChoices)
	{

	}

	virtual bool IsValidValue (const NUIE::UINodeConstPtr&, const std::shared_ptr<const NE::IntValue>& value) const override
	{
		int valueInt = value->GetValue ();
		return valueInt >= 0 && valueInt < (int) valueChoices.size ();
	}

	virtual std::vector<std::wstring> GetValueChoices () const override
	{
		return valueChoices;
	}

	virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
	{
		if (DBGERROR (!NE::Node::IsTypeConst<BasicUINode> (uiNode))) {
			return nullptr;
		}
		BasicUINodeConstPtr basicUINode = NE::Node::CastConst<BasicUINode> (uiNode);
		return GetValueInternal (basicUINode);
	}

	virtual bool SetValueInternal (NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
	{
		if (DBGERROR (!NE::Node::IsType<BasicUINode> (uiNode))) {
			return nullptr;
		}
		BasicUINodePtr basicUINode = NE::Node::Cast<BasicUINode> (uiNode);
		return SetValueInternal (invalidator, evaluationEnv, basicUINode, value);
	}

	virtual NE::ValueConstPtr GetValueInternal (const BasicUINodeConstPtr& uiNode) const = 0;
	virtual bool SetValueInternal (NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv& evaluationEnv, BasicUINodePtr& uiNode, const NE::ValueConstPtr& value) = 0;

private:
	std::vector<std::wstring> valueChoices;
};

}

#endif
