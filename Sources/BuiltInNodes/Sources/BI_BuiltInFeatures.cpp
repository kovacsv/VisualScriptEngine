#include "BI_BuiltInFeatures.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_UINodeCommonParameters.hpp"

namespace BI
{

NE::SerializationInfo ValueCombinationFeature::serializationInfo (NE::ObjectVersion (1));
NE::SerializationInfo EnableDisableFeature::serializationInfo (NE::ObjectVersion (1));

static void SetNodeValueCombination (NE::ValueCombinationMode valueCombination, NUIE::NodeUIManager& uiManager, NUIE::UINodePtr& uiNode)
{
	std::shared_ptr<ValueCombinationFeature> featureNode = NE::Node::Cast<ValueCombinationFeature> (uiNode);
	if (DBGERROR (featureNode == nullptr)) {
		return;
	}
	featureNode->SetValueCombinationMode (valueCombination);
	uiManager.InvalidateNodeValue (uiNode);
	uiManager.InvalidateNodeDrawing (uiNode);
	uiManager.RequestRecalculate ();
}

class SetValueCombinationModeCommand : public NUIE::NodeCommand
{
public:
	SetValueCombinationModeCommand (const std::wstring& name, bool isChecked, NE::ValueCombinationMode valueCombination) :
		NUIE::NodeCommand (name, isChecked),
		valueCombination (valueCombination)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
	{
		return NE::Node::IsType<ValueCombinationFeature> (uiNode);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment&, NUIE::UINodePtr& uiNode) override
	{
		SetNodeValueCombination (valueCombination, uiManager, uiNode);
	}

private:
	NE::ValueCombinationMode valueCombination;
};

static void EnableDisableNode (bool enable, NUIE::NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode)
{
	std::shared_ptr<EnableDisableFeature> featureNode = NE::Node::Cast<EnableDisableFeature> (uiNode);
	if (DBGERROR (featureNode == nullptr)) {
		return;
	}
	featureNode->SetEnableState (enable, evaluationEnv);
	uiManager.InvalidateNodeDrawing (uiNode);
	uiManager.RequestRecalculate ();
}

class EnableDisableNodeCommand : public NUIE::NodeCommand
{
public:
	EnableDisableNodeCommand (const std::wstring& name, bool isChecked, bool enable) :
		NUIE::NodeCommand (name, isChecked),
		enable (enable)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
	{
		return NE::Node::IsType<EnableDisableFeature> (uiNode);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, NUIE::UINodePtr& uiNode) override
	{
		EnableDisableNode (enable, uiManager, uiEnvironment.GetEvaluationEnv (), uiNode);
	}

private:
	bool enable;
};

Feature::Feature ()
{

}

Feature::~Feature ()
{

}

ValueCombinationFeature::ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode) :
	valueCombinationMode (valueCombinationMode)
{

}

ValueCombinationFeature::~ValueCombinationFeature ()
{

}

NE::ValueCombinationMode ValueCombinationFeature::GetValueCombinationMode () const
{
	return valueCombinationMode;
}

void ValueCombinationFeature::SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode)
{
	valueCombinationMode = newValueCombinationMode;
}

bool ValueCombinationFeature::CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const
{
	return NE::CombineValues (valueCombinationMode, values, processor);
}

void ValueCombinationFeature::RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	NUIE::NodeGroupCommandPtr setValueCombinationModeGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (L"Set Value Combination"));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (L"Shortest", valueCombinationMode == NE::ValueCombinationMode::Shortest, NE::ValueCombinationMode::Shortest)));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (L"Longest", valueCombinationMode == NE::ValueCombinationMode::Longest, NE::ValueCombinationMode::Longest)));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (L"Cross Product", valueCombinationMode == NE::ValueCombinationMode::CrossProduct, NE::ValueCombinationMode::CrossProduct)));
	commandRegistrator.RegisterNodeGroupCommand (setValueCombinationModeGroup);
}

