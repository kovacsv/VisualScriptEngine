#ifndef ARITHMETICUINODES_HPP
#define ARITHMETICUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "BuiltInFeatures.hpp"

namespace NUIE
{

class BinaryOperationNode : public UINode,
							public ValueCombinationFeature
{
	SERIALIZABLE;

public:
	BinaryOperationNode ();
	BinaryOperationNode (const std::wstring& name, const Point& position);
	virtual ~BinaryOperationNode ();

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
		
	virtual void				RegisterParameters (NodeParameterList& parameterList) const override;
	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const override;

private:
	virtual double				DoOperation (double a, double b) const = 0;
};

class AdditionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (AdditionNode);

public:
	AdditionNode ();
	AdditionNode (const std::wstring& name, const Point& position);
	virtual ~AdditionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class SubtractionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (SubtractionNode);

public:
	SubtractionNode ();
	SubtractionNode (const std::wstring& name, const Point& position);
	virtual ~SubtractionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class MultiplicationNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (MultiplicationNode);

public:
	MultiplicationNode ();
	MultiplicationNode (const std::wstring& name, const Point& position);
	virtual ~MultiplicationNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

class DivisionNode : public BinaryOperationNode
{
	DYNAMIC_SERIALIZABLE (DivisionNode);

public:
	DivisionNode ();
	DivisionNode (const std::wstring& name, const Point& position);
	virtual ~DivisionNode ();

private:
	virtual double DoOperation (double a, double b) const override;
};

}

#endif
