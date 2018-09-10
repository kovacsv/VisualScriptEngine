#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "TestNodes.hpp"

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_UINodeParameters.hpp"
#include "NUIE_UINodeCommonParameters.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace EnableDisableFeatureTest
{

class CalculatedCollector : public EvaluationData
{
public:
	std::unordered_map<NodeId, int> values;
};

class TestCalcEnvironment : public NodeUICalculationEnvironment
{
public:
	TestCalcEnvironment () :
		collector (new CalculatedCollector ()),
		evalEnv (collector)
	{

	}

	virtual NE::EvaluationEnv& GetEvaluationEnv ()
	{
		return evalEnv;
	}

	virtual void OnValuesRecalculated ()
	{

	}

	virtual void OnRedrawRequested ()
	{

	}

	std::shared_ptr<CalculatedCollector> collector;
	EvaluationEnv evalEnv;
};

class TestNode :	public SerializableTestUINode,
					public EnableDisableFeature
{
public:
	TestNode (const std::wstring& name, const Point& position) :
		SerializableTestUINode (name, position),
		EnableDisableFeature (true)
	{
		
	}

	virtual void RegisterSlots () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), L"First Input", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), L"Second Input", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), L"Single Output")));
	}

	virtual ValuePtr Calculate (EvaluationEnv& env) const override
	{
		ValuePtr a = EvaluateSingleInputSlot (SlotId ("in1"), env);
		ValuePtr b = EvaluateSingleInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}

	virtual void ProcessValue (const ValuePtr& value, EvaluationEnv& env) const
	{
		EnableDisableFeature::FeatureProcessValue (value, env);
	}

	virtual void OnCalculated (const ValuePtr& value, EvaluationEnv& env) const override
	{
		InsertValue (value, env);
	}

	virtual void OnEnabled (const ValuePtr& value, EvaluationEnv& env) const override
	{
		InsertValue (value, env);
	}

	virtual void OnDisabled (EvaluationEnv& env) const override
	{
		RemoveValue (env);
	}

	void RemoveValue (EvaluationEnv& env) const
	{
		std::shared_ptr<CalculatedCollector> collector = env.GetData<CalculatedCollector> ();
		collector->values.erase (GetId ());
	}

	void InsertValue (const ValuePtr& value, EvaluationEnv& env) const
	{
		std::shared_ptr<CalculatedCollector> collector = env.GetData<CalculatedCollector> ();
		collector->values.insert ({ GetId (), IntValue::Get (value) });
	}
};


TEST (EnableDisableTest)
{
	NodeUIManager uiManager;
	TestCalcEnvironment calcEnv;

	std::shared_ptr<TestNode> node1 (new TestNode (L"TestNode", Point (0, 0)));
	std::shared_ptr<TestNode> node2 (new TestNode (L"TestNode", Point (0, 0)));
	std::shared_ptr<TestNode> node3 (new TestNode (L"TestNode", Point (0, 0)));
	std::shared_ptr<TestNode> node4 (new TestNode (L"TestNode", Point (0, 0)));

	uiManager.AddNode (node1, calcEnv.evalEnv);
	uiManager.AddNode (node2, calcEnv.evalEnv);
	uiManager.AddNode (node3, calcEnv.evalEnv);
	uiManager.AddNode (node4, calcEnv.evalEnv);

	uiManager.ConnectOutputSlotToInputSlot (node1->GetUIOutputSlot (SlotId ("out")), node3->GetUIInputSlot (SlotId ("in1")));
	uiManager.ConnectOutputSlotToInputSlot (node2->GetUIOutputSlot (SlotId ("out")), node3->GetUIInputSlot (SlotId ("in2")));
	uiManager.ConnectOutputSlotToInputSlot (node3->GetUIOutputSlot (SlotId ("out")), node4->GetUIInputSlot (SlotId ("in1")));

	uiManager.Update (calcEnv);
	ASSERT (calcEnv.collector->values.size () == 4);
	ASSERT (calcEnv.collector->values.find (node3->GetId ()) != calcEnv.collector->values.end ());

	node3->SetEnableState (false, node3->GetCalculatedValue (), calcEnv.evalEnv);
	uiManager.RequestRecalculate ();

	uiManager.Update (calcEnv);
	ASSERT (calcEnv.collector->values.size () == 3);
	ASSERT (calcEnv.collector->values.find (node3->GetId ()) == calcEnv.collector->values.end ());
}

}
