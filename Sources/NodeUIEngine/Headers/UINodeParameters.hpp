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
	enum class Type
	{
		String,
		Integer
	};

	NodeParameter (const std::string& paramId, const std::wstring& name, Type type);
	virtual ~NodeParameter ();

	const std::string&		GetId () const;
	const std::wstring&		GetName () const;
	Type					GetType () const;

	virtual NE::ValuePtr	GetValue (const UINodePtr& uiNode) const = 0;
	virtual bool			IsApplicableTo (const UINodePtr& uiNode) const = 0;
	virtual bool			CanSetValue (const UINodePtr& uiNode, const NE::ValuePtr& value) const = 0;
	virtual bool			SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValuePtr& value) = 0;

private:
	std::string		paramId;
	std::wstring	name;
	Type			type;
};

typedef std::shared_ptr<NodeParameter> NodeParameterPtr;
typedef std::shared_ptr<const NodeParameter> NodeParameterConstPtr;

template <typename NodeType, typename ValueType>
class TypedNodeParameter : public NodeParameter
{
public:
	TypedNodeParameter (const std::string& paramId, const std::wstring& name, NodeParameter::Type type) :
		NodeParameter (paramId, name, type)
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

template <typename NodeType, typename ValueType>
class SlotDefaultValueParameter : public TypedNodeParameter<NodeType, ValueType>
{
public:
	SlotDefaultValueParameter (const std::string& paramId, const std::wstring& name, NodeParameter::Type type, const NE::SlotId& slotId) :
		TypedNodeParameter<NodeType, ValueType> (paramId, name, type),
		slotId (slotId)
	{

	}

	virtual NE::ValuePtr GetValue (const NUIE::UINodePtr& uiNode) const override
	{
		NE::InputSlotConstPtr inputSlot = uiNode->GetInputSlot (slotId);
		if (DBGERROR (inputSlot == nullptr)) {
			return nullptr;
		}
		return inputSlot->GetDefaultValue ();
	}

	virtual bool SetValue (NUIE::NodeUIManager& uiManager, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValuePtr& value) override
	{
		if (DBGERROR (!CanSetValue (uiNode, value))) {
			return false;
		}
		uiNode->GetInputSlot (slotId)->SetDefaultValue (value);
		uiNode->InvalidateValue ();
		uiManager.RequestRecalculate ();
		return true;
	}

private:
	NE::SlotId slotId;
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

class NodeParameterAccessor
{
public:
	NodeParameterAccessor ();
	virtual ~NodeParameterAccessor ();

	virtual size_t					GetParameterCount () const = 0;
	virtual const std::wstring&		GetParameterName (size_t index) const = 0;
	virtual NE::ValuePtr			GetParameterValue (size_t index) const = 0;
	virtual NodeParameter::Type		GetParameterType (size_t index) const = 0;
	virtual bool					SetParameterValue (size_t index, const NE::ValuePtr& value) = 0;
};

typedef std::shared_ptr<NodeParameterAccessor> NodeParameterAccessorPtr;
typedef std::shared_ptr<const NodeParameterAccessor> NodeParameterAccessorConstPtr;

std::wstring	ParameterValueToString (const NE::ValuePtr& value, NodeParameter::Type type);
NE::ValuePtr	StringToParameterValue (const std::wstring& str, NodeParameter::Type type);

void			RegisterCommonParameters (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterList& parameterList);
bool			ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValuePtr& value);

}

#endif
