#ifndef BI_BASICUINODE_HPP
#define BI_BASICUINODE_HPP

#include "NUIE_UINode.hpp"
#include "BI_UINodeLayout.hpp"

namespace BI
{

class BasicUINode : public NUIE::UINode
{
	SERIALIZABLE;

public:
	BasicUINode ();
	BasicUINode (const NE::String& name, const NUIE::Point& position);
	BasicUINode (const NE::String& name, const NUIE::Point& position, const NUIE::IconId& iconId);
	BasicUINode (const NE::String& name, const NUIE::Point& position, const NUIE::IconId& iconId, const UINodeLayoutPtr& layout);
	virtual ~BasicUINode ();

	bool								HasIconId () const;
	const NUIE::IconId&					GetIconId () const;
	void								SetIconId (const NUIE::IconId& newIconId);

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

private:
	virtual NUIE::EventHandlerResult	HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;
	virtual NUIE::EventHandlerResult	HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;
	virtual void						UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

	NUIE::IconId		iconId;
	UINodeLayoutPtr		layout;
};

using BasicUINodePtr = std::shared_ptr<BasicUINode>;
using BasicUINodeConstPtr = std::shared_ptr<const BasicUINode>;

}

#endif
