#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NE_SingleValues.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

NodeParameter::NodeParameter (const NE::LocString& name, const ParameterType& type) :
	name (name),
	type (type)
{

}

NodeParameter::~NodeParameter ()
{

}

const NE::LocString& NodeParameter::GetName () const
{
	return name;
}

const ParameterType& NodeParameter::GetType () const
{
	return type;
}

std::vector<NE::LocString> NodeParameter::GetValueChoices () const
{
	DBGBREAK ();
	return {};
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

static bool IsParameterApplicableTo (const NodeParameterPtr& parameter, const std::vector<UINodeConstPtr>& uiNodes)
{
	for (const UINodeConstPtr& uiNode : uiNodes) {
		if (!parameter->IsApplicableTo (uiNode)) {
			return false;
		}
	}
	return true;
}

std::wstring ParameterValueToString (const NE::ValueConstPtr& value, const ParameterType& type)
{
	std::wstring result;
	if (type == ParameterType::String) {
		if (DBGVERIFY (NE::Value::IsType<NE::StringValue> (value))) {
			result = NE::StringValue::Get (value);
		}
	} else if (type == ParameterType::Integer) {
		if (DBGVERIFY (NE::Value::IsType<NE::IntValue> (value))) {
			result = std::to_wstring (NE::IntValue::Get (value));
		}
	} else if (type == ParameterType::Float) {
		if (DBGVERIFY (NE::Value::IsType<NE::FloatValue> (value))) {
			result = std::to_wstring (NE::FloatValue::Get (value));
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
		} else if (type == ParameterType::Float) {
			result.reset (new NE::FloatValue (std::stof (str)));
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

void RegisterCommonParameters (const NodeUIManager& uiManager, const NE::NodeCollection& nodeCollection, NodeParameterList& parameterList)
{
	std::vector<UINodeConstPtr> uiNodes;
	nodeCollection.Enumerate ([&] (const NE::NodeId& nodeId) {
		UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
		if (DBGVERIFY (uiNode != nullptr)) {
			uiNodes.push_back (uiNode);
		}
		return true;
	});

	std::unordered_set<std::wstring> registeredParameterNames;
	for (const UINodeConstPtr& uiNode : uiNodes) {
		NodeParameterList parameters;
		uiNode->RegisterParameters (parameters);
		for (size_t paramIndex = 0; paramIndex < parameters.GetParameterCount (); ++paramIndex) {
			NodeParameterPtr& parameter = parameters.GetParameter (paramIndex);
			std::wstring parameterName = parameter->GetName ().GetLocalized ();
			if (registeredParameterNames.find (parameterName) != registeredParameterNames.end ()) {
				return;
			}
			registeredParameterNames.insert (parameterName);
			if (IsParameterApplicableTo (parameter, uiNodes)) {
				parameterList.AddParameter (parameter);
			}
		}
	}
}

bool ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NE::NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValueConstPtr& value)
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
		NodeUIManagerNodeInvalidator invalidator (uiManager, uiNode);
		if (DBGERROR (!parameter->SetValue (invalidator, evaluationEnv, uiNode, value))) {
			return false;
		}
	}

	return true;
}

}
