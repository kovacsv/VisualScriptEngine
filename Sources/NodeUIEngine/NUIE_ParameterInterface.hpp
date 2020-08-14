#ifndef NUIE_PARAMETERINTERFACE_HPP
#define NUIE_PARAMETERINTERFACE_HPP

#include "NE_Value.hpp"
#include "NUIE_NodeUIManager.hpp"

namespace NUIE
{

class ParameterType
{
public:
	ParameterType (const std::string& id);

	const std::string&	GetId () const;
	bool				operator== (const ParameterType& rhs) const;
	bool				operator!= (const ParameterType& rhs) const;

	static const ParameterType Undefined;
	static const ParameterType Boolean;
	static const ParameterType Integer;
	static const ParameterType Float;
	static const ParameterType Double;
	static const ParameterType String;
	static const ParameterType Enumeration;

private:
	std::string id;
};

class ParameterInterface
{
public:
	ParameterInterface ();
	virtual ~ParameterInterface ();

	virtual size_t						GetParameterCount () const = 0;
	virtual std::wstring				GetParameterName (size_t index) const = 0;
	virtual NE::ValueConstPtr			GetParameterValue (size_t index) const = 0;
	virtual std::vector<std::wstring>	GetParameterValueChoices (size_t index) const = 0;
	virtual const ParameterType&		GetParameterType (size_t index) const = 0;
	virtual bool						IsValidParameterValue (size_t index, const NE::ValueConstPtr& value) const = 0;
	virtual bool						SetParameterValue (size_t index, const NE::ValueConstPtr& value) = 0;
	virtual void						ApplyChanges (NodeUIManager& uiManager, NE::EvaluationEnv& evalEnv) = 0;
};

using ParameterInterfacePtr = std::shared_ptr<ParameterInterface>;
using ParameterInterfaceConstPtr = std::shared_ptr<const ParameterInterface>;

}

#endif
