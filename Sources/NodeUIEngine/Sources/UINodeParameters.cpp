#include "UINodeParameters.hpp"
#include "NodeUIManager.hpp"
#include "Debug.hpp"

namespace NUIE
{

NodeParameter::NodeParameter (const std::string& paramId, const std::wstring& name) :
	paramId (paramId),
	name (name)
{

}

NodeParameter::~NodeParameter ()
{

}

const std::string& NodeParameter::GetId () const
{
	return paramId;
}

const std::wstring& NodeParameter::GetName () const
{
	return name;
}

NodeParameterList::NodeParameterList ()
{

}

NodeParameterList::~NodeParameterList ()
{

}

void NodeParameterList::AddParameter (const NodeParameterPtr& parameter)
{
	parameters.push_back (parameter);
}

size_t NodeParameterList::GetParameterCount () const
{
	return parameters.size ();
}

NodeParameterPtr& NodeParameterList::GetParameter (size_t index)
{
	return parameters[index];
}

void NodeParameterList::EnumerateParameters (const std::function<void (NodeParameterPtr&)>& processor)
{
	for (NodeParameterPtr& parameter : parameters) {
		processor (parameter);
	}
}

static bool IsParameterApplicableTo (NodeParameterPtr& parameter, const std::vector<UINodePtr>& uiNodes)
{
	for (const UINodePtr& uiNode : uiNodes) {
		if (!parameter->IsApplicableTo (uiNode)) {
			return false;
		}
	}
	return true;
}

void RegisterCommonParameters (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterList& parameterList)
{
	std::vector<UINodePtr> uiNodes;
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodePtr uiNode = uiManager.GetUINode (nodeId);
		if (DBGVERIFY (uiNode != nullptr)) {
			uiNodes.push_back (uiNode);
		}
		return true;
	});

	std::unordered_set<std::string> registeredParameters;
	for (const UINodePtr& uiNode : uiNodes) {
		NodeParameterList parameters;
		uiNode->RegisterParameters (parameters);
		parameters.EnumerateParameters ([&] (NodeParameterPtr& parameter) {
			if (registeredParameters.find (parameter->GetId ()) != registeredParameters.end ()) {
				return;
			}
			registeredParameters.insert (parameter->GetId ());
			if (IsParameterApplicableTo (parameter, uiNodes)) {
				parameterList.AddParameter (parameter);
			}
		});
	}
}

bool ApplyCommonParameter (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterPtr& parameter, NE::EvaluationEnv& evaluationEnv, NE::ValuePtr& value)
{
	std::vector<UINodePtr> uiNodes;
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodePtr uiNode = uiManager.GetUINode (nodeId);
		if (DBGVERIFY (uiNode != nullptr)) {
			uiNodes.push_back (uiNode);
		}
		return true;
	});

	for (const UINodePtr& uiNode : uiNodes) {
		if (DBGERROR (!parameter->IsApplicableTo (uiNode))) {
			return false;
		}
		if (!parameter->CanSetValue (uiNode, value)) {
			return false;
		}
	}

	for (UINodePtr& uiNode : uiNodes) {
		if (DBGERROR (!parameter->SetValue (uiManager, evaluationEnv, uiNode, value))) {
			return false;
		}
	}

	return true;
}

}
