#include "BI_ViewerUINodes.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace BI
{

NE::DynamicSerializationInfo MultiLineViewerNode::serializationInfo (NE::ObjectId ("{2BACB82D-84A6-4472-82CB-786C98A50EF0}"), NE::ObjectVersion (1), MultiLineViewerNode::CreateSerializableInstance);

MultiLineViewerNode::MultiLineViewerNode () :
	MultiLineViewerNode (std::wstring (), NUIE::Point (), 0)
{

}

MultiLineViewerNode::MultiLineViewerNode (const std::wstring& name, const NUIE::Point& position, size_t textsPerPage) :
	NUIE::UINode (name, position),
	textsPerPage (textsPerPage),
	currentPage (1),
	textCount (0)
{

}

MultiLineViewerNode::~MultiLineViewerNode ()
{

}

void MultiLineViewerNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::Localize (L"Input"), nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::Localize (L"Output"))));
}

NE::ValueConstPtr MultiLineViewerNode::Calculate (NE::EvaluationEnv& env) const
{
	return EvaluateInputSlot (NE::SlotId ("in"), env);
}

void MultiLineViewerNode::RegisterParameters (NUIE::NodeParameterList& parameterList) const
{
	class TextPerPageParameter : public NUIE::PositiveIntegerNodeParameter<MultiLineViewerNode>
	{
	public:
		TextPerPageParameter () :
			PositiveIntegerNodeParameter<MultiLineViewerNode> (NE::Localize (L"Texts per page"))
		{

		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue ((int) GetTypedNode (uiNode)->GetTextsPerPage ()));
		}

		virtual bool SetValueInternal (NUIE::NodeInvalidator& invalidator, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetTextsPerPage (NE::IntValue::Get (value));
			invalidator.InvalidateDrawing ();
			return true;
		}
	};

	NUIE::UINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new TextPerPageParameter ()));
}

NUIE::EventHandlerResult MultiLineViewerNode::HandleMouseClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys&, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface&)
{
	if (mouseButton != NUIE::MouseButton::Left) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	if (!HasSpecialRect (env, "minus") || !HasSpecialRect (env, "plus")) {
		return NUIE::EventHandlerResult::EventNotHandled;
	}

	NUIE::Rect minusButtonRect = GetSpecialRect (env, "minus");
	NUIE::Rect plusButtonRect = GetSpecialRect (env, "plus");

	if (minusButtonRect.Contains (position)) {
		currentPage = currentPage - 1;
		ValidateCurrentPage ();
		return NUIE::EventHandlerResult::EventHandled;
	} else if (plusButtonRect.Contains (position)) {
		currentPage = currentPage + 1;
		ValidateCurrentPage ();
		return NUIE::EventHandlerResult::EventHandled;
	}

	return NUIE::EventHandlerResult::EventNotHandled;
}

NUIE::EventHandlerResult MultiLineViewerNode::HandleMouseDoubleClick (NUIE::NodeUIEnvironment& env, const NUIE::ModifierKeys& keys, NUIE::MouseButton mouseButton, const NUIE::Point& position, NUIE::UINodeCommandInterface& commandInterface)
{
	return HandleMouseClick (env, keys, mouseButton, position, commandInterface);
}

bool MultiLineViewerNode::IsForceCalculated () const
{
	return true;
}

void MultiLineViewerNode::UpdateNodeDrawingImage (NUIE::NodeUIDrawingEnvironment& env, NUIE::NodeDrawingImage& drawingImage) const
{
	std::vector<std::wstring> nodeTexts;
	if (HasCalculatedValue ()) {
		NE::ValueConstPtr nodeValue = GetCalculatedValue ();
		if (nodeValue != nullptr) {
			NE::FlatEnumerate (nodeValue, [&] (const NE::ValueConstPtr& value) {
				if (value != nullptr) {
					nodeTexts.push_back (value->ToString (env.GetStringSettings ()));
				} else {
					nodeTexts.push_back (NE::Localize (L"<empty>"));
				}
			});
		}
	}

	textCount = nodeTexts.size ();
	size_t pageCount = GetPageCount ();
	ValidateCurrentPage ();

	std::vector<std::wstring> nodeTextsToShow;
	for (size_t i = 0; i < textsPerPage; ++i) {
		size_t textIndex = (currentPage - 1) * textsPerPage + i;
		if (textIndex < nodeTexts.size ()) {
			nodeTextsToShow.push_back (nodeTexts[textIndex]);
		}
	}

	if (nodeTextsToShow.empty ()) {
		nodeTextsToShow.push_back (NE::Localize (L"<empty>"));
		textCount += 1;
	}

	NUIE::NodePanelDrawer drawer;
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIHeaderPanel (GetNodeName (), NodeUIHeaderPanel::NodeStatus::HasValue)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUISlotPanel (*this, env)));
	drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUIMultiLineTextPanel (nodeTextsToShow, textCount, textsPerPage, env)));
	if (textCount > textsPerPage) {
		drawer.AddPanel (NUIE::NodeUIPanelPtr (new NodeUILeftRightButtonsPanel ("minus", NE::Localize (L"<"), "plus", NE::Localize (L">"), std::to_wstring (currentPage) + L" / " + std::to_wstring (pageCount) + L" (" + std::to_wstring (textCount) + L")", env)));
	}
	drawer.Draw (env, drawingImage);
}

size_t MultiLineViewerNode::GetPageCount () const
{
	size_t pageCount = textCount / textsPerPage;
	if (textCount % textsPerPage != 0) {
		pageCount = pageCount + 1;
	}
	return pageCount;
}

void MultiLineViewerNode::ValidateCurrentPage () const
{
	size_t pageCount = GetPageCount ();
	if (currentPage == 0) {
		currentPage = pageCount;
	}
	if (currentPage > pageCount) {
		currentPage = 1;
	}
}

NE::Stream::Status MultiLineViewerNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	NUIE::UINode::Read (inputStream);
	inputStream.Read (textsPerPage);
	return inputStream.GetStatus ();
}

NE::Stream::Status MultiLineViewerNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	NUIE::UINode::Write (outputStream);
	outputStream.Write (textsPerPage);
	return outputStream.GetStatus ();
}

size_t MultiLineViewerNode::GetTextsPerPage () const
{
	return textsPerPage;
}

void MultiLineViewerNode::SetTextsPerPage (size_t newTextsPerPage)
{
	textsPerPage = newTextsPerPage;
}

}
