#include "SimpleTest.hpp"
#include "NUIE_NodeEditor.hpp"
#include "BI_InputUINodes.hpp"
#include "BI_ViewerUINodes.hpp"
#include "TestEnvironment.hpp"

using namespace NE;
using namespace NUIE;
using namespace BI;

namespace NodeEditorVisualTestParameterSettings
{

class AllParametersTestNode : public BasicUINode
{
	DYNAMIC_SERIALIZABLE (AllParametersTestNode);

public:
	enum class MyEnum
	{
		EnumVal1 = 0,
		EnumVal2 = 1,
		EnumVal3 = 2
	};

	AllParametersTestNode () :
		AllParametersTestNode (LocString (), Point ())
	{

	}

	AllParametersTestNode (const LocString& name, const Point& position) :
		BasicUINode (name, position),
		boolVal (true),
		intVal (1),
		floatVal (2.0f),
		doubleVal (3.0),
		stringVal (L"str"),
		enumVal (MyEnum::EnumVal1)
	{

	}

	virtual ~AllParametersTestNode ()
	{

	}

	virtual void Initialize () override
	{
		RegisterUIOutputSlot (std::make_shared<UIOutputSlot> (SlotId ("out"), LocString (L"Value")));
	}

	virtual ValueConstPtr Calculate (EvaluationEnv& ) const override
	{
		ListValuePtr val (new ListValue ());
		val->Push (std::make_shared<BooleanValue> (boolVal));
		val->Push (std::make_shared<IntValue> (intVal));
		val->Push (std::make_shared<FloatValue> (floatVal));
		val->Push (std::make_shared<DoubleValue> (doubleVal));
		val->Push (std::make_shared<StringValue> (stringVal));
		val->Push (std::make_shared<IntValue> ((int) enumVal));
		return val;
	}

