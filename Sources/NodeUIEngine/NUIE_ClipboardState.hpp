#ifndef NUIE_CLIPBOARDSTATE_HPP
#define NUIE_CLIPBOARDSTATE_HPP

namespace NUIE
{

class ClipboardState
{
public:
	ClipboardState (bool hasContent);

	bool	HasContent () const;

private:
	bool	hasContent;
};

}

#endif
