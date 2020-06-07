#ifndef NUIE_NODEPARAMETERS_HPP
#define NUIE_NODEPARAMETERS_HPP

#include "NE_LocString.hpp"
#include "NE_Value.hpp"
#include "NE_NodeCollection.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_UINodeInvalidator.hpp"
#include "NUIE_ParameterInterface.hpp"

namespace NUIE
{

class NodeUIManager;
class NodeUIEnvironment;

class NodeParameter
{
public:
	NodeParameter (const NE::LocString& name, const ParameterType& type);
	virtual ~NodeParameter ();

	const NE::LocString&				GetName () const;
	const ParameterType&				GetType () const;
	virtual std::vector<NE::LocString>	GetValueChoices () const;

	virtual NE::ValueConstPtr			GetValue (const UINodeConstPtr& uiNode) const = 0;
	virtual bool						IsApplicableTo (const UINodeConstPtr& uiNode) const = 0;
	virtual bool						CanSetValue (const UINodeConstPtr& uiNode, const NE::ValueConstPtr& value) const = 0;
	virtual bool						SetValue (UINodeInvalidator& invalidator, NE::EvaluationEnv& evaluationEnv, UINodePtr& uiNode, const NE::ValueConstPtr& value) = 0;

private:
	NE::LocString	name;
	ParameterType	type;
};

using NodeParameterPtr = std::shared_ptr<NodeParameter>;
using NodeParameterConstPtr = std::shared_ptr<const NodeParameter>;

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

using NodeParameterListPtr = std::shared_ptr<NodeParameterList>;
using NodeParameterListConstPtr = std::shared_ptr<const NodeParameterList>;

std::wstring	ParameterValueToString (const NE::ValueConstPtr& value, const ParameterType& type);
NE::ValuePtr	StringToParameterValue (const std::wstring& str, const ParameterType& type);

void			RegisterCommonParameters (const NodeUIManager& uiManager, const NE::NodeCollection& nodeCollection, NodeParameterList& parameterList);
bool			ApplyCommonParameter (NodeUIManager& uiManager, NE::EvaluationEnv& evaluationEnv, const NE::NodeCollection& nodeCollection, NodeParameterPtr& parameter, const NE::ValueConstPtr& value);

}

#endif
