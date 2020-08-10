#include "NUIE_UIItemFinder.hpp"

namespace NUIE
{

static const double SlotSnappingDistanceInPixel = 20.0;

template <class SlotType>
static SlotType FindSlotByConnPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	SlotType foundSlot = nullptr;
	double minDistance = INF;
	const ViewBox& viewBox = uiManager.GetViewBox ();
	uiManager.EnumerateUINodes ([&] (const UINodeConstPtr& uiNode) {
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

UINodePtr FindNodeUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	const ViewBox& viewBox = uiManager.GetViewBox ();
	UINodePtr foundNode = nullptr;
	uiManager.EnumerateUINodes ([&] (const UINodePtr& uiNode) {
		Rect nodeRect = viewBox.ModelToView (uiNode->GetNodeRect (env));
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
	class NodeUIManagerNodeRectGetter : public NodeRectGetter
	{
	public:
		NodeUIManagerNodeRectGetter (const NodeUIManager& uiManager, NodeUIDrawingEnvironment& env) :
			uiManager (uiManager),
			env (env)
		{
	
		}

		virtual Rect GetNodeRect (const NE::NodeId& nodeId) const override
		{
			UINodeConstPtr uiNode = uiManager.GetUINode (nodeId);
			return uiNode->GetNodeRect (env);
		}

	private:
		const NodeUIManager& uiManager;
		NodeUIDrawingEnvironment& env;
	};

	const ViewBox& viewBox = uiManager.GetViewBox ();
	NodeUIManagerNodeRectGetter rectGetter (uiManager, env);
	UINodeGroupPtr foundGroup = nullptr;
	uiManager.EnumerateUINodeGroups ([&] (const UINodeGroupPtr& group) {
		Rect groupRect = viewBox.ModelToView (group->GetRect (env, rectGetter, uiManager.GetUIGroupNodes (group)));
		if (groupRect.Contains (viewPosition)) {
			foundGroup = group;
		}
		return true;
	});
	return foundGroup;
}

UIInputSlotConstPtr FindInputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	UIInputSlotConstPtr foundSlot = nullptr;
	FindItemUnderPosition (uiManager, env, viewPosition, nullptr, nullptr, [&] (const UIInputSlotConstPtr& inputSlot) {
		foundSlot = inputSlot;
	}, nullptr);
	return foundSlot;
}

UIOutputSlotConstPtr FindOutputSlotUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition)
{
	UIOutputSlotConstPtr foundSlot = nullptr;
	FindItemUnderPosition (uiManager, env, viewPosition, nullptr, [&] (const UIOutputSlotConstPtr& outputSlot) {
		foundSlot = outputSlot;
	}, nullptr, nullptr);
	return foundSlot;
}

bool FindItemUnderPosition (NodeUIManager& uiManager, NodeUIDrawingEnvironment& env, const Point& viewPosition,
							const std::function<void (const UINodePtr&)>& nodeFound,
							const std::function<void (const UIOutputSlotConstPtr&)>& outputSlotFound,
							const std::function<void (const UIInputSlotConstPtr&)>& inputSlotFound,
							const std::function<void (const UINodeGroupPtr&)>& nodeGroupFound)
{
	UINodePtr foundNode = FindNodeUnderPosition (uiManager, env, viewPosition);
	UIOutputSlotConstPtr foundOutputSlot = nullptr;
	UIInputSlotConstPtr foundInputSlot = nullptr;

	bool findItemsInsideNode = !uiManager.IsPreviewMode ();
	if (findItemsInsideNode) {
		if (foundNode != nullptr) {
			foundOutputSlot = FindSlotInNode<UIOutputSlotConstPtr> (foundNode, uiManager, env, viewPosition);
			if (foundOutputSlot != nullptr) {
				foundNode = nullptr;
			}
		}
		if (foundNode != nullptr) {
			foundInputSlot = FindSlotInNode<UIInputSlotConstPtr> (foundNode, uiManager, env, viewPosition);
			if (foundInputSlot != nullptr) {
				foundNode = nullptr;
			}
		}
	}

	if (foundNode != nullptr && nodeFound != nullptr) {
		nodeFound (foundNode);
		return true;
	}

	if (findItemsInsideNode) {
		if (foundOutputSlot == nullptr) {
			foundOutputSlot = FindSlotByConnPosition<UIOutputSlotConstPtr> (uiManager, env, viewPosition);
		}

		if (foundInputSlot == nullptr) {
			foundInputSlot = FindSlotByConnPosition<UIInputSlotConstPtr> (uiManager, env, viewPosition);
		}

		if (foundOutputSlot != nullptr && outputSlotFound != nullptr) {
			outputSlotFound (foundOutputSlot);
			return true;
		}
	
		if (foundInputSlot != nullptr && inputSlotFound != nullptr) {
			inputSlotFound (foundInputSlot);
			return true;
		}
	}

	if (nodeGroupFound != nullptr) {
		UINodeGroupPtr foundGroup = FindNodeGroupUnderPosition (uiManager, env, viewPosition);
		if (foundGroup != nullptr) {
			nodeGroupFound (foundGroup);
			return true;
		}
	}

	return false;
}

}
