#ifndef BI_UNARYFUNCTIONNODES_HPP
#define BI_UNARYFUNCTIONNODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

namespace BI
{

class UnaryFunctionNode : public BasicUINode
{
	SERIALIZABLE;

public:
	UnaryFunctionNode ();
	UnaryFunctionNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~UnaryFunctionNode ();

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
		
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::ValuePtr				DoSingleOperation (const NE::ValueConstPtr& aValue) const;
	virtual double				DoOperation (double a) const = 0;
};

class AbsoluteValueNode : public UnaryFunctionNode
{
	DYNAMIC_SERIALIZABLE (AbsoluteValueNode);

public:
	AbsoluteValueNode ();
	AbsoluteValueNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~AbsoluteValueNode ();

private:
	virtual double DoOperation (double a) const override;
};

}

#endif
