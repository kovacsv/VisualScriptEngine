#include "BI_ViewerUINodes.hpp"
#include "BI_UINodePanels.hpp"
#include "NE_Localization.hpp"
#include "NUIE_NodeCommonParameters.hpp"
#include "NUIE_SkinParams.hpp"

#include <algorithm>

namespace BI
{

DYNAMIC_SERIALIZATION_INFO (ViewerNode, 1, "{417392AA-F72D-4E84-8F58-766D0AAC07FC}");
DYNAMIC_SERIALIZATION_INFO (MultiLineViewerNode, 1, "{2BACB82D-84A6-4472-82CB-786C98A50EF0}");

ViewerNode::Layout::Layout () :
	HeaderWithSlotsAndTextLayout ()
{

}

std::wstring ViewerNode::Layout::GetText (const BasicUINode& uiNode, const NE::StringConverter& stringConverter) const
{
	if (uiNode.HasCalculatedValue ()) {
		NE::ValueConstPtr val = uiNode.GetCalculatedValue ();
		if (val != nullptr) {
			return val->ToString (stringConverter);
		}
	}
	return NE::LocalizeString (L"<empty>");
}

ViewerNode::ViewerNode () :
	ViewerNode (NE::LocString (), NUIE::Point ())
{

}

ViewerNode::ViewerNode (const NE::LocString& name, const NUIE::Point& position) :
	BasicUINode (name, position, UINodeLayoutPtr (new Layout ()))
{

}

ViewerNode::~ViewerNode ()
{

}

void ViewerNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::LocString (L"Input"), nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::LocString (L"Output"))));
}

bool ViewerNode::IsForceCalculated () const
{
	return true;
}

NE::ValueConstPtr ViewerNode::Calculate (NE::EvaluationEnv& env) const
{
	NE::ValueConstPtr val = EvaluateInputSlot (NE::SlotId ("in"), env);
	if (val == nullptr) {
		return nullptr;
	}
	return val->Clone ();
}

NE::Stream::Status ViewerNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	return inputStream.GetStatus ();
}

NE::Stream::Status ViewerNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
	return outputStream.GetStatus ();
}

MultiLineViewerNode::Layout::Layout (	const std::string& leftButtonId,
										const std::wstring& leftButtonText,
										const std::string& rightButtonId,
										const std::wstring& rightButtonText) :
	HeaderWithSlotsAndMultilineTextLayout (leftButtonId, leftButtonText, rightButtonId, rightButtonText)
{

}

void MultiLineViewerNode::Layout::GetTextInfo (const BasicUINode& uiNode,
												const NE::StringConverter& stringConverter,
												std::vector<std::wstring>& texts,
												size_t& textsPerPage) const
{
	if (uiNode.HasCalculatedValue ()) {
		NE::ValueConstPtr nodeValue = uiNode.GetCalculatedValue ();
		if (nodeValue != nullptr) {
			NE::FlatEnumerate (nodeValue, [&] (const NE::ValueConstPtr& value) {
				if (value != nullptr) {
					texts.push_back (value->ToString (stringConverter));
				} else {
					texts.push_back (NE::LocalizeString (L"<empty>"));
				}
				return true;
			});
		}
	}

	const MultiLineViewerNode* viewerNode = dynamic_cast<const MultiLineViewerNode*> (&uiNode);
	textsPerPage = viewerNode->GetTextsPerPage ();
}

MultiLineViewerNode::MultiLineViewerNode () :
	MultiLineViewerNode (NE::LocString (), NUIE::Point (), 0)
{

}

MultiLineViewerNode::MultiLineViewerNode (const NE::LocString& name, const NUIE::Point& position, size_t textsPerPage) :
	BasicUINode (name, position, UINodeLayoutPtr (new Layout ("minus", NE::LocalizeString (L"<"), "plus", NE::LocalizeString (L">")))),
	textsPerPage (textsPerPage)
{

}

MultiLineViewerNode::~MultiLineViewerNode ()
{

}

void MultiLineViewerNode::Initialize ()
{
	RegisterUIInputSlot (NUIE::UIInputSlotPtr (new NUIE::UIInputSlot (NE::SlotId ("in"), NE::LocString (L"Input"), nullptr, NE::OutputSlotConnectionMode::Single)));
	RegisterUIOutputSlot (NUIE::UIOutputSlotPtr (new NUIE::UIOutputSlot (NE::SlotId ("out"), NE::LocString (L"Output"))));
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
			PositiveIntegerNodeParameter<MultiLineViewerNode> (NE::LocString (L"Texts per page"))
		{

		}

		virtual NE::ValueConstPtr GetValueInternal (const NUIE::UINodeConstPtr& uiNode) const override
		{
			return NE::ValuePtr (new NE::IntValue ((int) GetTypedNode (uiNode)->GetTextsPerPage ()));
		}

		virtual bool SetValueInternal (NUIE::UINodeInvalidator& invalidator, NE::EvaluationEnv&, NUIE::UINodePtr& uiNode, const NE::ValueConstPtr& value) override
		{
			GetTypedNode (uiNode)->SetTextsPerPage (NE::IntValue::Get (value));
			invalidator.InvalidateDrawing ();
			return true;
		}
	};

	BasicUINode::RegisterParameters (parameterList);
	parameterList.AddParameter (NUIE::NodeParameterPtr (new TextPerPageParameter ()));
}

bool MultiLineViewerNode::IsForceCalculated () const
{
	return true;
}

NE::Stream::Status MultiLineViewerNode::Read (NE::InputStream& inputStream)
{
	NE::ObjectHeader header (inputStream);
	BasicUINode::Read (inputStream);
	inputStream.Read (textsPerPage);
	return inputStream.GetStatus ();
}

NE::Stream::Status MultiLineViewerNode::Write (NE::OutputStream& outputStream) const
{
	NE::ObjectHeader header (outputStream, serializationInfo);
	BasicUINode::Write (outputStream);
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
