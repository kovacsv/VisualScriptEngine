#ifndef NUIE_UINODEINVALIDATOR_HPP
#define NUIE_UINODEINVALIDATOR_HPP

#include "NUIE_UINode.hpp"

namespace NUIE
{

class UINodeInvalidator
{
public:
	UINodeInvalidator ();
	virtual ~UINodeInvalidator ();

	void			InvalidateValueAndDrawing ();
	void			RequestRecalculateAndRedraw ();

	virtual void	InvalidateValue () = 0;
	virtual void	InvalidateDrawing () = 0;

	virtual void	RequestRecalculate () = 0;
	virtual void	RequestRedraw () = 0;
};

}

#endif
