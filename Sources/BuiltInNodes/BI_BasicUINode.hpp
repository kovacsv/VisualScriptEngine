#ifndef BI_BASICUINODE_HPP
#define BI_BASICUINODE_HPP

#include "NUIE_UINode.hpp"
#include "BI_UINodeLayout.hpp"
#include "BI_NodeFeatureSet.hpp"

namespace BI
{

class BasicUINode : public NUIE::UINode
{
	SERIALIZABLE;

public:
	BasicUINode ();
	BasicUINode (const NE::LocString& name, const NUIE::Point& position);
	BasicUINode (const NE::LocString& name, const NUIE::Point& position, const UINodeLayoutPtr& layout);
	BasicUINode (const NE::LocString& name, const NUIE::Point& position, const UINodeLayoutPtr& layout, const NUIE::IconId& iconId);
	virtual ~BasicUINode ();

	bool								HasIconId () const;
	const NUIE::IconId&					GetIconId () const;
	void								SetIconId (const NUIE::IconId& newIconId);

	bool								HasFeature (const FeatureId& featureId) const;
	const NodeFeaturePtr&				GetFeature (const FeatureId& featureId) const;

	virtual NE::Stream::Status			Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status			Write (NE::OutputStream& outputStream) const override;

	virtual void						OnFeatureChange (const FeatureId& featureId, NE::EvaluationEnv& env) const;

protected:
	virtual void						RegisterParameters (NUIE::NodeParameterList& parameterList) const override;
	virtual void						RegisterCommands (NUIE::NodeCommandRegistrator& commandRegistrator) const override;
	bool								RegisterFeature (const NodeFeaturePtr& newFeature);

private:
	virtual NUIE::EventHandlerResult	HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;
	virtual NUIE::EventHandlerResult	HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& modifierKeys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface) override;
	virtual void						UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const override;

	UINodeLayoutPtr		layout;
	NUIE::IconId		iconId;
	NodeFeatureSet		nodeFeatureSet;
};

using BasicUINodePtr = std::shared_ptr<BasicUINode>;
using BasicUINodeConstPtr = std::shared_ptr<const BasicUINode>;

template <class FeatureType>
std::shared_ptr<FeatureType> GetUINodeFeature (const BasicUINode* uiNode, const FeatureId& featureId)
{
	if (DBGERROR (!uiNode->HasFeature (featureId))) {
		return nullptr;
	}
	return NodeFeature::Cast<FeatureType> (uiNode->GetFeature (featureId));
}

template <class FeatureType>
std::shared_ptr<FeatureType> GetUINodeFeature (const BasicUINodeConstPtr& uiNode, const FeatureId& featureId)
{
	if (DBGERROR (!uiNode->HasFeature (featureId))) {
		return nullptr;
	}
	return NodeFeature::Cast<FeatureType> (uiNode->GetFeature (featureId));
}

}

#endif
