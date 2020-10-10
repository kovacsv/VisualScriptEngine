#ifndef NUIE_NODEUIENVIRONMENT_HPP
#define NUIE_NODEUIENVIRONMENT_HPP

#include "NE_StringConverter.hpp"
#include "NE_EvaluationEnv.hpp"
#include "NUIE_Selection.hpp"
#include "NUIE_UndoState.hpp"
#include "NUIE_ClipboardState.hpp"

#include <memory>

namespace NUIE
{

class SkinParams;
class DrawingContext;
class EventHandler;
class ClipboardHandler;

class NodeUIDrawingEnvironment
{
public:
	NodeUIDrawingEnvironment ();
	virtual ~NodeUIDrawingEnvironment ();

	virtual const NE::StringConverter&	GetStringConverter () = 0;
	virtual const SkinParams&			GetSkinParams () = 0;
	virtual DrawingContext&				GetDrawingContext () = 0;
	virtual double						GetWindowScale () = 0;
};

class NodeUICalculationEnvironment
{
public:
	NodeUICalculationEnvironment ();
	virtual ~NodeUICalculationEnvironment ();

	virtual NE::EvaluationEnv&	GetEvaluationEnv () = 0;
	virtual void				OnEvaluationBegin () = 0;
	virtual void				OnEvaluationEnd () = 0;
	virtual void				OnValuesRecalculated () = 0;
	virtual void				OnRedrawRequested () = 0;
};

class NodeUIInteractionEnvironment
{
public:
	NodeUIInteractionEnvironment ();
	virtual ~NodeUIInteractionEnvironment ();

	virtual EventHandler&		GetEventHandler () = 0;
	virtual ClipboardHandler&	GetClipboardHandler () = 0;
	virtual void				OnSelectionChanged (const Selection& selection) = 0;
	virtual void				OnUndoStateChanged (const UndoState& undoState) = 0;
	virtual void				OnClipboardStateChanged (const ClipboardState& clipboardState) = 0;
};

class NodeUIEnvironment :	public NodeUIDrawingEnvironment,
							public NodeUICalculationEnvironment,
							public NodeUIInteractionEnvironment
{
public:
	NodeUIEnvironment ();
	virtual ~NodeUIEnvironment ();
};

}

#endif
