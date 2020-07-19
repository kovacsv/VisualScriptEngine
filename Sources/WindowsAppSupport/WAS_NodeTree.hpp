#ifndef WAS_NODETREE_HPP
#define WAS_NODETREE_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

class NodeTreeView
{
public:
	NodeTreeView ();
	~NodeTreeView ();

	bool	Init (HWND parentHandle, int x, int y, int width, int height);
	void	Resize (int x, int y, int width, int height);

	bool	HasGroup (const std::wstring& group) const;
	void	AddGroup (const std::wstring& group);
	void	AddItem (const std::wstring& group, const std::wstring& text, LPARAM lParam);
	void	ExpandAll ();

	HWND	GetListHandle ();

private:
	HWND											listHandle;
	std::unordered_map<std::wstring, HTREEITEM>		groups;
};

}

#endif
