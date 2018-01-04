#include "UINodeParameters.hpp"
#include "Debug.hpp"

namespace NUIE
{

NodeParameter::NodeParameter (const std::wstring& name) :
	name (name)
{

}

NodeParameter::~NodeParameter ()
{

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

}
