#ifndef ARITHMETICUINODES_HPP
#define ARITHMETICUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "BaseUINodes.hpp"

namespace NUIE
{

class AdditionNode : public HeaderWithSlotsUINode
{
	DYNAMIC_SERIALIZABLE (AdditionNode);

public:
	AdditionNode ();
	AdditionNode (const std::wstring& name, const Point& position);
	virtual ~AdditionNode ();

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

}

#endif
