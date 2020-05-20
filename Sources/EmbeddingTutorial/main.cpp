#include "NUIE_NodeEditor.hpp"

#pragma warning (disable : 4100)

class MyEventHandlers : public NUIE::EventHandlers
{
public:
	MyEventHandlers () :
		NUIE::EventHandlers ()
	{

	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UINodePtr& uiNode,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UIOutputSlotConstPtr& inputSlot,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UIInputSlotConstPtr& inputSlot,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual NUIE::MenuCommandPtr OnContextMenu (
		const NUIE::Point& position,
		const NUIE::UINodeGroupPtr& group,
		const NUIE::MenuCommandStructure& commands) override
	{
		return nullptr;
	}

	virtual void OnDoubleClick (
		const NUIE::Point& position) override
	{

	}

	virtual bool OnParameterSettings (
		NUIE::ParameterInterfacePtr paramAccessor,
		const NUIE::UINodePtr& uiNode) override
	{
		return false;
	}

	virtual bool OnParameterSettings (
		NUIE::ParameterInterfacePtr paramAccessor,
		const NUIE::UINodeGroupPtr& uiGroup) override
	{
		return false;
	}
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment () :
		NUIE::NodeUIEnvironment (),
		stringConverter (NE::GetDefaultStringConverter ()),
		skinParams (NUIE::GetDefaultSkinParams ()),
		drawingContext (),
		eventHandlers (),
		evaluationEnv (nullptr)
	{

	}

	virtual const NE::StringConverter& GetStringConverter () override
	{
		return stringConverter;
	}

	virtual const NUIE::SkinParams& GetSkinParams () override
	{
		return skinParams;
	}

	virtual NUIE::DrawingContext& GetDrawingContext () override
	{
		return drawingContext;
	}

	virtual double GetWindowScale () override
	{
		return 1.0;
	}

	virtual NE::EvaluationEnv& GetEvaluationEnv () override
	{
		return evaluationEnv;
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

	virtual NUIE::EventHandlers& GetEventHandlers () override
	{
		return eventHandlers;
	}

private:
	NE::BasicStringConverter	stringConverter;
	NUIE::BasicSkinParams		skinParams;
	NUIE::NullDrawingContext	drawingContext;
	MyEventHandlers				eventHandlers;
	NE::EvaluationEnv			evaluationEnv;
};

int main (int argc, char* argv[])
{
	MyNodeUIEnvironment uiEnvironment;
	NUIE::NodeEditor nodeEditor (uiEnvironment);
	nodeEditor.Update ();
	nodeEditor.Draw ();

	return 0;
}
