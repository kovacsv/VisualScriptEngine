#ifndef BI_INPUTUINODES_HPP
#define BI_INPUTUINODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"

namespace BI
{

class NumericUpDownNode : public NUIE::UINode
{
	SERIALIZABLE;

public:
	NumericUpDownNode ();
	NumericUpDownNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~NumericUpDownNode ();

	virtual void						Initialize () override;

	virtual NUIE::EventHandlerResult	HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;
	virtual NUIE::EventHandlerResult	HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;

	virtual bool						IsForceCalculated () const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	virtual void						Increase () = 0;
	virtual void						Decrease () = 0;

private:
	virtual void						UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;
};

class IntegerUpDownNode : public NumericUpDownNode
{
	DYNAMIC_SERIALIZABLE (IntegerUpDownNode);

public:
	IntegerUpDownNode ();
	IntegerUpDownNode (const std::wstring& name, const NUIE::Point& position, int val, int step);
	virtual ~IntegerUpDownNode ();

	virtual NE::ValuePtr				Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	virtual void						Increase () override;
	virtual void						Decrease () override;

	int									GetValue () const;
	void								SetValue (int newValue);

	int									GetStep () const;
	void								SetStep (int newStep);

private:
	int			val;
	int			step;
};

class DoubleUpDownNode : public NumericUpDownNode
{
	DYNAMIC_SERIALIZABLE (DoubleUpDownNode);

public:
	DoubleUpDownNode ();
	DoubleUpDownNode (const std::wstring& name, const NUIE::Point& position, double val, double step);
	virtual ~DoubleUpDownNode ();

	virtual NE::ValuePtr				Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	virtual void						Increase () override;
	virtual void						Decrease () override;

	double								GetValue () const;
	void								SetValue (double newValue);

	double								GetStep () const;
	void								SetStep (double newStep);

private:
	double			val;
	double			step;
};

class NumericRangeNode : public BasicUINode
{
	SERIALIZABLE;

public:
	NumericRangeNode ();
	NumericRangeNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~NumericRangeNode ();

	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class IntegerRangeNode : public NumericRangeNode
{
	DYNAMIC_SERIALIZABLE (IntegerRangeNode);

public:
	IntegerRangeNode ();
	IntegerRangeNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~IntegerRangeNode ();
	
	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class DoubleRangeNode : public NumericRangeNode
{
	DYNAMIC_SERIALIZABLE (DoubleRangeNode);

public:
	DoubleRangeNode ();
	DoubleRangeNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~DoubleRangeNode ();
	
	virtual void				Initialize () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

}

#endif
