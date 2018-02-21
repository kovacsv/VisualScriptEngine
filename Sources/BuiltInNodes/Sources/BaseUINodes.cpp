#include "BaseUINodes.hpp"
#include "UINodePanels.hpp"
#include "SkinParams.hpp"
#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"
#include "ContextDecorators.hpp"

namespace NUIE
{

NE::SerializationInfo CombinedValueNode::serializationInfo (NE::ObjectId ("{737B3D06-CB61-45BA-AB9E-7D7DF9C16B25}"), NE::ObjectVersion (1));
NE::SerializationInfo CalculationObserverNode::serializationInfo (NE::ObjectId ("{C5B07FFF-5F75-4A61-B40A-AA89F6B8A2E7}"), NE::ObjectVersion (1));

class SetValueCombinationModeCommand : public NodeCommand
{
public:
	SetValueCombinationModeCommand (const std::wstring& name, bool isChecked, NE::ValueCombinationMode combinationMode) :
		NodeCommand (name, isChecked),
		combinationMode (combinationMode)
	{

	}

	virtual bool IsApplicableTo (const UINodePtr& uiNode) override
	{
		return NE::Node::IsType<CombinedValueNode> (uiNode);
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UINodePtr& uiNode) override
	{
		std::shared_ptr<CombinedValueNode> combValueUINode = NE::Node::Cast<CombinedValueNode> (uiNode);
		if (DBGERROR (combValueUINode == nullptr)) {
			return;
		}
		combValueUINode->SetValueCombinationMode (combinationMode);
		uiManager.RequestRecalculate ();
		uiManager.InvalidateNodeDrawing (combValueUINode);
	}

private:
	NE::ValueCombinationMode combinationMode;
};

class EnableDisableNodeCommand : public NodeCommand
{
public:
	EnableDisableNodeCommand (const std::wstring& name, bool isChecked, bool enable) :
		NodeCommand (name, isChecked),
		enable (enable)
	{

	}

	virtual bool IsApplicableTo (const UINodePtr& uiNode) override
	{
		return NE::Node::IsType<CalculationObserverNode> (uiNode);
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UINodePtr& uiNode) override
	{
		std::shared_ptr<CalculationObserverNode> calcUINode = NE::Node::Cast<CalculationObserverNode> (uiNode);
		if (DBGERROR (calcUINode == nullptr)) {
			return;
		}
		calcUINode->SetEnableState (enable, uiEnvironment.GetEvaluationEnv ());
		uiManager.RequestRecalculate ();
	}

private:
	bool enable;
};

CombinedValueNode::CombinedValueNode () :
	UINode (),
	valueCombinationMode (NE::ValueCombinationMode::Longest)
{

}

CombinedValueNode::CombinedValueNode (const std::wstring& name, const Point& position) :
	UINode (name, position),
	valueCombinationMode (NE::ValueCombinationMode::Longest)
{

}

CombinedValueNode::~CombinedValueNode ()
{

}

void CombinedValueNode::SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode)
{
	valueCombinationMode = newValueCombinationMode;
	InvalidateValue ();
}

bool CombinedValueNode::CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const
{
	return NE::CombineValues (valueCombinationMode, values, processor);
}

void CombinedValueNode::RegisterCommands (NodeCommandRegistrator& commandRegistrator) const
{
	UINode::RegisterCommands (commandRegistrator);

	NodeGroupCommandPtr setValueCombinationModeGroup (new NodeGroupCommand<NodeCommandPtr> (L"Set Value Combination"));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand (L"Shortest", valueCombinationMode == NE::ValueCombinationMode::Shortest, NE::ValueCombinationMode::Shortest)));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand (L"Longest", valueCombinationMode == NE::ValueCombinationMode::Longest, NE::ValueCombinationMode::Longest)));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand (L"Cross Product", valueCombinationMode == NE::ValueCombinationMode::CrossProduct, NE::ValueCombinationMode::CrossProduct)));
	commandRegistrator.RegisterNodeGroupCommand (setValueCombinationModeGroup);
}

NE::Stream::Status CombinedValueNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	size_t valueCombinationModeInt = 0;
	inputStream.Read (valueCombinationModeInt);
	valueCombinationMode = (NE::ValueCombinationMode) valueCombinationModeInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status CombinedValueNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	size_t valueCombinationModeInt = (size_t) valueCombinationMode;
	outputStream.Write (valueCombinationModeInt);
	return outputStream.GetStatus ();
}

CalculationObserverNode::CalculationObserverNode () :
	CalculationObserverNode (L"", Point ())
{

}

CalculationObserverNode::CalculationObserverNode (const std::wstring& name, const Point& position) :
	CombinedValueNode (name, position),
	nodeEnabled (true)
{

}

CalculationObserverNode::~CalculationObserverNode ()
{

}

bool CalculationObserverNode::IsEnabled () const
{
	return nodeEnabled;
}

void CalculationObserverNode::SetEnableState (bool isEnabled, NE::EvaluationEnv& env)
{
	nodeEnabled = isEnabled;
	InvalidateDrawing ();
	if (nodeEnabled) {
		NE::ValuePtr value = GetCalculatedValue ();
		OnEnabled (value, env);
	} else {
		OnDisabled (env);
	}
}

void CalculationObserverNode::RegisterCommands (NodeCommandRegistrator& commandRegistrator) const
{
	CombinedValueNode::RegisterCommands (commandRegistrator);

	NodeGroupCommandPtr setNodeStatusGroup (new NodeGroupCommand<NodeCommandPtr> (L"Set Node Status"));
	setNodeStatusGroup->AddChildCommand (NodeCommandPtr (new EnableDisableNodeCommand (L"Enable", nodeEnabled, true)));
	setNodeStatusGroup->AddChildCommand (NodeCommandPtr (new EnableDisableNodeCommand (L"Disable", !nodeEnabled, false)));
	commandRegistrator.RegisterNodeGroupCommand (setNodeStatusGroup);
}

void CalculationObserverNode::OnCalculated (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

void CalculationObserverNode::OnEnabled (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

void CalculationObserverNode::OnDisabled (NE::EvaluationEnv&) const
{

}

void CalculationObserverNode::DrawInplace (NodeUIDrawingEnvironment& env) const
{
	if (nodeEnabled) {
		UINode::DrawInplace (env);
	} else {
		ColorBlenderContextDecorator disabledContext (env.GetDrawingContext (), env.GetSkinParams ().GetBackgroundColor ());
		NodeUIDrawingEnvironmentContextDecorator disabledEnv (env, disabledContext);
		UINode::DrawInplace (disabledEnv);
	}
}

void CalculationObserverNode::CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	if (nodeEnabled) {
		OnCalculated (value, env);
	}
}

NE::Stream::Status CalculationObserverNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	inputStream.Read (nodeEnabled);
	return inputStream.GetStatus ();
}

NE::Stream::Status CalculationObserverNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	outputStream.Write (nodeEnabled);
	return outputStream.GetStatus ();
}

}
