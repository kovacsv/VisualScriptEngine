#include "BI_BuiltInFeatures.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_ContextDecorators.hpp"
#include "NUIE_SkinParams.hpp"
#include "NUIE_NodeCommonParameters.hpp"

namespace BI
{

const NUIE::FeatureId EnableDisableFeatureId ("{A60BA8C8-ADFB-48D2-A112-ADD99F0B6CE7}");
const NUIE::FeatureId ValueCombinationFeatureId ("{B8F03216-5CB8-49FC-B748-94479BD2C8CA}");

NE::DynamicSerializationInfo EnableDisableFeature::serializationInfo (NE::ObjectId ("{1C89FD8B-085E-45C8-B0B8-E75883F53C68}"), NE::ObjectVersion (1), EnableDisableFeature::CreateSerializableInstance);
NE::DynamicSerializationInfo ValueCombinationFeature::serializationInfo (NE::ObjectId ("{7BC21A4E-4D2E-4B00-BD73-9897DB3616BA}"), NE::ObjectVersion (1), ValueCombinationFeature::CreateSerializableInstance);

static void EnableDisableNode (EnableDisableFeature::State state, EnableDisableFeature::Mode mode, NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv& env, NUIE::UINodePtr& uiNode)
{
	std::shared_ptr<EnableDisableFeature> enableDisableFeature = GetEnableDisableFeature (uiNode);
	if (DBGERROR (enableDisableFeature == nullptr)) {
		return;
	}
	enableDisableFeature->SetState (state);
	uiNode->OnFeatureChange (EnableDisableFeatureId, env);
	if (mode == EnableDisableFeature::Mode::Invalidate) {
		invalidator.InvalidateValue ();
	}
	invalidator.InvalidateDrawing ();
	invalidator.RequestRecalculate ();
}

class EnableDisableNodeCommand : public NUIE::NodeCommand
{
public:
	EnableDisableNodeCommand (const std::wstring& name, bool isChecked, EnableDisableFeature::State state, EnableDisableFeature::Mode mode) :
		NUIE::NodeCommand (name, isChecked),
		state (state),
		mode (mode)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodeConstPtr& uiNode) override
	{
		return uiNode->HasFeature (EnableDisableFeatureId);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, NUIE::UINodePtr& uiNode) override
	{
		NUIE::NodeUIManagerNodeInvalidator invalidator (uiManager, uiNode);
		EnableDisableNode (state, mode, invalidator, uiEnvironment.GetEvaluationEnv (), uiNode);
	}

private:
	EnableDisableFeature::State		state;
	EnableDisableFeature::Mode		mode;
};

static void SetNodeValueCombination (NE::ValueCombinationMode valueCombination, NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv& env, NUIE::UINodePtr& uiNode)
{
	std::shared_ptr<ValueCombinationFeature> valueCombinationFeature = GetValueCombinationFeature (uiNode);
	if (DBGERROR (valueCombinationFeature == nullptr)) {
		return;
	}
	valueCombinationFeature->SetValueCombinationMode (valueCombination);
	uiNode->OnFeatureChange (ValueCombinationFeatureId, env);
	invalidator.InvalidateValueAndDrawing ();
}

class SetValueCombinationModeCommand : public NUIE::NodeCommand
{
public:
	SetValueCombinationModeCommand (const std::wstring& name, bool isChecked, NE::ValueCombinationMode valueCombination) :
		NUIE::NodeCommand (name, isChecked),
		valueCombination (valueCombination)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodeConstPtr& uiNode) override
	{
		return uiNode->HasFeature (ValueCombinationFeatureId);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, NUIE::UINodePtr& uiNode) override
	{
		NUIE::NodeUIManagerNodeInvalidator invalidator (uiManager, uiNode);
		SetNodeValueCombination (valueCombination, invalidator, env.GetEvaluationEnv (), uiNode);
	}

private:
	NE::ValueCombinationMode valueCombination;
};

EnableDisableFeature::EnableDisableFeature () :
	EnableDisableFeature (State::Enabled, Mode::DoNotInvalidate)
{

}

EnableDisableFeature::EnableDisableFeature (State state, Mode mode) :
	NUIE::NodeFeature (EnableDisableFeatureId),
	state (state),
	mode (mode)
{

}

