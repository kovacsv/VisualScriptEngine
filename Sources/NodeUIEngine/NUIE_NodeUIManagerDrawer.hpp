#ifndef NUIE_NODEUIMANAGERDRAWER_HPP
#define NUIE_NODEUIMANAGERDRAWER_HPP

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeDrawingModifier.hpp"

namespace NUIE
{

class SelectionParams
{
public:
	SelectionParams (const NodeUIManager& uiManager, const SkinParams& skinParams);

	double	GetThickness () const;

private:
	double	thickness;
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

	void	DrawBackground (NodeUIDrawingEnvironment& drawingEnv) const;
	void	DrawGroups (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const;
	void	DrawConnections (NodeUIDrawingEnvironment& drawingEnv, const SelectionParams& selectionParams, const NodeDrawingModifier* drawModifier) const;
	void	DrawConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end) const;
	void	DrawTemporaryConnection (NodeUIDrawingEnvironment& drawingEnv, const Pen& pen, const Point& beg, const Point& end, NodeDrawingModifier::Direction dir) const;
	void	DrawNodes (NodeUIDrawingEnvironment& drawingEnv, const SelectionParams& selectionParams, const NodeDrawingModifier* drawModifier) const;
	void	DrawNode (NodeUIDrawingEnvironment& drawingEnv, const SelectionParams& selectionParams, const Point& offset, SelectionMode selectionMode, const UINodeConstPtr& uiNode) const;
	void	DrawNode (NodeUIDrawingEnvironment& drawingEnv, const SelectionParams& selectionParams, SelectionMode selectionMode, const UINodeConstPtr& uiNode) const;
	void	DrawSelectionRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier) const;

	bool	IsConnectionVisible (NodeUIDrawingEnvironment& drawingEnv, const Point& beg, const Point& end) const;
	bool	IsNodeVisible (NodeUIDrawingEnvironment& drawingEnv, const SelectionParams& selectionParams, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode) const;
	bool	IsRectVisible (NodeUIDrawingEnvironment& drawingEnv, const Rect& rect) const;

	Rect	GetNodeRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode) const;
	Rect	GetExtendedNodeRect (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode) const;
	Point	GetOutputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode, const NE::SlotId& slotId) const;
	Point	GetInputSlotConnPosition (NodeUIDrawingEnvironment& drawingEnv, const NodeDrawingModifier* drawModifier, const UINodeConstPtr& uiNode, const NE::SlotId& slotId) const;

	const NodeUIManager&	uiManager;
};

}

#endif
