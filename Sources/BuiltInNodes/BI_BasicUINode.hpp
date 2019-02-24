#ifndef BI_BASICUINODE_HPP
#define BI_BASICUINODE_HPP

#include "NUIE_UINode.hpp"

namespace BI
{

class BasicUINode : public NUIE::UINode
{
	SERIALIZABLE;

public:
	BasicUINode ();
	BasicUINode (const std::wstring& name, const NUIE::Point& position);
	virtual ~BasicUINode ();

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

private:
	virtual void UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;
};

}

#endif