void ValueCombinationFeature::RegisterFeatureParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueCombinationParameter : public NUIE::EnumerationParameter<ValueCombinationFeature>
	{
	public:
		ValueCombinationParameter () :
			NUIE::EnumerationParameter<ValueCombinationFeature> (L"Value Combination", { L"Shortest", L"Longest", L"Cross Product" })
		{
		
		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodePtr& uiNode) const override
		{
			NE::ValueCombinationMode valueCombination = GetTypedNode (uiNode)->GetValueCombinationMode ();
			int valueCombinationInt = (int) valueCombination;
			return NE::ValuePtr (new NE::IntValue (valueCombinationInt));
		}
		
		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			int valueCombinationInt = NE::IntValue::Get (value);
			NE::ValueCombinationMode valueCombination = (NE::ValueCombinationMode) valueCombinationInt;
			SetNodeValueCombination (valueCombination, uiManager, uiNode);
			return true;
		}
	};

	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueCombinationParameter ()));
}

NE::Stream::Status ValueCombinationFeature::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	size_t valueCombinationModeInt = 0;
	inputStream.Read (valueCombinationModeInt);
	valueCombinationMode = (NE::ValueCombinationMode) valueCombinationModeInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status ValueCombinationFeature::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	size_t valueCombinationModeInt = (size_t) valueCombinationMode;
	outputStream.Write (valueCombinationModeInt);
	return outputStream.GetStatus ();
}

EnableDisableFeature::EnableDisableFeature (bool nodeEnabled) :
	nodeEnabled (nodeEnabled)
{

}

EnableDisableFeature::~EnableDisableFeature ()
{

}

bool EnableDisableFeature::GetEnableState () const
{
	return nodeEnabled;
}

void EnableDisableFeature::SetEnableState (bool isNodeEnabled, NE::EvaluationEnv& env)
{
	nodeEnabled = isNodeEnabled;
	if (nodeEnabled) {
		OnEnabled (env);
	} else {
		OnDisabled (env);
	}
}

void EnableDisableFeature::RegisterFeatureCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	NUIE::NodeGroupCommandPtr setNodeStatusGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (L"Set Node Status"));
	setNodeStatusGroup->AddChildCommand (NUIE::NodeCommandPtr (new EnableDisableNodeCommand (L"Enable", nodeEnabled, true)));
	setNodeStatusGroup->AddChildCommand (NUIE::NodeCommandPtr (new EnableDisableNodeCommand (L"Disable", !nodeEnabled, false)));
	commandRegistrator.RegisterNodeGroupCommand (setNodeStatusGroup);
}

void EnableDisableFeature::RegisterFeatureParameters (NUIE::NodeParameterList& parameterList) const
{
	class EnableDisableParameter : public NUIE::EnumerationParameter<EnableDisableFeature>
	{
	public:
		EnableDisableParameter () :
			NUIE::EnumerationParameter<EnableDisableFeature> (L"Status", { L"Enable", L"Disable" })
		{
		
		}

		virtual NE::ValuePtr GetValueInternal (const NUIE::UINodePtr& uiNode) const override
		{
			bool enableState = GetTypedNode (uiNode)->GetEnableState ();
			int enableStateInt = (enableState ? 0 : 1);
			return NE::ValuePtr (new NE::IntValue (enableStateInt));
		}
		
		virtual bool SetValueInternal (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			int enableStateInt = NE::IntValue::Get (value);
			bool enableState = (enableStateInt == 0 ? true : false);
			EnableDisableNode (enableState, uiManager, evaluationEnv, uiNode);
			return true;
		}
	};

	parameterList.AddParameter (NUIE::NodeParameterPtr (new EnableDisableParameter ()));
}

void EnableDisableFeature::CreateDrawingEnvironment (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const
{
	if (nodeEnabled) {
		drawer (env);
	} else {
		NUIE::ColorBlenderContextDecorator disabledContext (env.GetDrawingContext (), env.GetSkinParams ().GetBackgroundColor ());
		NUIE::NodeUIDrawingEnvironmentContextDecorator disabledEnv (env, disabledContext);
		drawer (disabledEnv);
	}
}

void EnableDisableFeature::FeatureProcessValue (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	if (nodeEnabled) {
		OnCalculated (value, env);
	}
}

NE::Stream::Status EnableDisableFeature::Read (NE::InputStream & inputStream)
{
	NE::ObjectHeader header (inputStream);
	inputStream.Read (nodeEnabled);
	return inputStream.GetStatus ();
}

NE::Stream::Status EnableDisableFeature::Write (NE::OutputStream & outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	outputStream.Write (nodeEnabled);
	return outputStream.GetStatus ();
}

}
