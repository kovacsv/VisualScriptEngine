#include "NUIE_Localization.hpp"

#include <vector>
#include <sstream>

namespace NUIE
{

class PoContentReader
{
public:
	PoContentReader () :
		readState (State::WaitForMessageId),
		messageId ()
	{
	}

	bool AddLine (const std::wstring& poLine, const std::function<bool (const std::wstring&, const std::wstring&)>& onMessageFound)
	{
		return ProcessLine (TrimString (poLine), onMessageFound);
	}

private:
	enum class State
	{
		WaitForMessageId,
		WaitForMessageString
	};

	bool ProcessLine (const std::wstring& poLine, const std::function<bool (const std::wstring&, const std::wstring&)>& onMessageFound)
	{
		if (poLine.empty () || poLine[0] == L'#') {
			return true;
		}
		std::wstring command;
		std::wstring str;
		if (!GetPoLineParts (poLine, command, str)) {
			return false;
		}
		if (command == L"msgid") {
			if (readState == State::WaitForMessageId) {
				readState = State::WaitForMessageString;
			} else {
				return false;
			}
			messageId = str;
		} else if (command == L"msgstr") {
			if (readState == State::WaitForMessageString) {
				readState = State::WaitForMessageId;
			} else {
				return false;
			}
			if (ContainsInvalidChar (messageId) || ContainsInvalidChar (str)) {
				return false;
			}
			if (!onMessageFound (RemoveEscapeChars (messageId), RemoveEscapeChars (str))) {
				return false;
			}
			messageId = std::wstring ();
		}
		return true;
	}

	bool GetPoLineParts (const std::wstring& poLine, std::wstring& command, std::wstring& str) const
	{
		static const wchar_t QuotChar = L'"';
		size_t firstQuot = poLine.find_first_of (QuotChar);
		size_t lastQuot = poLine.find_last_of (QuotChar);
		if (firstQuot == std::wstring::npos || lastQuot == std::wstring::npos) {
			return false;
		}
		command = TrimString (poLine.substr (0, firstQuot));
		str = TrimString (poLine.substr (firstQuot + 1, lastQuot - firstQuot - 1));
		return true;
	}

	std::wstring TrimString (const std::wstring& str) const
	{
		static const std::wstring WhiteSpaces = L"\t\r\n ";
		std::wstring result = str;
		result.erase (0, result.find_first_not_of (WhiteSpaces));
		result.erase (result.find_last_not_of (WhiteSpaces) + 1);
		return result;
	}

	bool ContainsInvalidChar (const std::wstring& str) const
	{
		static const wchar_t EscapeChar = L'\\';
		static const wchar_t EscapedChar = L'"';
		size_t pos = str.find_first_of (EscapedChar);
		while (pos != std::wstring::npos) {
			if (pos == 0 || str[pos - 1] != EscapeChar) {
				return true;
			}
			pos = str.find_first_of (EscapedChar, pos + 1);
		}
		return false;
	}

	std::wstring RemoveEscapeChars (const std::wstring& str) const
	{
		static const wchar_t EscapeChar = L'\\';
		std::wstring result = str;
		size_t pos = result.find_first_of (EscapeChar);
		while (pos != std::wstring::npos) {
			result.erase (pos, 1);
			pos = result.find_first_of (EscapeChar);
		}
		return result;
	}

	State			readState;
	std::wstring	messageId;
};

PoDictionarySource::PoDictionarySource (const std::wstring & poContent) :
	poContent (poContent)
{

}

PoDictionarySource::~PoDictionarySource ()
{

}

bool PoDictionarySource::EnumerateEntries (const std::function<bool (const std::wstring&, const std::wstring&)>& processor)
{
	std::wstringstream poContentStream (poContent);
	PoContentReader poReader;
	std::wstring poLine;
	while (std::getline (poContentStream, poLine)) {
		bool success = poReader.AddLine (poLine, [&] (const std::wstring& id, const std::wstring& msg) {
			return processor (id, msg);
		});
		if (!success) {
			return false;
		}
	}
	return true;
}

}
