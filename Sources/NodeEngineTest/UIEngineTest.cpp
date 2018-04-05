#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestNodes.hpp"

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_DrawingContext.hpp"

using namespace NE;
using namespace NUIE;

namespace NodeUIEngineTest
{

class TestInputSlot : public UIInputSlot
{
public:
	TestInputSlot (const SlotId& id, const std::wstring& name) :
		UIInputSlot (id, name, ValuePtr (new IntValue (0)), NE::OutputSlotConnectionMode::Single)
	{

	}
};

class TestOutputSlot : public UIOutputSlot
{
public:
	TestOutputSlot (const SlotId& id, const std::wstring& name) :
		UIOutputSlot (id, name)
	{

	}
};


class TestNode : public SerializableTestUINode
{
public:
	TestNode (const Point& position) :
		SerializableTestUINode (L"Test Node", position)
	{
		
	}

	virtual void RegisterSlots () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new TestInputSlot (SlotId ("in1"), L"First Input")));
		RegisterUIInputSlot (UIInputSlotPtr (new TestInputSlot (SlotId ("in2"), L"Second Input")));
		RegisterUIOutputSlot (UIOutputSlotPtr (new TestOutputSlot (SlotId ("out"), L"Single Output")));
	}

	virtual ValuePtr Calculate (NE::EvaluationEnv&) const override
	{
		return ValuePtr (new IntValue (42));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}
};

TEST (UINodeDataTest)
{
	NodeUIManager uiManager;

	UINodePtr node (new TestNode (Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);

	ASSERT (node->GetNodeName () == L"Test Node");
	node->SetNodeName (L"Test Node 2");
	ASSERT (node->GetNodeName () == L"Test Node 2");

	node->GetUIInputSlot (SlotId ("in1"))->SetName (L"A In");
	node->GetUIInputSlot (SlotId ("in2"))->SetName (L"B In");
	node->GetUIOutputSlot (SlotId ("out"))->SetName (L"A Out");
	ASSERT (node->GetUIInputSlot (SlotId ("in1"))->GetName () == L"A In");
	ASSERT (node->GetUIInputSlot (SlotId ("in2"))->GetName () == L"B In");
	ASSERT (node->GetUIOutputSlot (SlotId ("out"))->GetName () == L"A Out");

	node->GetUIInputSlot (SlotId ("in1"))->SetName (L"A In Modified");
	ASSERT (node->GetUIInputSlot (SlotId ("in1"))->GetName () == L"A In Modified");
}

TEST (UIManagerBaseTest)
{
	NodeUIManager uiManager;

	UINodePtr node1 (new TestNode (Point (0.0, 0.0)));
	UINodePtr node2 (new TestNode (Point (0.0, 0.0)));

	ASSERT (uiManager.AddNode (node1, NE::EmptyEvaluationEnv) != nullptr);
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	ASSERT (node1->GetNodeName () == L"Test Node");
	ASSERT (node1->GetUIInputSlot (SlotId ("in1"))->GetName () == L"First Input");
	ASSERT (node1->GetUIInputSlot (SlotId ("in2"))->GetName () == L"Second Input");
	ASSERT (node1->GetUIOutputSlot (SlotId ("out"))->GetName () == L"Single Output");

	ASSERT (uiManager.ConnectOutputSlotToInputSlot (node1->GetUIOutputSlot (SlotId ("out")), node2->GetUIInputSlot (SlotId ("in1"))));
	ASSERT (uiManager.ConnectOutputSlotToInputSlot (node1->GetUIOutputSlot (SlotId ("out")), node2->GetUIInputSlot (SlotId ("in2"))));
	ASSERT (uiManager.DeleteNode (node1->GetId (), NE::EmptyEvaluationEnv));
}

TEST (ViewBoxTest)
{
	// model : 6 x 4

	{
		ViewBox viewBox (Point (1.0, 2.0), 1.0);
		ASSERT (viewBox.ModelToView (Point (0.0, 0.0)).IsEqualTo (Point (1.0, 2.0)));
		ASSERT (viewBox.ModelToView (Point (1.0, 1.0)).IsEqualTo (Point (2.0, 3.0)));
		ASSERT (viewBox.ViewToModel (Point (1.0, 2.0)).IsEqualTo (Point (0.0, 0.0)));
		ASSERT (viewBox.ViewToModel (Point (2.0, 3.0)).IsEqualTo (Point (1.0, 1.0)));
	}
	{
		ViewBox viewBox (Point (1.0, 2.0), 0.5);
		ASSERT (viewBox.ModelToView (Point (0.0, 0.0)).IsEqualTo (Point (1.0, 2.0)));
		ASSERT (viewBox.ModelToView (Point (2.0, 2.0)).IsEqualTo (Point (2.0, 3.0)));
		ASSERT (viewBox.ModelToView (Point (6.0, 4.0)).IsEqualTo (Point (4.0, 4.0)));
		ASSERT (viewBox.ViewToModel (Point (1.0, 2.0)).IsEqualTo (Point (0.0, 0.0)));
		ASSERT (viewBox.ViewToModel (Point (2.0, 3.0)).IsEqualTo (Point (2.0, 2.0)));
		ASSERT (viewBox.ViewToModel (Point (4.0, 4.0)).IsEqualTo (Point (6.0, 4.0)));
	}
}

TEST (ViewBoxScaleFixPointTest)
{
	std::vector<Point> points = {
		Point (0.0, 0.0),
		Point (2.0, 0.0),
		Point (0.0, 3.0),
		Point (2.0, 3.0)
	};

	for (double scale = 0.1; scale < 1.1; scale += 0.1) {
		for (const Point& mousePoint : points) {
			ViewBox viewBox (Point (1.0, 2.0), 1.0);
			Point origModelPoint = viewBox.ViewToModel (mousePoint);
			viewBox.SetScale (scale, mousePoint);
			ASSERT (viewBox.ViewToModel (mousePoint).IsEqualTo (origModelPoint));
		}
	}
}

}
