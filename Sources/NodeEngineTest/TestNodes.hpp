#ifndef TESTNODES_HPP
#define TESTNODES_HPP

#include "NUIE_UINode.hpp"

using namespace NE;
using namespace NUIE;

class SerializableTestNode : public Node
{
	DYNAMIC_SERIALIZABLE (SerializableTestNode);

public:
	SerializableTestNode ();

	virtual void				Initialize () override;
	virtual ValueConstPtr		Calculate (NE::EvaluationEnv&) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

class SerializableTestUINode : public UINode
{
	DYNAMIC_SERIALIZABLE (SerializableTestUINode);

public:
	SerializableTestUINode ();
	SerializableTestUINode (const LocString& nodeName, const Point& nodePosition);

	virtual void				Initialize () override;
	virtual ValueConstPtr		Calculate (NE::EvaluationEnv&) const override;
	virtual void				UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const override;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;
};

#endif
