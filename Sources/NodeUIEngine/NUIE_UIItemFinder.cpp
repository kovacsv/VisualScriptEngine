#include "NUIE_UIItemFinder.hpp"

namespace NUIE
{

static const double SlotSnappingDistanceInPixel = 20.0;

template <class SlotType>
static SlotType FindSlotInNode (const UINodePtr& uiNode, const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	SlotType foundSlot = nullptr;
	const ViewBox& viewBox = uiManager.GetViewBox ();
	uiNode->EnumerateUISlots<SlotType> ([&] (const SlotType& currentSlot) {
		if (uiNode->HasSlotRect<SlotType> (env, currentSlot->GetId ())) {
			Rect slotRect = viewBox.ModelToView (uiNode->GetSlotRect<SlotType> (env, currentSlot->GetId ()));
			if (slotRect.Contains (viewPosition)) {
				foundSlot = currentSlot;
				return false;
			}
		}
		return true;
	});
	return foundSlot;
}

template <class SlotType>
static SlotType FindSlotByConnPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	SlotType foundSlot = nullptr;
	double minDistance = INF;
	const ViewBox& viewBox = uiManager.GetViewBox ();
	uiManager.EnumerateNodes ([&] (const UINodeConstPtr& uiNode) {
		uiNode->EnumerateUISlots<SlotType> ([&] (const SlotType& currentSlot) {
			Point slotConnPosition = viewBox.ModelToView (uiNode->GetSlotConnPosition<SlotType> (env, currentSlot->GetId ()));
			double distance = Point::Distance (viewPosition, slotConnPosition);
			if (distance < minDistance) {
				foundSlot = currentSlot;
				minDistance = distance;
			}
			return true;
		});
		return true;
	});
	if (minDistance > SlotSnappingDistanceInPixel * viewBox.GetScale ()) {
		return nullptr;
	}
	return foundSlot;
}

static UIInputSlotConstPtr FindInputSlotUnderPosition (const UINodePtr& nodeToFindIn, NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	UIInputSlotConstPtr foundInputSlot = nullptr;
	if (nodeToFindIn != nullptr) {
		foundInputSlot = FindSlotInNode<UIInputSlotConstPtr> (nodeToFindIn, uiManager, env, viewPosition);
	} else {
		foundInputSlot = FindSlotByConnPosition<UIInputSlotConstPtr> (uiManager, env, viewPosition);
	}
	return foundInputSlot;
}

static UIOutputSlotConstPtr FindOutputSlotUnderPosition (const UINodePtr& nodeToFindIn, NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	UIOutputSlotConstPtr foundOutputSlot = nullptr;
	if (nodeToFindIn != nullptr) {
		foundOutputSlot = FindSlotInNode<UIOutputSlotConstPtr> (nodeToFindIn, uiManager, env, viewPosition);
	} else {
		foundOutputSlot = FindSlotByConnPosition<UIOutputSlotConstPtr> (uiManager, env, viewPosition);
	}
	return foundOutputSlot;
}

static bool NeedToFindSlots (const NodeUIManager& uiManager)
{
	return !uiManager.IsPreviewMode ();
}

UINodePtr FindNodeUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	UINodePtr foundNode = nullptr;
	uiManager.EnumerateNodes ([&] (const UINodePtr& uiNode) {
		Rect nodeRect = viewBox.ModelToView (uiNode->GetRect (env));
		if (nodeRect.Contains (viewPosition)) {
			if (foundNode == nullptr || foundNode->GetId () < uiNode->GetId ()) {
				foundNode = uiNode;
			}
		}
		return true;
	});
	return foundNode;
}

UINodeGroupPtr FindNodeGroupUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	NodeUIManagerNodeRectGetter rectGetter (uiManager, env);
	UINodeGroupPtr foundGroup = nullptr;
	uiManager.EnumerateNodeGroups ([&] (const UINodeGroupPtr& group) {
		Rect groupRect = viewBox.ModelToView (group->GetRect (env, rectGetter, uiManager.GetGroupNodes (group)));
		if (groupRect.Contains (viewPosition)) {
			foundGroup = group;
		}
		return true;
	});
	return foundGroup;
}

UIInputSlotConstPtr FindInputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	if (!NeedToFindSlots (uiManager)) {
		return nullptr;
	}
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	return FindInputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
}

UIOutputSlotConstPtr FindOutputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	if (!NeedToFindSlots (uiManager)) {
		return nullptr;
	}
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	return FindOutputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
}

bool FindItemUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition,
							const std::function<void (const UIInputSlotConstPtr&)>& inputSlotFound,
							const std::function<void (const UIOutputSlotConstPtr&)>& outputSlotFound,
							const std::function<void (const UINodePtr&)>& nodeFound,
							const std::function<void (const UINodeGroupPtr&)>& nodeGroupFound)
{
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	if (NeedToFindSlots (uiManager)) {
		UIInputSlotConstPtr foundInputSlot = FindInputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
		if (foundInputSlot != nullptr) {
			inputSlotFound (foundInputSlot);
			return true;
		}
		UIOutputSlotConstPtr foundOutputSlot = FindOutputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
		if (foundOutputSlot != nullptr) {
			outputSlotFound (foundOutputSlot);
			return true;
		}
	}

	if (foundNode != nullptr) {
		nodeFound (foundNode);
		return true;
	}

	UINodeGroupPtr foundGroup = FindNodeGroupUnderPosition (uiManager, env, viewPosition);
	if (foundGroup != nullptr) {
		nodeGroupFound (foundGroup);
		return true;
	}

	return false;
}

}
