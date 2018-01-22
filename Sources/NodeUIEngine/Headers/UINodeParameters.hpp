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
	virtual bool			CanSetValue (const UINodePtr& uiNode, NE::ValuePtr& value) const = 0;
	virtual bool			SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, NE::ValuePtr& value) = 0;

private:
	std::string		paramId;
	std::wstring	name;
};

typedef std::shared_ptr<NodeParameter> NodeParameterPtr;
typedef std::shared_ptr<const NodeParameter> NodeParameterConstPtr;

class NodeParameterList
{
public:
	NodeParameterList ();
	~NodeParameterList ();

	void				AddParameter (const NodeParameterPtr& parameter);

	size_t				GetParameterCount () const;
	NodeParameterPtr&	GetParameter (size_t index);
	void				EnumerateParameters (const std::function<void (NodeParameterPtr&)>& processor);

private:
	std::vector<NodeParameterPtr>	parameters;
};

void RegisterCommonParameters (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterList& parameterList);
bool ApplyCommonParameter (NodeUIManager& uiManager, const NodeCollection& nodeCollection, NodeParameterPtr& parameter, NE::EvaluationEnv& evaluationEnv, NE::ValuePtr& value);

}

#endif
