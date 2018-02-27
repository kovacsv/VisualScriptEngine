#include "BuiltInFeatures.hpp"
#include "NodeUIManager.hpp"

namespace NUIE
{

NE::SerializationInfo ValueCombinationFeature::serializationInfo (NE::ObjectVersion (1));

class SetValueCombinationModeCommand2 : public NodeCommand
{
public:
	SetValueCombinationModeCommand2 (const std::wstring& name, bool isChecked, NE::ValueCombinationMode combinationMode) :
		NodeCommand (name, isChecked),
		combinationMode (combinationMode)
	{

	}

	virtual bool IsApplicableTo (const UINodePtr& uiNode) override
	{
		return NE::Node::IsType<ValueCombinationFeature> (uiNode);
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UINodePtr& uiNode) override
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

void ValueCombinationFeature::RegisterFeatureCommands (NodeCommandRegistrator& commandRegistrator) const
{
	NodeGroupCommandPtr setValueCombinationModeGroup (new NodeGroupCommand<NodeCommandPtr> (L"Set Value Combination"));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand2 (L"Shortest", valueCombinationMode == NE::ValueCombinationMode::Shortest, NE::ValueCombinationMode::Shortest)));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand2 (L"Longest", valueCombinationMode == NE::ValueCombinationMode::Longest, NE::ValueCombinationMode::Longest)));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand2 (L"Cross Product", valueCombinationMode == NE::ValueCombinationMode::CrossProduct, NE::ValueCombinationMode::CrossProduct)));
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

}
