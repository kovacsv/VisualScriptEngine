#include "ViewerUINodes.hpp"
#include "UINodePanels.hpp"
#include "UINodeCommonParameters.hpp"
#include "SkinParams.hpp"

#include <algorithm>

namespace NUIE
{

NE::DynamicSerializationInfo MultiLineViewerUINode::serializationInfo (NE::ObjectId ("{2BACB82D-84A6-4472-82CB-786C98A50EF0}"), NE::ObjectVersion (1), MultiLineViewerUINode::CreateSerializableInstance);

MultiLineViewerUINode::MultiLineViewerUINode () :
	UINode (),
	textsPerPage (0),
	currentPage (1),
	textCount (0)
{

}

MultiLineViewerUINode::MultiLineViewerUINode (const std::wstring& name, const Point& position, size_t textsPerPage) :
	UINode (name, position),
	textsPerPage (textsPerPage),
	currentPage (1),
	textCount (0)
{

}

MultiLineViewerUINode::~MultiLineViewerUINode ()
{

}

void MultiLineViewerUINode::RegisterSlots ()
{
	RegisterUIInputSlot (UIInputSlotPtr (new UIInputSlot (NE::SlotId ("in"), L"Input", nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (UIOutputSlotPtr (new UIOutputSlot (NE::SlotId ("out"), L"Output")));
}

NE::ValuePtr MultiLineViewerUINode::Calculate (NE::EvaluationEnv& env) const
{
	return EvaluateSingleInputSlot (NE::SlotId ("in"), env);
}

void MultiLineViewerUINode::RegisterParameters (NodeParameterList& parameterList) const
{
	class TextPerPageParameter : public PositiveIntegerParameter<MultiLineViewerUINode>
	{
	public:
		TextPerPageParameter () :
			PositiveIntegerParameter<MultiLineViewerUINode> ("MultiLineViewerUINodeTextPerPageParameter", L"Texts per page")
		{

		}

		virtual NE::ValuePtr GetValue (const UINodePtr& uiNode) const override
		{
			std::shared_ptr<MultiLineViewerUINode> viewerNode = NE::Node::Cast<MultiLineViewerUINode> (uiNode);
			return NE::ValuePtr (new NE::IntValue ((int) viewerNode->GetTextsPerPage ()));
		}

		virtual bool SetValue (NodeUIManager& uiManager, NE::EvaluationEnv&, UINodePtr& uiNode, const NE::ValuePtr& value) override
		{
			if (DBGERROR (!CanSetValue (uiNode, value))) {
				return false;
			}
			std::shared_ptr<MultiLineViewerUINode> viewerNode = NE::Node::Cast<MultiLineViewerUINode> (uiNode);
			viewerNode->SetTextsPerPage (NE::IntValue::Get (value));
			uiManager.InvalidateNodeDrawing (viewerNode);
			return true;
		}
	};

	UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NodeParameterPtr (new TextPerPageParameter ()));
}

NUIE::EventHandlerResult MultiLineViewerUINode::HandleMouseClick (NodeUIEnvironment& env, const KeySet&, MouseButton mouseButton, const Point& position)
{
	if (mouseButton != MouseButton::Left) {
		return EventHandlerResult::EventNotHandled;
	}

	if (!HasSpecialRect (env, "minus") || !HasSpecialRect (env, "plus")) {
		return EventHandlerResult::EventNotHandled;
	}

	Rect minusButtonRect = GetSpecialRect (env, "minus");
	Rect plusButtonRect = GetSpecialRect (env, "plus");

	if (minusButtonRect.Contains (position)) {
		currentPage = currentPage - 1;
		ValidateCurrentPage ();
		return EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		currentPage = currentPage + 1;
		ValidateCurrentPage ();
		return EventHandlerResult::EventHandled;
	}

	return EventHandlerResult::EventNotHandled;
}

void MultiLineViewerUINode::UpdateNodeDrawingImage (NodeUIDrawingEnvironment& env, NodeDrawingImage& drawingImage) const
{
	class NodeUIMultiLineTextViewerPanel : public NodeUIMultiLineTextPanel
	{
	public:
		NodeUIMultiLineTextViewerPanel (const std::vector<std::wstring>& nodeTexts, NodeUIDrawingEnvironment& env, size_t textsPerPage, size_t currentPage) :
			NodeUIMultiLineTextPanel (nodeTexts, env, textsPerPage, currentPage)
		{
		
		}

	private:
		virtual Color GetBackgroundColor (NodeUIDrawingEnvironment&) const override
		{
			return Color (255, 255, 100);
		}
	};

	class NodeUILeftRightButtonsViewerPanel : public NodeUILeftRightButtonsPanel
	{
	public:
		NodeUILeftRightButtonsViewerPanel (NodeUIDrawingEnvironment& env, size_t currentPage, size_t pageCount, size_t textCount) :
			NodeUILeftRightButtonsPanel ("minus", L"<", "plus", L">", std::to_wstring (currentPage) + L" / " + std::to_wstring (pageCount) + L" (" + std::to_wstring (textCount) + L")", env)
		{
		
		}
	};
	
	std::vector<std::wstring> nodeTexts = { L"<empty>" };
	DBGASSERT (ValueIsCalculated ());
	NE::ValuePtr nodeValue = GetCalculatedValue ();
	if (nodeValue != nullptr) {
		NE::IListValuePtr listValue = NE::CreateListValue (nodeValue);
		nodeTexts.clear ();
		listValue->Enumerate ([&] (const NE::ValuePtr& value) {
			nodeTexts.push_back (value->ToString ());
		});
	}

	textCount = nodeTexts.size ();
	size_t pageCount = GetPageCount ();
	ValidateCurrentPage ();

	NodeUIPanelDrawer drawer;
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIHeaderPanel (GetNodeName ())));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUISlotPanel (*this, env)));
	drawer.AddPanel (NodeUIPanelPtr (new NodeUIMultiLineTextViewerPanel (nodeTexts, env, textsPerPage, currentPage)));
	if (textCount > textsPerPage) {
		drawer.AddPanel (NodeUIPanelPtr (new NodeUILeftRightButtonsViewerPanel (env, currentPage, pageCount, textCount)));
	}
	drawer.Draw (env, drawingImage);
}

void MultiLineViewerUINode::CalculationPostProcess (const NE::ValuePtr&, NE::EvaluationEnv&) const
{

}

size_t MultiLineViewerUINode::GetPageCount () const
{
	size_t pageCount = textCount / textsPerPage;
	if (textCount % textsPerPage != 0) {
		pageCount = pageCount + 1;
	}
	return pageCount;
}

void MultiLineViewerUINode::ValidateCurrentPage () const
{
	size_t pageCount = GetPageCount ();
	if (currentPage == 0) {
		currentPage = pageCount;
	}
	if (currentPage > pageCount) {
		currentPage = 1;
	}
}

NE::Stream::Status MultiLineViewerUINode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	UINode::Read (inputStream);
	inputStream.Read (textsPerPage);
	return inputStream.GetStatus ();
}

NE::Stream::Status MultiLineViewerUINode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	UINode::Write (outputStream);
	outputStream.Write (textsPerPage);
	return outputStream.GetStatus ();
}

size_t MultiLineViewerUINode::GetTextsPerPage () const
{
	return textsPerPage;
}

void MultiLineViewerUINode::SetTextsPerPage (size_t newTextsPerPage)
{
	textsPerPage = newTextsPerPage;
	InvalidateDrawing ();
}

}
