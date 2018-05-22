#ifndef NODEUIMANAGERDRAWER_HPP
#define NODEUIMANAGERDRAWER_HPP

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
	
	void Draw (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const;

private:
	void				DrawBackground (NodeUIDrawingEnvironment& env) const;
	void				DrawGroups (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const;
	void				DrawConnections (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const;
	void				DrawConnection (NodeUIDrawingEnvironment& env, const Point& beg, const Point& end) const;
	void				DrawNodes (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const;
	void				DrawNode (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode) const;
	void				DrawNode (NodeUIDrawingEnvironment& env, const UINode* uiNode) const;
	void				DrawSelectionRect (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier) const;

	void				InitSortedNodeList () const;
	bool				IsConnectionVisible (NodeUIDrawingEnvironment& env, const Point& beg, const Point& end) const;
	bool				IsNodeVisible (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode) const;
	bool				IsRectVisible (NodeUIDrawingEnvironment& env, const Rect& rect) const;

	Rect				GetNodeRect (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode) const;
	Point				GetOutputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const;
	Point				GetInputSlotConnPosition (NodeUIDrawingEnvironment& env, const NodeDrawingModfier* drawModifier, const UINode* uiNode, const NE::SlotId& slotId) const;

	const NodeUIManager&					uiManager;
	NodeIdToNodeMap							nodeIdToNodeMap;
	mutable std::vector<const UINode*>		sortedNodeList;
};

}

#endif
