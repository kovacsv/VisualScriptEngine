#ifndef WAS_HWNDNODEUIENVIRONMENT_HPP
#define WAS_HWNDNODEUIENVIRONMENT_HPP

#include "NUIE_NodeUIEnvironment.hpp"
#include "NUIE_NodeEditor.hpp"
#include "WAS_HwndEventHandlers.hpp"
#include "WAS_NodeEditorNodeListHwndControl.hpp"

namespace WAS
{

class HwndNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	HwndNodeUIEnvironment (	const NE::StringSettingsPtr& stringSettings,
							const NUIE::SkinParamsPtr& skinParams,
							const NE::EvaluationDataPtr& evalData);

	void								Init (NUIE::NodeEditor* nodeEditorPtr, HWND parentHandle);
	void								RegisterNode (const std::wstring& group, const std::wstring& text, const std::function<NUIE::UINodePtr (const NUIE::Point&)>& creator);
	void								OnResize (int x, int y, int width, int height);

	virtual const NE::StringSettings&	GetStringSettings () override;
	virtual const NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::DrawingContext&		GetDrawingContext () override;

	virtual NE::EvaluationEnv&			GetEvaluationEnv () override;
	virtual void						OnEvaluationBegin () override;
	virtual void						OnEvaluationEnd () override;
	virtual void						OnValuesRecalculated () override;
	
	virtual void						OnRedrawRequested () override;
	virtual NUIE::EventHandlers&		GetEventHandlers () override;
	virtual double						GetMouseMoveMinOffset () override;

private:
	NE::StringSettingsPtr				stringSettings;
	NUIE::SkinParamsPtr					skinParams;
	WAS::HwndEventHandlers				eventHandlers;
	NE::EvaluationEnv					evaluationEnv;
	WAS::NodeEditorNodeListHwndControl	nodeEditorControl;
};

}

#endif
