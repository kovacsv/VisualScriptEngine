#ifndef BI_BINARYOPERATIONNODES_HPP
#define BI_BINARYOPERATIONNODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

namespace BI
{

class BinaryOperationNode : public BasicUINode
{
	SERIALIZABLE;

public:
	BinaryOperationNode ();
	BinaryOperationNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~BinaryOperationNode ();

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
		
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::ValuePtr				DoSingleOperation (const NE::ValueConstPtr& aValue, const NE::ValueConstPtr& bValue) const;
	virtual double				DoOperation (double a, double b) const = 0;
};

class AdditionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (AdditionNode);

public:
	AdditionNode ();
	AdditionNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~AdditionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class SubtractionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (SubtractionNode);

public:
	SubtractionNode ();
	SubtractionNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~SubtractionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class MultiplicationNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (MultiplicationNode);

public:
	MultiplicationNode ();
	MultiplicationNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~MultiplicationNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class DivisionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (DivisionNode);

public:
	DivisionNode ();
	DivisionNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~DivisionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

}

#endif
