#ifndef INPUTUINODES_HPP
#define INPUTUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"

namespace BIN
{

class NumericUpDownNode : public NUIE::UINode
{
	DYNAMIC_SERIALIZABLE (NumericUpDownNode);

public:
	NumericUpDownNode ();
	NumericUpDownNode (const std::wstring& name, const NUIE::Point& position, double val, double step);
	virtual ~NumericUpDownNode ();

	virtual void						RegisterSlots () override;
	virtual NE::ValuePtr				Calculate (NE::EvaluationEnv& env) const override;
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NUIE::EventHandlerResult	HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::KeySet& pressedKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position) override;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	void		Increase ();
	void		Decrease ();

	double		GetValue () const;
	void		SetValue (double newValue);

	double		GetStep () const;
	void		SetStep (double newStep);

private:
	virtual void	UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

	double			val;
	double			step;
};

class NumberRangeNode : public NUIE::UINode
{
	DYNAMIC_SERIALIZABLE (NumberRangeNode);

public:
	NumberRangeNode ();
	NumberRangeNode (const std::wstring& name, const NUIE::Point& position);
	virtual ~NumberRangeNode ();
	
	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;
};

}

#endif
