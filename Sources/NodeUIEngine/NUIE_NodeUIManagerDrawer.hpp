#ifndef NUIE_NODEUIMANAGERDRAWER_HPP
#define NUIE_NODEUIMANAGERDRAWER_HPP

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeDrawingModifier.hpp"

namespace NUIE
{

class NodeIdToNodeMap
{
public:
	NodeIdToNodeMap (const NodeUIManager& uiManager);

	void			Insert (const NE::NodeId& nodeId, const UINode* uiNode);
	const UINode*	GetUINode (const NE::NodeId& nodeId) const;
	void			Enumerate (const std::function<void (const UINode*)>& processor) const;

private:
	std::unordered_map<NE::NodeId, const UINode*>	nodeIdToNodeMap;
};

class NodeUIScaleIndependentData
{
public:
	NodeUIScaleIndependentData (const NodeUIManager& uiManager, const SkinParams& skinParams);

	double	GetSelectionThickness () const;

private:
	double selectionThickness;
};

class NodeUIManagerDrawer
{
public:
	NodeUIManagerDrawer (const NodeUIManager& uiManager);
	
	void Draw (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const;

private:
	enum class SelectionMode
	{
		Selected,
		NotSelected
	};

	void				DrawBackground (NodeUIDrawingEnvironment& drawingEnv) const;
	void				DrawGroups (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const;
	void				DrawConnections (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const;
	void				DrawConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end) const;
	void				DrawTemporaryConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end, NodeDrawingModifier::Direction dir) const;
	void				DrawNodes (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier) const;
	void				DrawNode (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const Point& offset, SelectionMode selectionMode, const UINode* uiNode) const;
	void				DrawNode (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, SelectionMode selectionMode, const UINode* uiNode) const;
	void				DrawSelectionRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const;

	void				InitSortedNodeList () const;
	bool				IsConnectionVisible (NodeUIDrawingEnvironment& drawingEnv, const Point& beg, const Point& end) const;
	bool				IsNodeVisible (NodeUIDrawingEnvironment& drawingEnv, const NodeUIScaleIndependentData& scaleIndependentData, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const;
	bool				IsRectVisible (NodeUIDrawingEnvironment& drawingEnv, const Rect& rect) const;

	Rect				GetNodeRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const;
	Point				GetOutputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const;
	Point				GetInputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const;

	const NodeUIManager&				uiManager;
	NodeIdToNodeMap						nodeIdToNodeMap;
	mutable std::vector<const UINode*>	sortedNodeList;
};

Rect ExtendNodeRect (NodeUIDrawingEnvironment& drawingEnv, const Rect& originalRect);
Rect GetNodeExtendedRect (NodeUIDrawingEnvironment& drawingEnv, const UINode* uiNode);

}

#endif
