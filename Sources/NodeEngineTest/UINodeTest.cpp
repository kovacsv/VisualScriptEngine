#include "SimpleTest.hpp"
#include "NE_NodeManager.hpp"
#include "NE_Node.hpp"
#include "NE_InputSlot.hpp"
#include "NE_OutputSlot.hpp"
#include "NE_SingleValues.hpp"
#include "TestUtils.hpp"
#include "TestNodes.hpp"

#include "NUIE_NodeUIManager.hpp"
#include "NUIE_NodeParameters.hpp"
#include "NUIE_NodeCommonParameters.hpp"

using namespace NE;
using namespace NUIE;

namespace UINodeTest
{

class TestNode : public SerializableTestUINode
{
public:
	TestNode (const std::wstring& name, const Point& position) :
		SerializableTestUINode (name, position)
	{
		
	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), L"First Input", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), L"Second Input", NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), L"Single Output")));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class In1DefaultValueParameter : public SlotDefaultValueNodeParameter<TestNode, NE::IntValue>
		{
		public:
			In1DefaultValueParameter () :
				SlotDefaultValueNodeParameter<TestNode, NE::IntValue> (L"In1", ParameterType::Integer, SlotId ("in1"))
			{
			
			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new In1DefaultValueParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{
	
	}
};

class TestNode2 : public SerializableTestUINode
{
public:
	TestNode2 (const std::wstring& name, const Point& position) :
		SerializableTestUINode (name, position)
	{

	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), L"First Input", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), L"Second Input", NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), L"Single Output")));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class In2DefaultValueParameter : public SlotDefaultValueNodeParameter<TestNode2, NE::IntValue>
		{
		public:
			In2DefaultValueParameter () :
				SlotDefaultValueNodeParameter<TestNode2, NE::IntValue> (L"In2", ParameterType::Integer, SlotId ("in1"))
			{

			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new In2DefaultValueParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{

	}
};

class EnumerationParamTestNode : public SerializableTestUINode
{
public:
	enum class MyEnumValue
	{
		AEnum,
		BEnum,
		CEnum
	};

	EnumerationParamTestNode (const std::wstring& name, const Point& position) :
		SerializableTestUINode (name, position),
		myEnumValue (MyEnumValue::AEnum)
	{

	}

	virtual void Initialize () override
	{
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in1"), L"First Input", NE::ValuePtr (new NE::IntValue (1)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (SlotId ("in2"), L"Second Input", NE::ValuePtr (new NE::IntValue (2)), NE::OutputSlotConnectionMode::Single)));
		RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (SlotId ("out"), L"Single Output")));
	}

	virtual ValueConstPtr Calculate (NE::EvaluationEnv& env) const override
	{
		ValueConstPtr a = EvaluateInputSlot (SlotId ("in1"), env);
		ValueConstPtr b = EvaluateInputSlot (SlotId ("in2"), env);
		int result = IntValue::Get (a) + IntValue::Get (b);
		return ValuePtr (new IntValue (result));
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class MyEnumerationParameter : public EnumerationNodeParameter<EnumerationParamTestNode>
		{
		public:
			MyEnumerationParameter () :
				EnumerationNodeParameter<EnumerationParamTestNode> (L"EnumParam", { L"AString", L"BString", L"CString" })
			{

			}

			virtual NE::ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return NE::ValuePtr (new NE::IntValue ((int) GetTypedNode (uiNode)->GetMyEnumValue ()));
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetMyEnumValue ((MyEnumValue) NE::IntValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		UINode::RegisterParameters (parameterList);
		parameterList.AddParameter (NodeParameterPtr (new MyEnumerationParameter ()));
	}

	virtual void UpdateNodeDrawingImage (NodeUIDrawingEnvironment&, NodeDrawingImage&) const override
	{

	}

	void SetMyEnumValue (MyEnumValue newValue)
	{
		myEnumValue = newValue;
	}

	MyEnumValue GetMyEnumValue () const
	{
		return myEnumValue;
	}

private:
	MyEnumValue myEnumValue;
};

TEST (NodeParametersTest)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);
	UINodePtr node (new TestNode (L"TestNode", Point (0, 0)));

	NodeUIManagerNodeInvalidator invalidator (uiManager, node);

	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);

	NodeParameterList paramList;
	node->RegisterParameters (paramList);
	ValueConstPtr nodeValue = node->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (IntValue::Get (nodeValue) == 3);

	ASSERT (paramList.GetParameterCount () == 2);
	
	ASSERT (node->GetNodeName () == L"TestNode");
	NodeParameterPtr nameParam = paramList.GetParameter (0);
	ASSERT (nameParam->GetName () == L"Node Name");
	ValuePtr newNameValue (new StringValue (L"NewNodeName"));
	nameParam->SetValue (invalidator, NE::EmptyEvaluationEnv, node, newNameValue);
	ASSERT (node->GetNodeName () == L"NewNodeName");

	NodeParameterPtr in1DefParam = paramList.GetParameter (1);
	ASSERT (in1DefParam->GetName () == L"In1");
	ASSERT (IntValue::Get (in1DefParam->GetValue (node)) == 1);
	ValuePtr newIn1Value (new IntValue (5));
	in1DefParam->SetValue (invalidator, NE::EmptyEvaluationEnv, node, newIn1Value);
	ASSERT (IntValue::Get (in1DefParam->GetValue (node)) == 5);
	nodeValue = node->Evaluate (NE::EmptyEvaluationEnv);
	ASSERT (IntValue::Get (nodeValue) == 7);
}

