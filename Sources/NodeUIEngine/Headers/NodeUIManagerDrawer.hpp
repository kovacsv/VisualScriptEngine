#ifndef NODEUIMANAGERDRAWER_HPP
#define NODEUIMANAGERDRAWER_HPP

#include "NodeUIManager.hpp"

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
	
	void Draw (NodeUIEnvironment& env, const NodeDrawingExtension* drawExt) const;

private:
	void				DrawBackground (NodeUIEnvironment& env) const;
	void				DrawConnections (NodeUIEnvironment& env, const NodeDrawingExtension* drawExt) const;
	void				DrawConnection (NodeUIEnvironment& env, const Point& beg, const Point& end) const;
	void				DrawNodes (NodeUIEnvironment& env) const;
	void				DrawSelectionRect (NodeUIEnvironment& env, const NodeDrawingExtension* drawExt) const;

	void				InitNodesToDraw (NodeUIEnvironment& env) const;
	bool				IsConnectionVisible (NodeUIEnvironment& env, const Point& beg, const Point& end) const;
	bool				IsNodeVisible (NodeUIEnvironment& env, const UINode* uiNode) const;
	bool				IsRectVisible (NodeUIEnvironment& env, const Rect& rect) const;

	const NodeUIManager&					uiManager;
	NodeIdToNodeMap							nodeIdToNodeMap;
	mutable std::vector<const UINode*>		visibleNodes;
	mutable std::vector<const UINode*>		visibleConnectedNodes;
};

}

#endif