EnableDisableFeature::~EnableDisableFeature ()
{

}

EnableDisableFeature::State EnableDisableFeature::GetState () const
{
	return state;
}

EnableDisableFeature::Mode EnableDisableFeature::GetMode () const
{
	return mode;
}

void EnableDisableFeature::SetState (State newState)
{
	state = newState;
}

void EnableDisableFeature::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	NUIE::NodeGroupCommandPtr setNodeStatusGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (NE::Localize (L"Set Node Status")));
	setNodeStatusGroup->AddChildCommand (NUIE::NodeCommandPtr (new EnableDisableNodeCommand (NE::Localize (L"Enable"), state == State::Enabled, State::Enabled, mode)));
	setNodeStatusGroup->AddChildCommand (NUIE::NodeCommandPtr (new EnableDisableNodeCommand (NE::Localize (L"Disable"), state == State::Disabled, State::Disabled, mode)));
	commandRegistrator.RegisterNodeGroupCommand (setNodeStatusGroup);
}

void EnableDisableFeature::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class EnableDisableParameter : public NUIE::EnumerationFeatureParameter<EnableDisableFeature>
	{
	public:
		EnableDisableParameter () :
			NUIE::EnumerationFeatureParameter<EnableDisableFeature> (NE::Localize (L"Status"), { NE::Localize (L"Enable"), NE::Localize (L"Disable") }, EnableDisableFeatureId)
		{

		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			EnableDisableFeature::State state = GetEnableDisableFeature (uiNode)->GetState ();
			int stateInt = (state == EnableDisableFeature::State::Enabled ? 0 : 1);
			return NE::ValuePtr (new NE::IntValue (stateInt));
		}

		virtual bool SetValueInternal (NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			int stateInt = NE::IntValue::Get (value);
			EnableDisableFeature::State enableState = (stateInt == 0 ? EnableDisableFeature::State::Enabled : EnableDisableFeature::State::Disabled);
			EnableDisableFeature::Mode mode = GetEnableDisableFeature (uiNode)->GetMode ();
			EnableDisableNode (enableState, mode, invalidator, evaluationEnv, uiNode);
			return true;
		}
	};

	parameterList.AddParameter (NUIE::NodeParameterPtr (new EnableDisableParameter ()));
}

void EnableDisableFeature::DrawInplace (NUIE::NodeUIDrawingEnvironment& env, const std::function<void (NUIE::NodeUIDrawingEnvironment&)>& drawer) const
{
	if (state == State::Enabled) {
		drawer (env);
	} else {
		NUIE::ColorBlenderContextDecorator disabledContext (env.GetDrawingContext (), env.GetSkinParams ().GetDisabledBlendColor ());
		NUIE::NodeUIDrawingEnvironmentContextDecorator disabledEnv (env, disabledContext);
		drawer (disabledEnv);
	}
}

NE::Stream::Status EnableDisableFeature::Read (NE::InputStream & inputStream)
{
	NE::ObjectHeader header (inputStream);
	NodeFeature::Read (inputStream);
	ReadEnum (inputStream, state);
	ReadEnum (inputStream, mode);
	return inputStream.GetStatus ();
}

NE::Stream::Status EnableDisableFeature::Write (NE::OutputStream & outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NodeFeature::Write (outputStream);
	WriteEnum (outputStream, state);
	WriteEnum (outputStream, mode);
	return outputStream.GetStatus ();
}

ValueCombinationFeature::ValueCombinationFeature () :
	ValueCombinationFeature (NE::ValueCombinationMode::Longest)
{

}

ValueCombinationFeature::ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode) :
	NUIE::NodeFeature (ValueCombinationFeatureId),
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

bool ValueCombinationFeature::CombineValues (const std::vector<NE::ValueConstPtr>& values, const std::function<bool (const NE::ValueCombination&)>& processor) const
{
	return NE::CombineValues (valueCombinationMode, values, processor);
}

