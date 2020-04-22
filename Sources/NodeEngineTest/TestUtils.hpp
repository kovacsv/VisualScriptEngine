#ifndef TESTUTILS_HPP
#define TESTUTILS_HPP

#include "NUIE_NodeUIEnvironment.hpp"

using namespace NE;
using namespace NUIE;

class TestDrawingEnvironment : public NodeUIDrawingEnvironment
{
public:
	virtual const NE::StringConverter&	GetStringConverter () override;
	virtual const SkinParams&			GetSkinParams () override;
	virtual DrawingContext&				GetDrawingContext () override;
	virtual double						GetWindowScale () override;
};

#endif
