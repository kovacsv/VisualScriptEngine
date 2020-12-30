#include "NUIE_UIItemFinder.hpp"

namespace NUIE
{

static const double SlotSnappingDistanceInPixel = 20.0;

template <class SlotType>
Point GetSlotConnPosition (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env);

template <class SlotType>
bool HasSlotRect (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env);

template <class SlotType>
Rect GetSlotRect (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env);

template <class SlotType>
static void EnumerateUISlots (const UINodePtr& uiNode, const std::function<bool (SlotType)>& processor);

template <>
Point GetSlotConnPosition<UIInputSlotPtr> (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env)
{
	return uiNode->GetInputSlotConnPosition (env, slotId);
}

template <>
Point GetSlotConnPosition<UIOutputSlotPtr> (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env)
{
	return uiNode->GetOutputSlotConnPosition (env, slotId);
}

template <>
bool HasSlotRect <UIInputSlotPtr> (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env)
{
	return uiNode->HasInputSlotRect (env, slotId);
}

template <>
bool HasSlotRect<UIOutputSlotPtr> (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env)
{
	return uiNode->HasOutputSlotRect (env, slotId);
}

template <>
Rect GetSlotRect<UIInputSlotPtr> (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env)
{
	return uiNode->GetInputSlotRect (env, slotId);
}

template <>
Rect GetSlotRect<UIOutputSlotPtr> (const UINodePtr& uiNode, const NE::SlotId& slotId, NodeUIDrawingEnvironment& env)
{
	return uiNode->GetOutputSlotRect (env, slotId);
}

template <>
void EnumerateUISlots<UIInputSlotPtr> (const UINodePtr& uiNode, const std::function<bool (UIInputSlotPtr)>& processor)
{
	uiNode->EnumerateUIInputSlots (processor);
}

template <>
void EnumerateUISlots<UIOutputSlotPtr> (const UINodePtr& uiNode, const std::function<bool (UIOutputSlotPtr)>& processor)
{
	uiNode->EnumerateUIOutputSlots (processor);
}

template <class SlotType>
static SlotType FindSlotInNode (const UINodePtr& uiNode, const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	SlotType foundSlot = nullptr;
	const ViewBox& viewBox = uiManager.GetViewBox ();
	EnumerateUISlots<SlotType> (uiNode, [&] (SlotType currentSlot) {
		if (HasSlotRect<SlotType> (uiNode, currentSlot->GetId (), env)) {
			Rect slotModelRect = GetSlotRect<SlotType> (uiNode, currentSlot->GetId (), env);
			Rect slotRect = viewBox.ModelToView (slotModelRect);
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
	uiManager.EnumerateNodes ([&] (UINodePtr uiNode) {
		EnumerateUISlots<SlotType> (uiNode, [&] (SlotType currentSlot) {
			Point slotModelConnPosition = GetSlotConnPosition<SlotType> (uiNode, currentSlot->GetId (), env);
			Point slotConnPosition = viewBox.ModelToView (slotModelConnPosition);
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

static UIInputSlotPtr FindInputSlotUnderPosition (const UINodePtr& nodeToFindIn, NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	UIInputSlotPtr foundInputSlot = nullptr;
	if (nodeToFindIn != nullptr) {
		foundInputSlot = FindSlotInNode<UIInputSlotPtr> (nodeToFindIn, uiManager, env, viewPosition);
	} else {
		foundInputSlot = FindSlotByConnPosition<UIInputSlotPtr> (uiManager, env, viewPosition);
	}
	return foundInputSlot;
}

static UIOutputSlotPtr FindOutputSlotUnderPosition (const UINodePtr& nodeToFindIn, NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	UIOutputSlotPtr foundOutputSlot = nullptr;
	if (nodeToFindIn != nullptr) {
		foundOutputSlot = FindSlotInNode<UIOutputSlotPtr> (nodeToFindIn, uiManager, env, viewPosition);
	} else {
		foundOutputSlot = FindSlotByConnPosition<UIOutputSlotPtr> (uiManager, env, viewPosition);
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
	uiManager.EnumerateNodes ([&] (UINodePtr uiNode) {
		Rect nodeRect = viewBox.ModelToView (uiNode->GetRect (env));
		if (nodeRect.Contains (viewPosition)) {
			foundNode = uiNode;
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
	uiManager.EnumerateNodeGroups ([&] (UINodeGroupPtr group) {
		Rect groupRect = viewBox.ModelToView (group->GetRect (env, rectGetter, uiManager.GetGroupNodes (group)));
		if (groupRect.Contains (viewPosition)) {
			foundGroup = group;
		}
		return true;
	});
	return foundGroup;
}

UIInputSlotPtr FindInputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	if (!NeedToFindSlots (uiManager)) {
		return nullptr;
	}
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	return FindInputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
}

UIOutputSlotPtr FindOutputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	if (!NeedToFindSlots (uiManager)) {
		return nullptr;
	}
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	return FindOutputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
}

bool FindItemUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition,
							const std::function<void (const UIInputSlotPtr&)>& inputSlotFound,
							const std::function<void (const UIOutputSlotPtr&)>& outputSlotFound,
							const std::function<void (const UINodePtr&)>& nodeFound,
							const std::function<void (const UINodeGroupPtr&)>& nodeGroupFound)
{
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	if (NeedToFindSlots (uiManager)) {
		UIInputSlotPtr foundInputSlot = FindInputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
		if (foundInputSlot != nullptr) {
			inputSlotFound (foundInputSlot);
			return true;
		}
		UIOutputSlotPtr foundOutputSlot = FindOutputSlotUnderPosition (foundNode, uiManager, env, viewPosition);
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
