#ifndef TESTAPPNODES_HPP
#define TESTAPPNODES_HPP

#include "NE_SingleValues.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UINodeCommandRegistration.hpp"
#include "BI_BuiltInFeatures.hpp"

class ColorNode :	public NUIE::UINode,
					public BI::ValueCombinationFeature
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

class DrawableNode :	public NUIE::UINode,
						public BI::ValueCombinationFeature,
						public BI::EnableDisableFeature
{
	SERIALIZABLE;

public:
	DrawableNode ();
	DrawableNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual void				ProcessValue (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;

	virtual void				OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnEnabled (NE::EvaluationEnv& env) const override;
	virtual void				OnDisabled (NE::EvaluationEnv& env) const override;
	virtual void				OnDelete (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const override;
	virtual void				UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const;
	void						AddItem (NE::EvaluationEnv& env) const;
	void						RemoveItem (NE::EvaluationEnv& env) const;

	mutable NUIE::DrawingItemConstPtr	drawingItem;
};

class PointNode : public DrawableNode
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

class LineNode : public DrawableNode
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

class CircleNode : public DrawableNode
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

class OffsetNode : public DrawableNode
{
	DYNAMIC_SERIALIZABLE (OffsetNode);

public:
	OffsetNode ();
	OffsetNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
