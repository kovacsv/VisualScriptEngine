#ifndef UINODEPARAMETERS_HPP
#define UINODEPARAMETERS_HPP

#include "UINode.hpp"
#include "UINodeParameterAccessor.hpp"
#include "Value.hpp"

namespace NUIE
{

class NodeCollection;
class NodeUIManager;
class NodeUIEnvironment;

class NodeParameter
{
public:
	NodeParameter (const std::string& paramId, const std::wstring& name, ParameterType type);
	virtual ~NodeParameter ();

	const std::string&		GetId () const;
	const std::wstring&		GetName () const;
	ParameterType			GetType () const;

	virtual NE::ValuePtr	GetValue (const UINodePtr& uiNode) const = 0;
	virtual bool			IsApplicableTo (const UINodePtr& uiNode) const = 0;
	virtual bool			CanSetValue (const UINodePtr& uiNode, const NE::ValuePtr& value) const = 0;
	virtual bool			SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValuePtr& value) = 0;

private:
	std::string		paramId;
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

std::wstring	ParameterValueToString (const NE::ValuePtr& value, ParameterType type);
NE::ValuePtr	StringToParameterValue (const std::wstring& str, ParameterType type);

void			RegisterCommonParameters (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterList& parameterList);
bool			ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValuePtr& value);

}

#endif