TEST (NodeParametersTest2)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr node (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);
	UINodePtr node2 (new TestNode2 (L"TestNode2", Point (0, 0)));
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection (), paramList);
		ASSERT (paramList.GetParameterCount () == 0);
	}

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection ({ node->GetId () }), paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName () == L"In1");
	}

	{
		NodeParameterList paramList;
		RegisterCommonParameters (uiManager, NodeCollection ({ node2->GetId () }), paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName () == L"In2");
	}

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection;
		nodeCollection.Insert (node->GetId ());
		nodeCollection.Insert (node2->GetId ());
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 1);
		ASSERT (paramList.GetParameter (0)->GetName () == L"Node Name");
	}
}

TEST (NodeParametersTest3)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	UINodePtr node (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);
	UINodePtr node2 (new TestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node2, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection;
		nodeCollection.Insert (node->GetId ());
		nodeCollection.Insert (node2->GetId ());
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName () == L"In1");
		ValuePtr newName (new StringValue (L"NewName"));
		ApplyCommonParameter (uiManager, EmptyEvaluationEnv, nodeCollection, paramList.GetParameter (0), newName);
		ASSERT (node->GetNodeName () == L"NewName");
		ASSERT (node2->GetNodeName () == L"NewName");
	}
}

TEST (NodeParametersTest4)
{
	TestDrawingEnvironment env;
	NodeUIManager uiManager (env);

	std::shared_ptr<EnumerationParamTestNode> node (new EnumerationParamTestNode (L"TestNode", Point (0, 0)));
	ASSERT (uiManager.AddNode (node, NE::EmptyEvaluationEnv) != nullptr);

	{
		NodeParameterList paramList;
		NodeCollection nodeCollection ({ node->GetId () });
		RegisterCommonParameters (uiManager, nodeCollection, paramList);
		ASSERT (paramList.GetParameterCount () == 2);
		ASSERT (paramList.GetParameter (0)->GetName () == L"Node Name");
		ASSERT (paramList.GetParameter (1)->GetName () == L"EnumParam");
		std::vector<std::wstring> choices = paramList.GetParameter (1)->GetValueChoices ();
		ASSERT (choices.size () == 3);
		ASSERT (choices[0] == L"AString");
		ASSERT (choices[1] == L"BString");
		ASSERT (choices[2] == L"CString");

		ASSERT (node->GetMyEnumValue () == EnumerationParamTestNode::MyEnumValue::AEnum);

		ValuePtr newValue (new IntValue (1));
		ApplyCommonParameter (uiManager, EmptyEvaluationEnv, nodeCollection, paramList.GetParameter (1), newValue);
		ASSERT (node->GetMyEnumValue () == EnumerationParamTestNode::MyEnumValue::BEnum);
	}
}

}
