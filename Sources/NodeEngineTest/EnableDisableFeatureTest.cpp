#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "BI_BasicUINode.hpp"
#include "BI_BuiltInFeatures.hpp"
#include "TestUtils.hpp"
#include "TestNodes.hpp"

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"

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

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evalEnv;
	}

	virtual void OnEvaluationBegin () override
	{

	}

	virtual void OnEvaluationEnd () override
	{

	}
	
	virtual void OnValuesRecalculated () override
	{

	}

	virtual void OnRedrawRequested () override
	{

	}

	std::shared_ptr<CalculatedCollector> collector;
	EvaluationEnv evalEnv;
};

class TestNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (TestNode);

public:
	TestNode () :
		TestNode (LocString (), Point ())
	{
	}

	TestNode (const LocString& name, const Point& position) :
		BasicUINode (name, position)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), LocString (L"First Input"), NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), LocString (L"Second Input"), NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), LocString (L"Single Output"))));
		RegisterFeature (NodeFeaturePtr (new EnableDisableFeature ()));
	}

	virtual ValueConstPtr Calculate (EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}

	virtual void OnFeatureChange (const FeatureId&, EvaluationEnv& env) const override
	{
		std::shared_ptr<EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
		if (enableDisable->GetState () == EnableDisableFeature::State::Enabled) {
			OnEnabled (env);
		} else {
			OnDisabled (env);
		}
	}

	virtual void ProcessCalculatedValue (const ValueConstPtr& value, EvaluationEnv& env) const override
	{
		std::shared_ptr<EnableDisableFeature> enableDisable = GetEnableDisableFeature (this);
		if (enableDisable->GetState () == EnableDisableFeature::State::Enabled) {
			OnCalculated (value, env);
		}
	}

	void OnCalculated (const ValueConstPtr& /*value*/, EvaluationEnv& env) const
	{
		RemoveValue (env);
		InsertValue (env);
	}

	void OnEnabled (EvaluationEnv& env) const
	{
		RemoveValue (env);
		InsertValue (env);
	}

	void OnDisabled (EvaluationEnv& env) const
	{
		RemoveValue (env);
	}

	void RemoveValue (EvaluationEnv& env) const
	{
		std::shared_ptr<CalculatedCollector> collector = env.GetData<CalculatedCollector> ();
		collector->values.erase (GetId ());
	}

	void InsertValue (EvaluationEnv& env) const
	{
		std::shared_ptr<CalculatedCollector> collector = env.GetData<CalculatedCollector> ();
		ValueConstPtr value = GetCalculatedValue ();
		collector->values.insert ({ GetId (), IntValue::Get (value) });
	}
};

DYNAMIC_SERIALIZATION_INFO (TestNode, 1, "{747EB91C-99ED-401A-98DF-A5BBA8FA4CE6}");

TEST (EnableDisableTest)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);
	TestCalcEnvironment calcEnv;

	std::shared_ptr<TestNode> node1 (new TestNode (LocString (L"TestNode"), Point (0, 0)));
	std::shared_ptr<TestNode> node2 (new TestNode (LocString (L"TestNode"), Point (0, 0)));
	std::shared_ptr<TestNode> node3 (new TestNode (LocString (L"TestNode"), Point (0, 0)));
	std::shared_ptr<TestNode> node4 (new TestNode (LocString (L"TestNode"), Point (0, 0)));

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

	std::shared_ptr<EnableDisableFeature> enableDisable = GetEnableDisableFeature (node3);
	enableDisable->SetState (EnableDisableFeature::State::Disabled);
	node3->OnFeatureChange (EnableDisableFeatureId, calcEnv.evalEnv);
	uiManager.RequestRecalculateAndRedraw ();
	
	uiManager.Update (calcEnv);
	ASSERT (calcEnv.collector->values.size () == 3);
	ASSERT (calcEnv.collector->values.find (node3->GetId ()) == calcEnv.collector->values.end ());
}

}
