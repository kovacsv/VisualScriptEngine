#include "UINodeCommonParameters.hpp"

namespace NUIE
{

NodeNameParameter::NodeNameParameter () :
	TypedNodeParameter<UINode, NE::StringValue> ("NodeNameParameter", L"Name", ParameterType::String)
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