	virtual void RegisterParameters (NodeParameterList& parameterList) const override
	{
		class BoolParameter : public BooleanNodeParameter<AllParametersTestNode>
		{
		public:
			BoolParameter () :
				BooleanNodeParameter<AllParametersTestNode> (LocString (L"Boolean"))
			{

			}

			virtual ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return std::make_shared<BooleanValue> (GetTypedNode (uiNode)->GetBoolVal ());
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, EvaluationEnv&, UINodePtr& uiNode, const ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetBoolVal (BooleanValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		class IntParameter : public IntegerNodeParameter<AllParametersTestNode>
		{
		public:
			IntParameter () :
				IntegerNodeParameter<AllParametersTestNode> (LocString (L"Integer"))
			{

			}

			virtual ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return std::make_shared<IntValue> (GetTypedNode (uiNode)->GetIntVal ());
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, EvaluationEnv&, UINodePtr& uiNode, const ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetIntVal (IntValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		class FloatParameter : public FloatNodeParameter<AllParametersTestNode>
		{
		public:
			FloatParameter () :
				FloatNodeParameter<AllParametersTestNode> (LocString (L"Float"))
			{

			}

			virtual ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return std::make_shared<FloatValue> (GetTypedNode (uiNode)->GetFloatVal ());
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, EvaluationEnv&, UINodePtr& uiNode, const ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetFloatVal (FloatValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		class DoubleParameter : public DoubleNodeParameter<AllParametersTestNode>
		{
		public:
			DoubleParameter () :
				DoubleNodeParameter<AllParametersTestNode> (LocString (L"Double"))
			{

			}

			virtual ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return std::make_shared<DoubleValue> (GetTypedNode (uiNode)->GetDoubleVal ());
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, EvaluationEnv&, UINodePtr& uiNode, const ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetDoubleVal (DoubleValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		class StringParameter : public StringNodeParameter<AllParametersTestNode>
		{
		public:
			StringParameter () :
				StringNodeParameter<AllParametersTestNode> (LocString (L"String"))
			{

			}

			virtual ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return std::make_shared<StringValue> (GetTypedNode (uiNode)->GetStringVal ());
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, EvaluationEnv&, UINodePtr& uiNode, const ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetStringVal (StringValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		class EnumParameter : public EnumerationNodeParameter<AllParametersTestNode>
		{
		public:
			EnumParameter () :
				EnumerationNodeParameter<AllParametersTestNode> (LocString (L"Enumeration"), { LocString (L"Enum Value 1"), LocString (L"Enum Value 2"), LocString (L"Enum Value 3") })
			{

			}

			virtual ValueConstPtr GetValueInternal (const UINodeConstPtr& uiNode) const override
			{
				return std::make_shared<IntValue> ((int) GetTypedNode (uiNode)->GetEnumVal ());
			}

			virtual bool SetValueInternal (UINodeInvalidator& invalidator, EvaluationEnv&, UINodePtr& uiNode, const ValueConstPtr& value) override
			{
				GetTypedNode (uiNode)->SetEnumVal ((MyEnum) IntValue::Get (value));
				invalidator.InvalidateValueAndDrawing ();
				return true;
			}
		};

		BasicUINode::RegisterParameters (parameterList);
		parameterList.AddParameter (std::make_shared<BoolParameter> ());
		parameterList.AddParameter (std::make_shared<IntParameter> ());
		parameterList.AddParameter (std::make_shared<FloatParameter> ());
		parameterList.AddParameter (std::make_shared<DoubleParameter> ());
		parameterList.AddParameter (std::make_shared<StringParameter> ());
		parameterList.AddParameter (std::make_shared<EnumParameter> ());
	}

	virtual Stream::Status Read (InputStream& inputStream) override
	{
		ObjectHeader header (inputStream);
		BasicUINode::Read (inputStream);
		inputStream.Read (boolVal);
		inputStream.Read (intVal);
		inputStream.Read (floatVal);
		inputStream.Read (doubleVal);
		inputStream.Read (stringVal);
		ReadEnum<MyEnum> (inputStream, enumVal);
		return inputStream.GetStatus ();
	}

	virtual Stream::Status Write (OutputStream& outputStream) const override
	{
		ObjectHeader header (outputStream, serializationInfo);
		BasicUINode::Write (outputStream);
		outputStream.Write (boolVal);
		outputStream.Write (intVal);
		outputStream.Write (floatVal);
		outputStream.Write (doubleVal);
		outputStream.Write (stringVal);
		WriteEnum<MyEnum> (outputStream, enumVal);
		return outputStream.GetStatus ();
	}

	bool GetBoolVal () const
	{
		return boolVal;
	}

	void SetBoolVal (bool newBoolVal)
	{
		boolVal = newBoolVal;
	}

	int GetIntVal () const
	{
		return intVal;
	}

	void SetIntVal (int newIntVal)
	{
		intVal = newIntVal;
	}

	float GetFloatVal () const
	{
		return floatVal;
	}

	void SetFloatVal (float newFloatVal)
	{
		floatVal = newFloatVal;
	}
	
	double GetDoubleVal () const
	{
		return doubleVal;
	}

	void SetDoubleVal (double newDoubleVal)
	{
		doubleVal = newDoubleVal;
	}

	const std::wstring& GetStringVal () const
	{
		return stringVal;
	}

	void SetStringVal (const std::wstring& newStringVal)
	{
		stringVal = newStringVal;
	}

	MyEnum GetEnumVal () const
	{
		return enumVal;
	}

	void SetEnumVal (MyEnum newEnumVal)
	{
		enumVal = newEnumVal;
	}

private:
	bool			boolVal;
	int				intVal;
	float			floatVal;
	double			doubleVal;
	std::wstring	stringVal;
	MyEnum			enumVal;
};

DYNAMIC_SERIALIZATION_INFO (AllParametersTestNode, 1, "{75CBE853-BC71-41E5-B46D-1AD44D1A8AC6}");

class ParameterSettingsTestEnv : public NodeEditorTestEnv
{
public:
	ParameterSettingsTestEnv (const BasicSkinParams& skinParams) :
		NodeEditorTestEnv (skinParams)
	{
		UINodePtr allParamsNode1 (new AllParametersTestNode (LocString (L"Params 1"), Point (100, 200)));
		UINodePtr allParamsNode2 (new AllParametersTestNode (LocString (L"Params 2"), Point (100, 300)));
		UINodePtr viewerNode1 (new MultiLineViewerNode (LocString (L"Viewer 1"), Point (600, 200), 10));
		UINodePtr viewerNode2 (new MultiLineViewerNode (LocString (L"Viewer 2"), Point (350, 400), 10));

		nodeEditor.AddNode (allParamsNode1);
		nodeEditor.AddNode (allParamsNode2);
		nodeEditor.AddNode (viewerNode1);
		nodeEditor.AddNode (viewerNode2);

		nodeEditor.ConnectOutputSlotToInputSlot (allParamsNode1->GetUIOutputSlot (SlotId ("out")), viewerNode1->GetUIInputSlot (SlotId ("in")));
		nodeEditor.ConnectOutputSlotToInputSlot (allParamsNode2->GetUIOutputSlot (SlotId ("out")), viewerNode2->GetUIInputSlot (SlotId ("in")));

		nodeEditor.Update ();
	}

	void SetParameter (ParameterInterfacePtr paramInterface, std::wstring name, ValueConstPtr val)
	{
		for (size_t i = 0; i < paramInterface->GetParameterCount (); i++) {
			if (paramInterface->GetParameterName (i) == name) {
				paramInterface->SetParameterValue (i, val);
				break;
			}
		}
	}
};

static const Point Padding = Point (10.0, 10.0);

TEST (ParameterSettingsTest_ModifyParameter_Single)
{
	ParameterSettingsTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"ParameterSettingsTest_Single_Initial.svg"));

	{ // set Params 1 node parameters
		env.SetNextCommandNodeParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Node Name", std::make_shared<StringValue> (L"Renamed 1"));
			env.SetParameter (paramInterface, L"Boolean", std::make_shared<BooleanValue> (false));
			env.SetParameter (paramInterface, L"Integer", std::make_shared<IntValue> (5));
			env.SetParameter (paramInterface, L"Float", std::make_shared<FloatValue> (6.0f));
			env.SetParameter (paramInterface, L"Double", std::make_shared<DoubleValue> (7.0));
			env.SetParameter (paramInterface, L"String", std::make_shared<StringValue> (L"str2"));
			env.SetParameter (paramInterface, L"Enumeration", std::make_shared<IntValue> ((int) AllParametersTestNode::MyEnum::EnumVal2));
			return true;
		});
		env.RightClick (env.GetNodeRect (env.GetNode (L"Params 1")).GetTopLeft () + Padding);
		ASSERT (env.CheckReference (L"ParameterSettingsTest_Single_ParamsChanged.svg"));
	}
}

TEST (ParameterSettingsTest_ModifyParameter_Multiple)
{
	ParameterSettingsTestEnv env (GetDefaultSkinParams ());
	ASSERT (env.CheckReference (L"ParameterSettingsTest_Multiple_Initial.svg"));

	{ // set Params 1 node parameters
		env.SetNextCommandNodeParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Integer", std::make_shared<IntValue> (5));
			return true;
		});
		env.RightClick (env.GetNodeRect (env.GetNode (L"Params 1")).GetTopLeft () + Padding);
		ASSERT (env.CheckReference (L"ParameterSettingsTest_Multiple_Params1Changed.svg"));
	}

	{ // select and set Params 1 and Params 2 nodes parameters
		env.SetNextCommandNodeParameterSettings ([&] (ParameterInterfacePtr paramInterface) {
			env.SetParameter (paramInterface, L"Float", std::make_shared<FloatValue> (6.0f));
			return true;
		});
		env.DragDrop (
			env.GetNodeRect (env.GetNode (L"Params 1")).GetTopLeft () - Padding,
			env.GetNodeRect (env.GetNode (L"Params 2")).GetBottomRight () + Padding
		);
		env.RightClick (env.GetNodeRect (env.GetNode (L"Params 1")).GetTopLeft () + Padding);
		env.ExecuteCommand (CommandCode::Escape);
		ASSERT (env.CheckReference (L"ParameterSettingsTest_Multiple_Params2Changed.svg"));
	}
}

}
