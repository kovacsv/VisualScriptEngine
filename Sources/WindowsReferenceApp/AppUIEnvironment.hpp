#ifndef APPUIENVIRONMENT_HPP
#define APPUIENVIRONMENT_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeEditor.hpp"
#include "WAS_NodeEditorNodeTreeHwndControl.hpp"
#include "WAS_HwndEventHandler.hpp"
#include "WAS_ClipboardHandler.hpp"
#include "WAS_FileMenu.hpp"
#include "WAS_Toolbar.hpp"

class AppUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	AppUIEnvironment ();

	void Init (NUIE::NodeEditor* nodeEditorPtr, WAS::FileMenu* fileMenuPtr, WAS::Toolbar* toolbarPtr, HWND parentHandle);
	void OnResize (int x, int y, int width, int height);
	void OnClipboardChanged ();

	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;
	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;

	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;

	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandler&			GetEventHandler () override;
	virtual NUIE::ClipboardHandler&		GetClipboardHandler () override;
	
	virtual void						OnSelectionChanged (const NUIE::Selection& selection) override;
	virtual void						OnUndoStateChanged (const NUIE::UndoState& undoState) override;
	virtual void						OnClipboardStateChanged (const NUIE::ClipboardState& clipboardState) override;
	virtual void						OnIncompatibleVersionPasted (const NUIE::Version&) override;

private:
	void								EnableCommand (UINT commandId, bool isEnabled);

	NE::BasicStringConverter			stringConverter;
	NUIE::BasicSkinParams				skinParams;
	WAS::HwndEventHandler				eventHandler;
	WAS::ClipboardHandler				clipboardHandler;
	NE::EvaluationEnv					evaluationEnv;
	WAS::NodeEditorNodeTreeHwndControl	nodeEditorControl;
	WAS::FileMenu*						fileMenu;
	WAS::Toolbar*						toolbar;
};

#endif
