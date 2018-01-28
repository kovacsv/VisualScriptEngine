#include "UINodeCommonParameters.hpp"

namespace NUIE
{

NodeNameParameter::NodeNameParameter () :
	NotEmptyStringParameter<UINode> ("NodeNameParameter", L"Name")
{

}

NE::ValuePtr NodeNameParameter::GetValue (const UINodePtr& uiNode) const
{
	return NE::ValuePtr (new NE::StringValue (uiNode->GetNodeName ()));
}

bool NodeNameParameter::SetValue (NodeUIManager& uiManager, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValuePtr& value)
{
	if (DBGERROR (!CanSetValue (uiNode, value))) {
		return false;
	}
	uiNode->SetNodeName (NE::StringValue::Get (value));
	uiNode->InvalidateDrawing ();
	uiManager.RequestRedraw ();
	return true;
}

}
