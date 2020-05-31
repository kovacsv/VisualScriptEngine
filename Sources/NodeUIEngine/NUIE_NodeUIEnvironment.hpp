#ifndef NUIE_NODEUIENVIRONMENT_HPP
#define NUIE_NODEUIENVIRONMENT_HPP

#include "NE_StringConverter.hpp"
#include "NE_EvaluationEnv.hpp"

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
	virtual double				GetMouseMoveMinOffset () = 0;
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
