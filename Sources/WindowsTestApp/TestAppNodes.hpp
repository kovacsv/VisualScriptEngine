#ifndef TESTAPPNODES_HPP
#define TESTAPPNODES_HPP

#include "Node.hpp"
#include "UINode.hpp"
#include "InputSlot.hpp"
#include "OutputSlot.hpp"
#include "SingleValues.hpp"
#include "NodeUIManager.hpp"
#include "UINodeCommands.hpp"
#include "BaseUINodes.hpp"

class GeometricNode : public NUIE::EnableDisableNode
{
	SERIALIZABLE;

public:
	GeometricNode ();
	GeometricNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				OnCalculated (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnEnabled (const NE::ValuePtr& value, NE::EvaluationEnv& env) const override;
	virtual void				OnDisabled (NE::EvaluationEnv& env) const override;
	virtual void				OnDeleted (NE::EvaluationEnv& env) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const = 0;
	void								AddItem (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	void								RemoveItem (NE::EvaluationEnv& env) const;

	mutable NUIE::DrawingItemConstPtr	drawingItem;
};

class ColorNode : public NUIE::CombinedValueNode
{
	DYNAMIC_SERIALIZABLE (ColorNode);

public:
	ColorNode ();
	ColorNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;

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

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const override;
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

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const override;
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

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const override;
};

#endif
