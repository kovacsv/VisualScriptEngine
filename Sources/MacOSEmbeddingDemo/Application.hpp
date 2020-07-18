#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "MAS_NodeEditorNSViewControl.hpp"

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
	NUIE::NullEventHandler				eventHandler;
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
};

#endif
