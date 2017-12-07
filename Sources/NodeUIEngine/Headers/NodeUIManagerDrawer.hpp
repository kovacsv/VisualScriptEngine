#ifndef NODEUIMANAGERDRAWER_HPP
#define NODEUIMANAGERDRAWER_HPP

#include "NodeUIManager.hpp"

namespace NUIE
{

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
	const UINode*		FindNodeById (const NE::NodeId& nodeId) const;

	const NodeUIManager&									uiManager;
	mutable std::vector<const UINode*>						nodesToDraw;
	mutable std::unordered_map<NE::NodeId, const UINode*>	nodeIdToNodeMap;
};

}

#endif
