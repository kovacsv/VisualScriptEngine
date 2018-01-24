#ifndef UINODEPARAMETERS_HPP
#define UINODEPARAMETERS_HPP

#include "UINode.hpp"
#include "Value.hpp"

namespace NUIE
{

class NodeCollection;
class NodeUIManager;
class NodeUIEnvironment;

class NodeParameter
{
public:
	NodeParameter (const std::string& paramId, const std::wstring& name);
	virtual ~NodeParameter ();

	const std::string&		GetId () const;
	const std::wstring&		GetName () const;

	virtual NE::ValuePtr	GetValue (const UINodePtr& uiNode) const = 0;
	virtual bool			IsApplicableTo (const UINodePtr& uiNode) const = 0;
	virtual bool			CanSetValue (const UINodePtr& uiNode, const NE::ValuePtr& value) const = 0;
	virtual bool			SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValuePtr& value) = 0;

private:
	std::string		paramId;
	std::wstring	name;
};

typedef std::shared_ptr<NodeParameter> NodeParameterPtr;
typedef std::shared_ptr<const NodeParameter> NodeParameterConstPtr;

template <typename NodeType, typename ValueType>
class TypedNodeParameter : public NodeParameter
{
public:
	TypedNodeParameter (const std::string& paramId, const std::wstring& name) :
		NodeParameter (paramId, name)
	{

	}

	virtual ~TypedNodeParameter ()
	{

	}

	virtual bool IsApplicableTo (const UINodePtr& uiNode) const
	{
		return NE::Node::IsType<NodeType> (uiNode);
	}

	virtual bool CanSetValue (const UINodePtr&, const NE::ValuePtr& value) const
	{
		return NE::Value::IsType<ValueType> (value);
	}
};

class NodeParameterList
{
public:
	NodeParameterList ();
	~NodeParameterList ();

	void						AddParameter (const NodeParameterPtr& parameter);

	size_t						GetParameterCount () const;
	NodeParameterPtr&			GetParameter (size_t index);
	const NodeParameterPtr&		GetParameter (size_t index) const;

private:
	std::vector<NodeParameterPtr>	parameters;
};

typedef std::shared_ptr<NodeParameterList> NodeParameterListPtr;
typedef std::shared_ptr<const NodeParameterList> NodeParameterListConstPtr;

void RegisterCommonParameters (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterList& parameterList);
bool ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValuePtr& value);

}

#endif
