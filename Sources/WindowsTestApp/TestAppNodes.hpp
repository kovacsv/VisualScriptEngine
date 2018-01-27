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

class Color
{
public:
	Color ();
	Color (unsigned char r, unsigned char g, unsigned char b);

	std::wstring ToString () const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Point
{
public:
	Point ();
	Point (int x, int y, int size, Color color);
	
	std::wstring ToString () const;

	int		x;
	int		y;
	int		size;
	Color	color;
};

class Line
{
public:
	Line ();
	Line (Point beg, Point end, Color color);

	std::wstring ToString () const;

	Point	beg;
	Point	end;
	Color	color;
};

class Circle
{
public:
	Circle ();
	Circle (Point center, int radius, Color color);

	std::wstring ToString () const;

	Point	center;
	int		radius;
	Color	color;
};

// TODO: IO for these values

class ColorValue : public NE::GenericValue<Color>
{
	DYNAMIC_SERIALIZABLE (ColorValue);

public:
	ColorValue ();
	ColorValue (const Color& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class PointValue : public NE::GenericValue<Point>
{
	DYNAMIC_SERIALIZABLE (PointValue);

public:
	PointValue ();
	PointValue (const Point& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class LineValue : public NE::GenericValue<Line>
{
	DYNAMIC_SERIALIZABLE (LineValue);

public:
	LineValue ();
	LineValue (const Line& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
}; 

class CircleValue : public NE::GenericValue<Circle>
{
	DYNAMIC_SERIALIZABLE (CircleValue);

public:
	CircleValue ();
	CircleValue (const Circle& val);
	virtual std::wstring ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
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

class ColorNode : public NUIE::CombinedValueUINode
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

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual NUIE::DrawingItemConstPtr	CreateDrawingItem (const NE::ValuePtr& value) const override;
};


#endif
