#ifndef BASEUINODES_HPP
#define BASEUINODES_HPP

#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "ValueCombination.hpp"

namespace NUIE
{

// TODO: better name
class NormalUINode : public UINode
{
	SERIALIZABLE;

public:
	NormalUINode ();
	NormalUINode (const std::wstring& name, const Point& position);
	virtual ~NormalUINode ();

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void UpdateNodeDrawingImage (NodeUIEnvironment& env, NodeDrawingImage& drawingImage) const override;
};

class CalculatedUINode : public NormalUINode
{
	SERIALIZABLE;

public:
	CalculatedUINode ();
	CalculatedUINode (const std::wstring& name, const Point& position);
	virtual ~CalculatedUINode ();

	bool						IsEnabled () const;
	void						SetEnableState (bool isEnabled, NE::EvaluationEnv& env);

	virtual void				Draw (NodeUIEnvironment& env) const override final;

	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const override;

	virtual void				OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	virtual void				OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	virtual void				OnDisabled (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void	CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;

	bool nodeEnabled;
};

class CombinedValueUINode : public CalculatedUINode
{
	SERIALIZABLE;

public:
	CombinedValueUINode ();
	CombinedValueUINode (const std::wstring& name, const Point& position);
	virtual ~CombinedValueUINode ();

	void						SetValueCombinationMode (NE::ValueCombinationMode newValueCombinationMode);
	bool						CombineValues (const std::vector<NE::ValuePtr>& values, const std::function<void (const NE::ValueCombination&)>& processor) const;

	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;


private:
	NE::ValueCombinationMode valueCombinationMode;
};

}

#endif
