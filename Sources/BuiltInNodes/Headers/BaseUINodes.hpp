#ifndef BASEUINODES_HPP
#define BASEUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "ValueCombination.hpp"
#include "UINodeLayouts.hpp"

namespace NUIE
{

// TODO: This is not ok, these are two orthogonal properties, should not inherit from each other

class CombinedValueNode : public UINode
{
	SERIALIZABLE;

public:
	CombinedValueNode ();
	CombinedValueNode (const std::wstring& name, const Point& position);
	virtual ~CombinedValueNode ();

	void						SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode);
	bool						CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const;

	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	NE::ValueCombinationMode valueCombinationMode;
};

class CalculationObserverNode : public CombinedValueNode
{
	SERIALIZABLE;

public:
	CalculationObserverNode ();
	CalculationObserverNode (const std::wstring& name, const Point& position);
	virtual ~CalculationObserverNode ();

	bool						IsEnabled () const;
	void						SetEnableState (bool isEnabled, NE::EvaluationEnv& env);

	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const override;

	virtual void				OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	virtual void				OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	virtual void				OnDisabled (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void				DrawInplace (NodeUIDrawingEnvironment& env) const override final;
	virtual void				CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;

	bool nodeEnabled;
};

}

#endif
