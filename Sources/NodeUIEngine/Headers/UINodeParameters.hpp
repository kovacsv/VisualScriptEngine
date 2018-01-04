#ifndef UINODEPARAMETERS_HPP
#define UINODEPARAMETERS_HPP

#include "UINode.hpp"
#include "Value.hpp"

namespace NUIE
{

class NodeUIManager;
class NodeUIEnvironment;

class NodeParameter
{
public:
	NodeParameter (const std::wstring& name);
	virtual ~NodeParameter ();

	const std::wstring&		GetName () const;

	virtual NE::ValuePtr	GetValue (const UINodePtr& uiNode) const = 0;
	virtual bool			IsApplicableTo (const UINodePtr& uiNode) const = 0;
	virtual bool			SetValue (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, NE::ValuePtr& value) = 0;

private:
	std::wstring name;
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

}

#endif
