#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "NUIE_NodeTree.hpp"
#include "MAS_NodeEditorNSViewControl.hpp"

class AppEventHandler : public NUIE::EventHandler
{
public:
	AppEventHandler ();
	virtual ~AppEventHandler ();
	
	void							Init (NUIE::NodeEditor* nodeEditorPtr, void* nsViewPtr);
	
	virtual NUIE::MenuCommandPtr	OnContextMenu (NUIE::EventHandler::ContextMenuType type, const NUIE::Point& position, const NUIE::MenuCommandStructure& commands) override;
	virtual bool					OnParameterSettings (NUIE::EventHandler::ParameterSettingsType type, NUIE::ParameterInterfacePtr paramAccessor) override;
	virtual void					OnDoubleClick (const NUIE::Point& position, NUIE::MouseButton mouseButton) override;

private:
	NUIE::NodeTree					nodeTree;
	NUIE::NodeEditor*				nodeEditor;
	void*							nsView;
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
	virtual void						OnSelectionChanged (const NUIE::Selection& selection) override;
	virtual void 						OnUndoStateChanged (const NUIE::UndoState& undoState) override;
	virtual void						OnClipboardStateChanged (const NUIE::ClipboardState& clipboardState) override;
	virtual void						OnIncompatibleVersionPasted (const NUIE::Version& version) override;
	
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
