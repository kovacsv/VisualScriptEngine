#include "BaseUINodes.hpp"
#include "UINodePanels.hpp"
#include "SkinParams.hpp"
#include "UINodeCommands.hpp"
#include "NodeUIManager.hpp"
#include "ContextDecorators.hpp"

namespace NUIE
{

NE::SerializationInfo NormalUINode::serializationInfo (NE::ObjectId ("{163B3E73-9ACF-40AB-994D-EDD0B1614309}"), NE::ObjectVersion (1));
NE::SerializationInfo CalculatedUINode::serializationInfo (NE::ObjectId ("{C5B07FFF-5F75-4A61-B40A-AA89F6B8A2E7}"), NE::ObjectVersion (1));
NE::SerializationInfo CombinedValueUINode::serializationInfo (NE::ObjectId ("{737B3D06-CB61-45BA-AB9E-7D7DF9C16B25}"), NE::ObjectVersion (1));

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
		return std::dynamic_pointer_cast<CalculatedUINode> (uiNode) != nullptr;
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment& uiEnvironment, UINodePtr& uiNode) override
	{
		std::shared_ptr<CalculatedUINode> calcUINode = std::dynamic_pointer_cast<CalculatedUINode> (uiNode);
		if (DBGERROR (calcUINode == nullptr)) {
			return;
		}
		calcUINode->SetEnableState (enable, uiEnvironment.GetEvaluationEnv ());
		uiManager.RequestRedraw ();
	}

private:
	bool enable;
};

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
		return std::dynamic_pointer_cast<CombinedValueUINode> (uiNode) != nullptr;
	}

	virtual void Do (NodeUIManager& uiManager, NodeUIEnvironment&, UINodePtr& uiNode) override
	{
		std::shared_ptr<CombinedValueUINode> combValueUINode = std::dynamic_pointer_cast<CombinedValueUINode> (uiNode);
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

NormalUINode::NormalUINode () :
	UINode ()
{

}

NormalUINode::NormalUINode (const std::wstring& name, const Point& position) :
	UINode (name, position)
{

}

NormalUINode::~NormalUINode ()
{

}

void NormalUINode::UpdateNodeDrawingImage (NodeUIEnvironment& env, NodeDrawingImage& drawingImage) const
{
	NodeUIPanelDrawer drawer;
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIStatusHeaderPanel (GetNodeName (), GetStatus (env.GetEvaluationEnv ()))));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUISlotPanel (*this, env)));
	drawer.Draw (env, drawingImage);
}

void NormalUINode::CalculationPostProcess (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

NE::Stream::Status NormalUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status NormalUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	return outputStream.GetStatus ();
}

CalculatedUINode::CalculatedUINode () :
	NormalUINode (),
	nodeEnabled (false)
{

}

CalculatedUINode::CalculatedUINode (const std::wstring& name, const Point& position) :
	NormalUINode (name, position),
	nodeEnabled (true)
{

}

CalculatedUINode::~CalculatedUINode ()
{

}

bool CalculatedUINode::IsEnabled () const
{
	return nodeEnabled;
}

void CalculatedUINode::SetEnableState (bool isEnabled, NE::EvaluationEnv& env)
{
	nodeEnabled = isEnabled;
	InvalidateDrawing ();
	if (nodeEnabled) {
		NE::ValuePtr value = Evaluate (env);
		OnEnabled (value, env);
	} else {
		OnDisabled (env);
	}
}

void CalculatedUINode::Draw (NodeUIEnvironment& env) const
{
	if (nodeEnabled) {
		UINode::Draw (env);
	} else {
		ColorBlenderContextDecorator disabledContext (env.GetDrawingContext (), env.GetSkinParams ().GetBackgroundColor ());
		NodeUIEnvironmentContextDecorator disabledEnv (env, disabledContext);
		UINode::Draw (disabledEnv);
	}
}

void CalculatedUINode::RegisterCommands (NodeCommandRegistrator& commandRegistrator) const
{
	NormalUINode::RegisterCommands (commandRegistrator);

	NodeGroupCommandPtr setNodeStatusGroup (new NodeGroupCommand<NodeCommandPtr> (L"Set Node Status"));
	setNodeStatusGroup->AddChildCommand (NodeCommandPtr (new EnableDisableNodeCommand (L"Enable", nodeEnabled, true)));
	setNodeStatusGroup->AddChildCommand (NodeCommandPtr (new EnableDisableNodeCommand (L"Disable", !nodeEnabled, false)));
	commandRegistrator.RegisterNodeGroupCommand (setNodeStatusGroup);
}

void CalculatedUINode::OnCalculated (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

void CalculatedUINode::OnEnabled (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

void CalculatedUINode::OnDisabled (NE::EvaluationEnv&) const
{

}

void CalculatedUINode::CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const
{
	if (nodeEnabled) {
		OnCalculated (value, env);
	}
}

NE::Stream::Status CalculatedUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NormalUINode::Read (inputStream);
	inputStream.Read (nodeEnabled);
	return inputStream.GetStatus ();
}

NE::Stream::Status CalculatedUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NormalUINode::Write (outputStream);
	outputStream.Write (nodeEnabled);
	return outputStream.GetStatus ();
}

CombinedValueUINode::CombinedValueUINode () :
	CalculatedUINode (),
	valueCombinationMode (NE::ValueCombinationMode::Longest)
{

}

CombinedValueUINode::CombinedValueUINode (const std::wstring& name, const Point& position) :
	CalculatedUINode (name, position),
	valueCombinationMode (NE::ValueCombinationMode::Longest)
{

}

CombinedValueUINode::~CombinedValueUINode ()
{

}

void CombinedValueUINode::SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode)
{
	valueCombinationMode = newValueCombinationMode;
	InvalidateValue ();
}

bool CombinedValueUINode::CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const
{
	return NE::CombineValues (valueCombinationMode, values, processor);
}

void CombinedValueUINode::RegisterCommands (NodeCommandRegistrator& commandRegistrator) const
{
	CalculatedUINode::RegisterCommands (commandRegistrator);

	NodeGroupCommandPtr setValueCombinationModeGroup (new NodeGroupCommand<NodeCommandPtr> (L"Set Value Combination"));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand (L"Shortest", valueCombinationMode == NE::ValueCombinationMode::Shortest, NE::ValueCombinationMode::Shortest)));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand (L"Longest", valueCombinationMode == NE::ValueCombinationMode::Longest, NE::ValueCombinationMode::Longest)));
	setValueCombinationModeGroup->AddChildCommand (NodeCommandPtr (new SetValueCombinationModeCommand (L"Cross Product", valueCombinationMode == NE::ValueCombinationMode::CrossProduct, NE::ValueCombinationMode::CrossProduct)));
	commandRegistrator.RegisterNodeGroupCommand (setValueCombinationModeGroup);
}

NE::Stream::Status CombinedValueUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	CalculatedUINode::Read (inputStream);
	size_t valueCombinationModeInt = 0;
	inputStream.Read (valueCombinationModeInt);
	valueCombinationMode = (NE::ValueCombinationMode) valueCombinationModeInt;
	return inputStream.GetStatus ();
}

NE::Stream::Status CombinedValueUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	CalculatedUINode::Write (outputStream);
	size_t valueCombinationModeInt = (size_t) valueCombinationMode;
	outputStream.Write (valueCombinationModeInt);
	return outputStream.GetStatus ();
}

}
