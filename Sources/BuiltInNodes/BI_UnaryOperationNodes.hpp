#ifndef BI_UNARYOPERATIONNODES_HPP
#define BI_UNARYOPERATIONNODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

namespace BI
{

class UnaryOperationNode : public BasicUINode
{
	SERIALIZABLE;

public:
	UnaryOperationNode ();
	UnaryOperationNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~UnaryOperationNode ();

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
		
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::ValuePtr				DoSingleOperation (const NE::ValueConstPtr& aValue) const;
	virtual bool				IsValidInput (double a) const;
	virtual double				DoOperation (double a) const = 0;
};

class AbsNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (AbsNode);

public:
	AbsNode ();
	AbsNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~AbsNode ();

private:
	virtual double DoOperation (double a) const override;
};

class FloorNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (FloorNode);

public:
	FloorNode ();
	FloorNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~FloorNode ();

private:
	virtual double DoOperation (double a) const override;
};

class CeilNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (CeilNode);

public:
	CeilNode ();
	CeilNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~CeilNode ();

private:
	virtual double DoOperation (double a) const override;
};

class NegativeNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (NegativeNode);

public:
	NegativeNode ();
	NegativeNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~NegativeNode ();

private:
	virtual double DoOperation (double a) const override;
};

class SqrtNode : public UnaryOperationNode
{
	DYNAMIC_SERIALIZABLE (SqrtNode);

public:
	SqrtNode ();
	SqrtNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~SqrtNode ();

private:
	virtual bool	IsValidInput (double a) const override;
	virtual double	DoOperation (double a) const override;
};

}

#endif
