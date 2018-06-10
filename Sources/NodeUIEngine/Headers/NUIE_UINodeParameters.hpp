#ifndef UINODEPARAMETERS_HPP
#define UINODEPARAMETERS_HPP

#include "NE_Value.hpp"
#include "NE_NodeCollection.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_ParameterInterface.hpp"

namespace NUIE
{

class NodeUIManager;
class NodeUIEnvironment;

class NodeParameter
{
public:
	NodeParameter (const std::wstring& name, const ParameterType& type);
	virtual ~NodeParameter ();

	const std::wstring&		GetName () const;
	const ParameterType&	GetType () const;

	virtual NE::ValuePtr	GetValue (const UINodePtr& uiNode) const = 0;
	virtual bool			IsApplicableTo (const UINodePtr& uiNode) const = 0;
	virtual bool			CanSetValue (const UINodePtr& uiNode, const NE::ValuePtr& value) const = 0;
	virtual bool			SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValuePtr& value) = 0;

private:
	std::wstring	name;
	ParameterType	type;
};

typedef std::shared_ptr<NodeParameter> NodeParameterPtr;
typedef std::shared_ptr<const NodeParameter> NodeParameterConstPtr;

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

std::wstring	ParameterValueToString (const NE::ValuePtr& value, const ParameterType& type);
NE::ValuePtr	StringToParameterValue (const std::wstring& str, const ParameterType& type);

void			RegisterCommonParameters (NodeUIManager& uiManager, const NE::NodeCollection& nodeCollection, NodeParameterList& parameterList);
bool			ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NE::NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValuePtr& value);

}

#endif
