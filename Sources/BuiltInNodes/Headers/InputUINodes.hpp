#ifndef INPUTUINODES_HPP
#define INPUTUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "BaseUINodes.hpp"

namespace NUIE
{

class NumberUpDownUINode : public UINode
{
	DYNAMIC_SERIALIZABLE (NumberUpDownUINode);

public:
	NumberUpDownUINode ();
	NumberUpDownUINode (const std::wstring& name, const Point& position, double val, double step);
	virtual ~NumberUpDownUINode ();

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NodeParameterList& parameterList) const;

	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const KeySet& pressedKeys, MouseButton mouseButton, const Point& position) override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	void		Increase ();
	void		Decrease ();

	double		GetValue () const;
	void		SetValue (double newValue);

	double		GetStep () const;
	void		SetStep (double newStep);

private:
	virtual void	UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const override;
	virtual void	CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;

	double			val;
	double			step;
};

class NumberRangeUINode : public HeaderWithSlotsUINode
{
	DYNAMIC_SERIALIZABLE (NumberRangeUINode);

public:
	NumberRangeUINode ();
	NumberRangeUINode (const std::wstring& name, const Point& position);
	virtual ~NumberRangeUINode ();
	
	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

}

#endif