void ValueCombinationFeature::RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const
{
	NUIE::NodeGroupCommandPtr setValueCombinationModeGroup (new NUIE::NodeGroupCommand<NUIE::NodeCommandPtr> (NE::Localize (L"Set Value Combination")));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (NE::Localize (L"Shortest"), valueCombinationMode == NE::ValueCombinationMode::Shortest, NE::ValueCombinationMode::Shortest)));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (NE::Localize (L"Longest"), valueCombinationMode == NE::ValueCombinationMode::Longest, NE::ValueCombinationMode::Longest)));
	setValueCombinationModeGroup->AddChildCommand (NUIE::NodeCommandPtr (new SetValueCombinationModeCommand (NE::Localize (L"Cross Product"), valueCombinationMode == NE::ValueCombinationMode::CrossProduct, NE::ValueCombinationMode::CrossProduct)));
	commandRegistrator.RegisterNodeGroupCommand (setValueCombinationModeGroup);
}

void ValueCombinationFeature::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class ValueCombinationParameter : public NUIE::EnumerationFeatureParameter<ValueCombinationFeature>
	{
	public:
		ValueCombinationParameter () :
			NUIE::EnumerationFeatureParameter<ValueCombinationFeature> (NE::Localize (L"Value Combination"), { NE::Localize (L"Shortest"), NE::Localize (L"Longest"), NE::Localize (L"Cross Product") }, ValueCombinationFeatureId)
		{
		
		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			NE::ValueCombinationMode valueCombination = GetValueCombinationFeature (uiNode)->GetValueCombinationMode ();
			int valueCombinationInt = (int) valueCombination;
			return NE::ValuePtr (new NE::IntValue (valueCombinationInt));
		}
		
		virtual bool SetValueInternal (NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv& evaluationEnv, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			int valueCombinationInt = NE::IntValue::Get (value);
			NE::ValueCombinationMode valueCombination = (NE::ValueCombinationMode) valueCombinationInt;
			SetNodeValueCombination (valueCombination, invalidator, evaluationEnv, uiNode);
			return true;
		}
	};

	parameterList.AddParameter (NUIE::NodeParameterPtr (new ValueCombinationParameter ()));
}

NE::Stream::Status ValueCombinationFeature::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NodeFeature::Read (inputStream);
	size_t valueCombinationModeInt = 0;
	inputStream.Read (valueCombinationModeInt);
	valueCombinationMode = (NE::ValueCombinationMode) valueCombinationModeInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status ValueCombinationFeature::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NodeFeature::Write (outputStream);
	size_t valueCombinationModeInt = (size_t) valueCombinationMode;
	outputStream.Write (valueCombinationModeInt);
	return outputStream.GetStatus ();
}

std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const NUIE::UINode* uiNode)
{
	return NUIE::GetUINodeFeature<EnableDisableFeature> (uiNode, EnableDisableFeatureId);
}

std::shared_ptr<EnableDisableFeature> GetEnableDisableFeature (const NUIE::UINodeConstPtr& uiNode)
{
	return NUIE::GetUINodeFeature<EnableDisableFeature> (uiNode, EnableDisableFeatureId);
}

std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const NUIE::UINode* uiNode)
{
	return NUIE::GetUINodeFeature<ValueCombinationFeature> (uiNode, ValueCombinationFeatureId);
}

std::shared_ptr<ValueCombinationFeature> GetValueCombinationFeature (const NUIE::UINodeConstPtr& uiNode)
{
	return NUIE::GetUINodeFeature<ValueCombinationFeature> (uiNode, ValueCombinationFeatureId);
}

bool CombineValues (const NUIE::UINode* uiNode, const std::vector<NE::ValueConstPtr>& values, const std::function<bool (const NE::ValueCombination&)>& processor)
{
	std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (uiNode);
	if (DBGERROR (valueCombination == nullptr)) {
		return false;
	}
	return valueCombination->CombineValues (values, processor);
}

bool CombineValues (const NUIE::UINodeConstPtr& uiNode, const std::vector<NE::ValueConstPtr>& values, const std::function<bool (const NE::ValueCombination&)>& processor)
{
	std::shared_ptr<ValueCombinationFeature> valueCombination = GetValueCombinationFeature (uiNode);
	if (DBGERROR (valueCombination == nullptr)) {
		return false;
	}
	return valueCombination->CombineValues (values, processor);
}

}
