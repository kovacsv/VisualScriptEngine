#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "MAS_NodeEditorNSViewControl.hpp"

class AppEventHandler : public NUIE::EventHandler
{
public:
	AppEventHandler ();
	virtual ~AppEventHandler ();
	
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UIOutputSlotConstPtr& outputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UIInputSlotConstPtr& inputSlot, const NUIE::MenuCommandStructure& commands) override;
	virtual NUIE::MenuCommandPtr	OnContextMenu (const NUIE::Point& position, const NUIE::UINodeGroupPtr& group, const NUIE::MenuCommandStructure& commands) override;
	virtual void					OnDoubleClick (NUIE::MouseButton mouseButton, const NUIE::Point& position) override;
	virtual bool					OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodePtr& uiNode) override;
	virtual bool					OnParameterSettings (NUIE::ParameterInterfacePtr paramAccessor, const NUIE::UINodeGroupPtr& uiGroup) override;
};

class AppNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	AppNodeUIEnvironment ();
	virtual ~AppNodeUIEnvironment ();
	
	void								Init (NUIE::NodeEditor* nodeEditorPtr, void* nativeParentHandle, int x, int y, int width, int height);
	void								Resize (int x, int y, int width, int height);
	
	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;
	
	virtual NE::EvaluationEnv& 			GetEvaluationEnv () override;
	virtual void 						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;

	virtual NUIE::EventHandler&			GetEventHandler () override;
	virtual NUIE::ClipboardHandler&		GetClipboardHandler () override;
	virtual double						GetMouseMoveMinOffset () override;
	
private:
	NE::BasicStringConverter			stringConverter;
	NUIE::BasicSkinParams				skinParams;
	NE::EvaluationEnv					evaluationEnv;
	AppEventHandler						eventHandler;
	NUIE::MemoryClipboardHandler		clipboardHandler;
	MAS::NodeEditorNSViewControl		nodeEditorControl;
};

class Application
{
public:
	Application ();

	void Run ();

private:
	AppNodeUIEnvironment	uiEnvironment;
	NUIE::NodeEditor		nodeEditor;
};

#endif
