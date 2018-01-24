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

class Point
{
public:
	Point (int x, int y, int size);
	
	std::wstring ToString () const;

	int x;
	int y;
	int size;
};

class Line
{
public:
	Line (Point beg, Point end);

	std::wstring ToString () const;

	Point beg;
	Point end;
};

class Circle
{
public:
	Circle (Point center, int radius);

	std::wstring ToString () const;

	Point	center;
	int		radius;
};

class PointValue : public NE::GenericValue<Point>
{
public:
	PointValue (const Point& val);
	virtual std::wstring ToString () const;
};

class LineValue : public NE::GenericValue<Line>
{
public:
	LineValue (const Line& val);
	virtual std::wstring ToString () const;
}; 

class CircleValue : public NE::GenericValue<Circle>
{
public:
	CircleValue (const Circle& val);
	virtual std::wstring ToString () const;
}; 

class GeometricNode : public NUIE::CombinedValueUINode
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

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const = 0;
	void								AddItem (const NE::ValuePtr& value, NE::EvaluationEnv& env) const;
	void								RemoveItem (NE::EvaluationEnv& env) const;

	mutable NUIE::DrawingItemConstPtr	drawingItem;
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
	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;

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

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const override;
};


#endif
