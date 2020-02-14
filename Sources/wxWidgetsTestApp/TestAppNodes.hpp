#ifndef TESTAPPNODES_HPP
#define TESTAPPNODES_HPP

#include "NE_SingleValues.hpp"
#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeMenuCommandRegistrator.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"

class ColorNode : public BI::BasicUINode
{
	DYNAMIC_SERIALIZABLE (ColorNode);

public:
	ColorNode ();
	ColorNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class DrawableNode : public BI::BasicUINode
{
	SERIALIZABLE;

public:
	DrawableNode ();
	DrawableNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual void				ProcessCalculatedValue (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnFeatureChange (const NUIE::FeatureId& featureId, NE::EvaluationEnv& env) const override;
	virtual void				OnDelete (NE::EvaluationEnv& env) const override;

	void						OnCalculated (const NE::ValueConstPtr& value, NE::EvaluationEnv& env) const;
	void						OnEnabled (NE::EvaluationEnv& env) const;
	void						OnDisabled (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void				DrawInplace (NUIE::NodeUIDrawingEnvironment& env) const override;
	NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValueConstPtr& value) const;
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

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
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

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class CircleNode : public DrawableNode
{
	DYNAMIC_SERIALIZABLE (CircleNode);

public:
	CircleNode ();
	CircleNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
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

	virtual void				Initialize () override;
	virtual NE::ValueConstPtr	Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
