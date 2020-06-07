#ifndef BI_INPUTUINODES_HPP
#define BI_INPUTUINODES_HPP

#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_UINodeLayouts.hpp"

namespace BI
{

class BooleanNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (BooleanNode);

public:
	class Layout : public HeaderWithSlotsAndSwitchLayout
	{
	public:
		Layout (const std::string& switchButtonId,
				const std::wstring& firstSwitchText,
				const std::wstring& secondSwitchText);

		virtual HeaderWithSlotsAndSwitchLayout::SelectedItem					GetSelectedItem (const BasicUINode& uiNode) const override;
		virtual std::shared_ptr<HeaderWithSlotsAndSwitchLayout::ClickHandler>	GetClickHandler (BasicUINode& uiNode) const override;
	};

	BooleanNode ();
	BooleanNode (const NE::LocString& name, const NUIE::Point& position, bool val);
	virtual ~BooleanNode ();

	virtual void						Initialize () override;
	virtual bool						IsForceCalculated () const override;

	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	bool								GetValue () const;
	void								SetValue (bool newVal);

private:
	bool								val;
};

class NumericUpDownNode : public BasicUINode
{
	SERIALIZABLE;

public:
	class Layout : public HeaderWithSlotsAndButtonsLayout
	{
	public:
		Layout (const std::string& leftButtonId,
				const std::wstring& leftButtonText,
				const std::string& rightButtonId,
				const std::wstring& rightButtonText);

		virtual std::wstring													GetMiddleText (const BasicUINode& uiNode, const NE::StringConverter& stringConverter) const override;
		virtual std::shared_ptr<HeaderWithSlotsAndButtonsLayout::ClickHandler>	GetClickHandler (BasicUINode& uiNode) const override;
	};

	NumericUpDownNode ();
	NumericUpDownNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~NumericUpDownNode ();

	virtual void						Initialize () override;

	virtual bool						IsForceCalculated () const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	virtual void						Increase () = 0;
	virtual void						Decrease () = 0;
};

class IntegerUpDownNode : public NumericUpDownNode
{
	DYNAMIC_SERIALIZABLE (IntegerUpDownNode);

public:
	IntegerUpDownNode ();
	IntegerUpDownNode (const NE::LocString& name, const NUIE::Point& position, int val, int step);
	virtual ~IntegerUpDownNode ();

	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;
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
	DoubleUpDownNode (const NE::LocString& name, const NUIE::Point& position, double val, double step);
	virtual ~DoubleUpDownNode ();

	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;
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
	NumericRangeNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~NumericRangeNode ();

	virtual bool				IsForceCalculated () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class IntegerIncrementedNode : public NumericRangeNode
{
	DYNAMIC_SERIALIZABLE (IntegerIncrementedNode);

public:
	IntegerIncrementedNode ();
	IntegerIncrementedNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~IntegerIncrementedNode ();
	
	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class DoubleIncrementedNode : public NumericRangeNode
{
	DYNAMIC_SERIALIZABLE (DoubleIncrementedNode);

public:
	DoubleIncrementedNode ();
	DoubleIncrementedNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~DoubleIncrementedNode ();
	
	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class DoubleDistributedNode : public NumericRangeNode
{
	DYNAMIC_SERIALIZABLE (DoubleDistributedNode);

public:
	DoubleDistributedNode ();
	DoubleDistributedNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~DoubleDistributedNode ();

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class ListBuilderNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (ListBuilderNode);

public:
	ListBuilderNode ();
	ListBuilderNode (const NE::LocString& name, const NUIE::Point& position);
	virtual ~ListBuilderNode ();

	virtual void						Initialize () override;
	virtual NE::ValueConstPtr			Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;
};

}

#endif
