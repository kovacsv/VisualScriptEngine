#ifndef GEOMETRICNODES2D_HPP
#define GEOMETRICNODES2D_HPP

#include "GenericValue.hpp"
#include "UINode.hpp"
#include "BuiltInFeatures.hpp"

namespace BI
{

class Color
{
public:
	Color ();
	Color (unsigned char r, unsigned char g, unsigned char b);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class Transformation
{
public:
	Transformation ();
	Transformation (double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33);
	~Transformation ();

	std::wstring			ToString () const;
	NE::Stream::Status		Read (NE::InputStream& inputStream);
	NE::Stream::Status		Write (NE::OutputStream& outputStream) const;

	void					Apply (double& x, double& y) const;

	static Transformation	Translation (double x, double y);

private:
	double matrix[9];
};

class Point
{
public:
	Point ();
	Point (double x, double y);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Point				Transform (const Transformation& transformation) const;

	double	x;
	double	y;
};

class Line
{
public:
	Line ();
	Line (const Point& beg, const Point& end, const Color& color);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Line				Transform (const Transformation& transformation) const;

	Point	beg;
	Point	end;
	Color	color;
};

class Circle
{
public:
	Circle ();
	Circle (const Point& center, double radius, const Color& color);

	std::wstring		ToString () const;
	NE::Stream::Status	Read (NE::InputStream& inputStream);
	NE::Stream::Status	Write (NE::OutputStream& outputStream) const;

	Circle				Transform (const Transformation& transformation) const;

	Point	center;
	double	radius;
	Color	color;
};

class ColorValue : public NE::GenericValue<Color>
{
	DYNAMIC_SERIALIZABLE (ColorValue);

public:
	ColorValue ();
	ColorValue (const Color& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class TransformationValue : public NE::GenericValue<Transformation>
{
	DYNAMIC_SERIALIZABLE (TransformationValue);

public:
	TransformationValue ();
	TransformationValue (const Transformation& val);

	virtual NE::ValuePtr		Clone () const override;
	virtual std::wstring		ToString () const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

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

class TranslationMatrixNode :	public NUIE::UINode,
								public BI::ValueCombinationFeature
{
	DYNAMIC_SERIALIZABLE (TranslationMatrixNode);

public:
	TranslationMatrixNode ();
	TranslationMatrixNode (const std::wstring& name, const NUIE::Point& position);

	virtual void				RegisterSlots () override;
	virtual NE::ValuePtr		Calculate (NE::EvaluationEnv& env) const override;
	virtual void				RegisterParameters (NUIE::NodeParameterList& parameterList) const;
	virtual void				RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

protected:
	virtual void				UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;
};

}

#endif
