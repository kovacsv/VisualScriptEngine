#ifndef WAS_HWNDNODEUIENVIRONMENT_HPP
#define WAS_HWNDNODEUIENVIRONMENT_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeEditor.hpp"
#include "WAS_HwndEventHandlers.hpp"
#include "WAS_NodeEditorHwndControl.hpp"

namespace WAS
{

class HwndNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	HwndNodeUIEnvironment (	const WAS::NodeEditorHwndBasedControlPtr& nodeEditorControl,
							const NE::StringConverterPtr& stringConverter,
							const NUIE::SkinParamsPtr& skinParams,
							const WAS::HwndEventHandlersPtr eventHandlers,
							const NE::EvaluationDataPtr& evalData);

	void								Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle);
	void								OnResize (int x, int y, int width, int height);

	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;
	virtual double						GetWindowScale () override;

	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandlers&		GetEventHandlers () override;

	WAS::NodeEditorHwndBasedControlPtr	GetNodeEditorControl () const;

private:
	WAS::NodeEditorHwndBasedControlPtr	nodeEditorControl;
	NE::StringConverterPtr				stringConverter;
	NUIE::SkinParamsPtr					skinParams;
	WAS::HwndEventHandlersPtr			eventHandlers;
	NE::EvaluationEnv					evaluationEnv;
};

}

#endif
