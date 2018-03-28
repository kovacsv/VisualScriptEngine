#include "BuiltInFeatures.hpp"
#include "NodeUIManager.hpp"
#include "ContextDecorators.hpp"
#include "SkinParams.hpp"

namespace BIN
{

NE::SerializationInfo ValueCombinationFeature::serializationInfo (NE::ObjectVersion (1));
NE::SerializationInfo EnableDisableFeature::serializationInfo (NE::ObjectVersion (1));

class SetValueCombinationModeCommand : public NUIE::NodeCommand
{
public:
	SetValueCombinationModeCommand (const std::wstring& name, bool isChecked, NE::ValueCombinationMode combinationMode) :
		NUIE::NodeCommand (name, isChecked),
		combinationMode (combinationMode)
	{

	}

	virtual bool IsApplicableTo (const NUIE::UINodePtr& uiNode) override
	{
		return NE::Node::IsType<ValueCombinationFeature> (uiNode);
	}

	virtual void Do (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment&, NUIE::UINodePtr& uiNode) override
	{
		std::shared_ptr<ValueCombinationFeature> featureNode = NE::Node::Cast<ValueCombinationFeature> (uiNode);
		if (DBGERROR (featureNode == nullptr)) {
			return;
		}
		featureNode->SetValueCombinationMode (combinationMode);
		uiManager.InvalidateNodeValue (uiNode);
		uiManager.InvalidateNodeDrawing (uiNode);
		uiManager.RequestRecalculate ();
	}

private:
	NE::ValueCombinationMode combinationMode;
};

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
		std::shared_ptr<EnableDisableFeature> featureNode = NE::Node::Cast<EnableDisableFeature> (uiNode);
		if (DBGERROR (featureNode == nullptr)) {
			return;
		}
		NE::ValuePtr value = uiNode->GetCalculatedValue ();
		featureNode->SetEnableState (enable, value, uiEnvironment.GetEvaluationEnv ());
		uiManager.InvalidateNodeDrawing (uiNode);
		uiManager.RequestRecalculate ();
	}

private:
	bool enable;
};

ValueCombinationFeature::ValueCombinationFeature (NE::ValueCombinationMode valueCombinationMode) :
	valueCombinationMode (valueCombinationMode)
{

}

ValueCombinationFeature::~ValueCombinationFeature ()
{

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

void EnableDisableFeature::SetEnableState (bool isNodeEnabled, const NE::ValuePtr& value, NE::EvaluationEnv& env)
{
	nodeEnabled = isNodeEnabled;
	if (nodeEnabled) {
		OnEnabled (value, env);
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

void EnableDisableFeature::FeatureCalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
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
