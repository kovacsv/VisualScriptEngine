#ifndef WAS_CLIPBOARDHANDLER_HPP
#define WAS_CLIPBOARDHANDLER_HPP

#include "WAS_IncludeWindowsHeaders.hpp"
#include "NUIE_ClipboardHandler.hpp"

namespace WAS
{

class ClipboardHandler : public NUIE::ClipboardHandler
{
public:
	ClipboardHandler ();
	virtual ~ClipboardHandler ();

	virtual NUIE::Version	GetCurrentVersion () const override;
	virtual bool			IsCompatibleVersion (const NUIE::Version& version) const override;

	virtual bool			HasClipboardContent () const override;
	virtual bool			GetClipboardContent (std::vector<char>& content) const override;
	virtual bool			SetClipboardContent (const std::vector<char>& content) override;

private:
	UINT	formatId;
};

}

#endif
