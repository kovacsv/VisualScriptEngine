#ifndef NODEEDITORCONTROL_HPP
#define NODEEDITORCONTROL_HPP

#include "WinDrawingContext.hpp"
#include "CustomControl.hpp"
#include "BuiltInCommands.hpp"
#include "WindowsAppUtilities.hpp"
#include "NodeEditor.hpp"
#include "ResultImage.hpp"

#include <memory>

class MyCreateNodeCommand : public NUIE::CreateNodeCommand
{
public:
	enum class NodeType
	{
		Integer,
		IntegerRange,
		Point,
		Line,
		Circle,
		Viewer
	};

	MyCreateNodeCommand (NodeType nodeType, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position);

	virtual NUIE::UINodePtr		CreateNode (const NUIE::Point& modelPosition) override;

private:
	NodeType nodeType;
};

class AppEventHandlers : public NUIE::EventHandlers
{
public:
	AppEventHandlers ();

	void						SetWindowHandle (HWND newHwnd);

	virtual void				RedrawRequested () override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, const NUIE::CommandStructure& commands) override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NUIE::UINodePtr& uiNode, const NUIE::CommandStructure& commands) override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::OutputSlotPtr& outputSlot, const NUIE::CommandStructure& commands) override;
	virtual NUIE::CommandPtr	OnContextMenu (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& env, const NUIE::Point& position, const NE::InputSlotPtr& inputSlot, const NUIE::CommandStructure& commands) override;

private:
	HWND hwnd;
};

class MyNodeUIEnvironment : public NUIE::NodeUIEnvironment
{
public:
	MyNodeUIEnvironment (const std::shared_ptr<ResultImageEvaluationData>& evaluationData);

	virtual NUIE::DrawingContext&	GetDrawingContext () override;
	virtual NUIE::SkinParams&		GetSkinParams () override;
	virtual NUIE::EventHandlers&	GetEventHandlers () override;
	virtual NE::EvaluationEnv&		GetEvaluationEnv () override;

	void							Init (HWND hwnd);
	void							DrawToHDC (HWND hwnd);
	void							ChangeContext (HWND hwnd, short contextType);

private:
	std::unique_ptr<WinDrawingContext>	drawingContext;
	NUIE::SkinParams					skinParams;
	AppEventHandlers					eventHandlers;
	NE::EvaluationEnv					evaluationEnv;
};

class NodeEditorControl : public CustomControl
{
public:
	NodeEditorControl (const std::shared_ptr<ResultImageEvaluationData>& evaluationData);

	virtual void		OnCreate (HWND hwnd) override;
	virtual void		OnPaint (HWND hwnd) override;
	virtual void		OnMouseDown (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void		OnMouseUp (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void		OnMouseMove (HWND hwnd, UI::Keys keys, int x, int y) override;
	virtual void		OnMouseWheel (HWND hwnd, UI::Keys keys, int x, int y, int delta) override;
	virtual void		OnMouseDoubleClick (HWND hwnd, UI::Keys keys, UI::MouseButton button, int x, int y) override;
	virtual void		OnResize (HWND hwnd, int newWidth, int newHeight) override;

	void				New ();
	bool				Open (const std::wstring& fileName);
	bool				Save (const std::wstring& fileName);
	void				ChangeContext (short contextType);

private:
	NUIE::KeySet		ConvertKeys (UI::Keys keys);
	NUIE::MouseButton	ConvertMouseButton (UI::MouseButton button);

	MyNodeUIEnvironment		uiEnvironment;
	NUIE::NodeEditor		nodeEditor;
};

#endif
