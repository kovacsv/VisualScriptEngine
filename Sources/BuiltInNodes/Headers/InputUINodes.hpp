#ifndef INPUTUINODES_HPP
#define INPUTUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "BaseUINodes.hpp"

namespace NUIE
{

class NumericUpDownUINode : public UINode
{
	SERIALIZABLE;

public:
	NumericUpDownUINode ();
	NumericUpDownUINode (const std::wstring& name, const Point& position);
	virtual ~NumericUpDownUINode ();

	virtual void				Increase () = 0;
	virtual void				Decrease () = 0;

	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void	UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const override;
	virtual void	CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;
};

class IntegerUpDownUINode : public NumericUpDownUINode
{
	DYNAMIC_SERIALIZABLE (IntegerUpDownUINode);

public:
	IntegerUpDownUINode ();
	IntegerUpDownUINode (const std::wstring& name, const Point& position, int val, int step);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NodeParameterList& parameterList) const;

	virtual void				Increase () override;
	virtual void				Decrease () override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	int							GetValue () const;
	void						SetValue (int newValue);

private:
	int val;
	int step;
};

class DoubleUpDownUINode : public NumericUpDownUINode
{
	DYNAMIC_SERIALIZABLE (DoubleUpDownUINode);

public:
	DoubleUpDownUINode ();
	DoubleUpDownUINode (const std::wstring& name, const Point& position, double val, double step);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual void				Increase () override;
	virtual void				Decrease () override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	double val;
	double step;
};

class IntegerRangeNode : public NormalUINode
{
	DYNAMIC_SERIALIZABLE (IntegerRangeNode);

public:
	IntegerRangeNode ();
	IntegerRangeNode (const std::wstring& name, const Point& position);
	virtual ~IntegerRangeNode ();
	
	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

}

#endif
