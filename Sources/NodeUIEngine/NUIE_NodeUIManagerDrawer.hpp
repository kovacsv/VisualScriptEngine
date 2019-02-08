#ifndef NUIE_NODEUIMANAGERDRAWER_HPP
#define NUIE_NODEUIMANAGERDRAWER_HPP

#include "NUIE_NodeUIManager.hpp"

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

class NodeUIManagerDrawer
{
public:
	NodeUIManagerDrawer (const NodeUIManager& uiManager);
	
	void Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier) const;

private:
	void				DrawBackground (NodeUIDrawingEnvironment& env) const;
	void				DrawGroups (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier) const;
	void				DrawConnections (NodeUIDrawingEnvironment& env, double selectionThickness, const NodeDrawingModifier* drawModifier) const;
	void				DrawConnection (NodeUIDrawingEnvironment& env, const Pen& pen, const Point& beg, const Point& end) const;
	void				DrawNodes (NodeUIDrawingEnvironment& env, double selectionThickness, const NodeDrawingModifier* drawModifier) const;
	void				DrawNode (NodeUIDrawingEnvironment& env, const UINode* uiNode, double selectionThickness) const;
	void				DrawSelectionRect (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier) const;

	void				InitSortedNodeList () const;
	bool				IsConnectionVisible (NodeUIDrawingEnvironment& env, const Point& beg, const Point& end) const;
	bool				IsRectVisible (NodeUIDrawingEnvironment& env, const Rect& rect) const;

	Rect				GetNodeRect (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier, const UINode* uiNode) const;
	Point				GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const;
	Point				GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModifier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const;
	double				GetNodeSelectionThickness (NodeUIDrawingEnvironment& env) const;

	const NodeUIManager&					uiManager;
	NodeIdToNodeMap							nodeIdToNodeMap;
	mutable std::vector<const UINode*>		sortedNodeList;
};

}

#endif
