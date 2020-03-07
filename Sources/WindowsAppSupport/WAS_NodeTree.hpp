#ifndef WAS_NODETREE_HPP
#define WAS_NODETREE_HPP

#include "NUIE_NodeEditor.hpp"
#include "WAS_IncludeWindowsHeaders.hpp"

namespace WAS
{

using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

class NodeTree
{
public:
	class Item
	{
	public:
		Item (const std::wstring& name, const CreatorFunction& creator);

		std::wstring		name;
		CreatorFunction		creator;
	};

	class Group
	{
	public:
		Group (const std::wstring& name);

		std::wstring		name;
		std::vector<Item>	items;
	};

	NodeTree ();

	void						AddItem (const std::wstring& groupName, const std::wstring& itemName, const CreatorFunction& creator);
	const std::vector<Group>&	GetGroups () const;

private:
	std::vector<Group> groups;
};

extern const NodeTree EmptyNodeTree;

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
