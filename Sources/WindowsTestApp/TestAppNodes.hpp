#ifndef TESTAPPNODES_HPP
#define TESTAPPNODES_HPP

#include "Node.hpp"
#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "NodeUIManager.hpp"
#include "UINodeCommands.hpp"
#include "BuiltInFeatures.hpp"

class GeometricNode :	public NUIE::UINode,
						public NUIE::ValueCombinationFeature,
						public NUIE::EnableDisableFeature
{
	SERIALIZABLE;

public:
	GeometricNode ();
	GeometricNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void				CalculationPostProcess (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;

	virtual void				OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnDisabled (NE::EvaluationEnv& env) const override;
	virtual void				OnDeleted (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void				UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const;
	void						AddItem (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	void						RemoveItem (NE::EvaluationEnv& env) const;

	mutable NUIE::DrawingItemConstPtr	drawingItem;
};

class ColorNode :	public NUIE::UINode,
					public NUIE::ValueCombinationFeature
{
	DYNAMIC_SERIALIZABLE (ColorNode);

public:
	ColorNode ();
	ColorNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;
	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;
};

class PointNode : public GeometricNode
{
	DYNAMIC_SERIALIZABLE (PointNode);

public:
	PointNode ();
	PointNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class LineNode : public GeometricNode
{
	DYNAMIC_SERIALIZABLE (LineNode);

public:
	LineNode ();
	LineNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class CircleNode : public GeometricNode
{
	DYNAMIC_SERIALIZABLE (CircleNode);

public:
	CircleNode ();
	CircleNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
