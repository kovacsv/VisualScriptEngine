#include "UINodeParameters.hpp"
#include "NodeUIManager.hpp"
#include "SingleValues.hpp"
#include "Debug.hpp"

namespace NUIE
{

NodeParameter::NodeParameter (const std::wstring& name, const ParameterType& type) :
	name (name),
	type (type)
{

}

NodeParameter::~NodeParameter ()
{

}

const std::wstring& NodeParameter::GetName () const
{
	return name;
}

const ParameterType& NodeParameter::GetType () const
{
	return type;
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

const NodeParameterPtr& NodeParameterList::GetParameter (size_t index) const
{
	return parameters[index];
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

std::wstring ParameterValueToString (const NE::ValuePtr& value, const ParameterType& type)
{
	std::wstring result = L"";
	if (type == ParameterType::String) {
		if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
			result = NE::StringValue::Get (value);
		}
	} else if (type == ParameterType::Integer) {
		if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
			result = std::to_wstring (NE::IntValue::Get (value));
		}
	} else if (type == ParameterType::Double) {
		if (DBGVERIFY (NE::Value::IsType<NE::DoubleValue> (value))) {
			result = std::to_wstring (NE::DoubleValue::Get (value));
		}
	} else {
		DBGBREAK ();
	}
	return result;
}

NE::ValuePtr StringToParameterValue (const std::wstring& str, const ParameterType& type)
{
	NE::ValuePtr result = nullptr;
	try {
		if (type == ParameterType::String) {
			result.reset (new NE::StringValue (str));
		} else if (type == ParameterType::Integer) {
			result.reset (new NE::IntValue (std::stoi (str)));
		} else if (type == ParameterType::Double) {
			result.reset (new NE::DoubleValue (std::stod (str)));
		} else {
			DBGBREAK ();
		}
	} catch (...) {
		result = nullptr;
	}
	return result;
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

	std::unordered_set<std::wstring> registeredParameterNames;
	for (const UINodePtr& uiNode : uiNodes) {
		NodeParameterList parameters;
		uiNode->RegisterParameters (parameters);
		for (size_t paramIndex = 0; paramIndex < parameters.GetParameterCount (); ++paramIndex) {
			NodeParameterPtr& parameter = parameters.GetParameter (paramIndex);
			if (registeredParameterNames.find (parameter->GetName ()) != registeredParameterNames.end ()) {
				return;
			}
			registeredParameterNames.insert (parameter->GetName ());
			if (IsParameterApplicableTo (parameter, uiNodes)) {
				parameterList.AddParameter (parameter);
			}
		}
	}
}

bool ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValuePtr& value)
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
