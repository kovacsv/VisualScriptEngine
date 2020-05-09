#ifndef NUIE_UINODE_HPP
#define NUIE_UINODE_HPP

#include "NE_String.hpp"
#include "NE_Node.hpp"
#include "NUIE_NodeDrawingImage.hpp"
#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_InputEventHandler.hpp"
#include "NUIE_UIInputSlot.hpp"
#include "NUIE_UIOutputSlot.hpp"
#include "NUIE_NodeFeatureSet.hpp"
#include <string>

namespace NUIE
{

class NodeUIEnvironment;
class NodeParameterList;
class NodeCommandRegistrator;
class NodeFeature;

class UINodeCommandInterface
{
public:
	UINodeCommandInterface ();
	virtual ~UINodeCommandInterface ();

	virtual void RunUndoableCommand (const std::function<void ()>& func) = 0;
};

class UINode : public NE::Node
{
	SERIALIZABLE;

public:
	UINode ();
	UINode (const std::wstring& nodeName, const Point& nodePosition);
	virtual ~UINode ();

	std::wstring				GetNodeName () const;
	void						SetNodeName (const std::wstring& newNodeName);

	const Point&				GetNodePosition () const;
	void						SetNodePosition (const Point& newPosition);

	void						Draw (NodeUIDrawingEnvironment& env) const;

	Rect						GetNodeRect (NodeUIDrawingEnvironment& env) const;
	void						InvalidateDrawing () const;

	Point						GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Point						GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetInputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;
	Rect						GetOutputSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	bool						HasSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const;
	Rect						GetSpecialRect (NodeUIDrawingEnvironment& env, const std::string& rectId) const;

	UIInputSlotConstPtr			GetUIInputSlot (const NE::SlotId& slotId) const;
	UIOutputSlotConstPtr		GetUIOutputSlot (const NE::SlotId& slotId) const;

	void						EnumerateUIInputSlots (const std::function<bool (const UIInputSlotConstPtr&)>& processor) const;
	void						EnumerateUIOutputSlots (const std::function<bool (const UIOutputSlotConstPtr&)>& processor) const;

	bool						HasFeature (const FeatureId& featureId) const;
	const NodeFeaturePtr&		GetFeature (const FeatureId& featureId) const;

	virtual EventHandlerResult	HandleMouseClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position, UINodeCommandInterface& commandInterface);
	virtual EventHandlerResult	HandleMouseDoubleClick (NodeUIEnvironment& env, const ModifierKeys& modifierKeys, MouseButton mouseButton, const Point& position, UINodeCommandInterface& commandInterface);

	virtual void				RegisterParameters (NodeParameterList& parameterList) const;
	virtual void				RegisterCommands (NodeCommandRegistrator& commandRegistrator) const;
	virtual void				OnFeatureChange (const FeatureId& featureId, NE::EvaluationEnv& env) const;

	virtual void				OnAdd (NE::EvaluationEnv& env) const;
	virtual void				OnDelete (NE::EvaluationEnv& env) const;

	virtual NE::Stream::Status	Read (NE::InputStream& inputStream) override;
	virtual NE::Stream::Status	Write (NE::OutputStream& outputStream) const override;

	template <class SlotType>
	Point GetSlotConnPosition (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	template <class SlotType>
	bool HasSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	template <class SlotType>
	Rect GetSlotRect (NodeUIDrawingEnvironment& env, const NE::SlotId& slotId) const;

	template <class SlotConstType>
	void EnumerateUISlots (const std::function<bool (const SlotConstType&)>& processor) const;

protected:
	virtual void				DrawInplace (NodeUIDrawingEnvironment& env) const;
	bool						RegisterUIInputSlot (const UIInputSlotPtr& newInputSlot);
	bool						RegisterUIOutputSlot (const UIOutputSlotPtr& newOutputSlot);
	bool						RegisterFeature (const NodeFeaturePtr& newFeature);

private:
	const NodeDrawingImage&		GetNodeDrawingImage (NodeUIDrawingEnvironment& env) const;
	virtual void				UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const = 0;

	virtual bool				RegisterInputSlot (const NE::InputSlotPtr& newInputSlot) override;
	virtual bool				RegisterOutputSlot (const NE::OutputSlotPtr& newOutputSlot) override;

	NE::String					nodeName;
	Point						nodePosition;
	UINodeFeatureSet			nodeFeatureSet;
	mutable NodeDrawingImage	nodeDrawingImage;
};

using UINodePtr = std::shared_ptr<UINode>;
using UINodeConstPtr = std::shared_ptr<const UINode>;

template <class FeatureType>
std::shared_ptr<FeatureType> GetUINodeFeature (const UINode* uiNode, const FeatureId& featureId)
{
	return NodeFeature::Cast<FeatureType> (uiNode->GetFeature (featureId));
}

template <class FeatureType>
std::shared_ptr<FeatureType> GetUINodeFeature (const UINodeConstPtr& uiNode, const FeatureId& featureId)
{
	return NodeFeature::Cast<FeatureType> (uiNode->GetFeature (featureId));
}

}

#endif
