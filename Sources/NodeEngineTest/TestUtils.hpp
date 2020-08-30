#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include "NUIE_EventHandler.hpp"
#include "NUIE_ClipboardHandler.hpp"
#include "NUIE_NodeUIEnvironment.hpp"

using namespace NE;
using namespace NUIE;

class TestUIEnvironment : public NodeUIEnvironment
{
public:
	TestUIEnvironment ();
	TestUIEnvironment (const EvaluationDataPtr& data);

	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const SkinParams&			GetSkinParams () override;
	virtual DrawingContext&				GetDrawingContext () override;
	virtual double						GetWindowScale () override;

	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	virtual void						OnRedrawRequested () override;

	virtual EventHandler&				GetEventHandler () override;
	virtual ClipboardHandler&			GetClipboardHandler () override;
	virtual double						GetMouseMoveMinOffset () override;

private:
	NE::EvaluationEnv					evalEnv;
	NUIE::NullEventHandler				eventHandler;
	NUIE::MemoryClipboardHandler		clipboardHandler;
};

#endif
