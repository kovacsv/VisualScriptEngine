#include "NUIE_UINodeInvalidator.hpp"

namespace NUIE
{

UINodeInvalidator::UINodeInvalidator ()
{

}

UINodeInvalidator::~UINodeInvalidator ()
{

}

void UINodeInvalidator::InvalidateValueAndDrawing ()
{
	InvalidateValue ();
	InvalidateDrawing ();
}

void UINodeInvalidator::RequestRecalculateAndRedraw ()
{
	RequestRecalculate ();
	RequestRedraw ();
}

}
