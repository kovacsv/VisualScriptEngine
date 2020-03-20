#include "WAS_NodeEditorHwndControl.hpp"
#include "WAS_WindowsAppUtils.hpp"
#include "BI_BuiltInCommands.hpp"
#include "NE_Debug.hpp"

namespace WAS
{

NodeTree::Item::Item (const std::wstring& name, const CreatorFunction& creator) :
	name (name),
	creator (creator)
{

}

const std::wstring& NodeTree::Item::GetName () const
{
	return name;
}

const CreatorFunction& NodeTree::Item::GetCreator () const
{
	return creator;
}

NodeTree::Group::Group (const std::wstring& name) :
	name (name)
{

}

void NodeTree::Group::AddItem (const Item& item)
{
	items.push_back (item);
}

const std::wstring& NodeTree::Group::GetName () const
{
	return name;
}

const std::vector<NodeTree::Item>& NodeTree::Group::GetItems () const
{
	return items;
}

NodeTree::NodeTree ()
{

}

void NodeTree::AddItem (const std::wstring& groupName, const Item& item)
{
	Group* group = nullptr;
	auto foundGroup = std::find_if (groups.begin (), groups.end (), [&] (const Group& group) {
		return group.GetName () == groupName;
	});
	if (foundGroup != groups.end ()) {
		group = &*foundGroup;
	} else {
		groups.push_back (Group (groupName));
		group = &groups.back ();
	}
	group->AddItem (item);
}

void NodeTree::AddItem (const std::wstring& groupName, const std::wstring& itemName, const CreatorFunction& creator)
{
	AddItem (groupName, Item (itemName, creator));
}

const std::vector<NodeTree::Group>& NodeTree::GetGroups () const
{
	return groups;
}

const NodeTree EmptyNodeTree;

NodeTreeView::NodeTreeView () :
	listHandle (NULL)
{

}

NodeTreeView::~NodeTreeView ()
{

}

bool NodeTreeView::Init (HWND parentHandle, int x, int y, int width, int height)
{
	listHandle = CreateWindowEx (
		0, WC_TREEVIEW, NULL, WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS, 
		x, y, width, height, parentHandle, NULL, NULL, NULL
	);
	if (DBGERROR (listHandle == NULL)) {
		return false;
	}
	return true;
}

void NodeTreeView::Resize (int x, int y, int width, int height)
{
	if (listHandle == NULL) {
		return;
	}
	MoveWindow (listHandle, x, y, width, height, TRUE);
}

bool NodeTreeView::HasGroup (const std::wstring& group) const
{
	return groups.find (group) != groups.end ();
}

void NodeTreeView::AddGroup (const std::wstring& group)
{
	auto found = groups.find (group);
	if (DBGERROR (found != groups.end ())) {
		return;
	}

	TVINSERTSTRUCT tvInsertStruct;
	ZeroMemory (&tvInsertStruct, sizeof (TVINSERTSTRUCT));
	tvInsertStruct.hParent = NULL;
	tvInsertStruct.hInsertAfter = NULL;
	tvInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsertStruct.item.pszText = (LPWSTR) group.c_str ();
	tvInsertStruct.item.cchTextMax = sizeof (tvInsertStruct.item.pszText) / sizeof (wchar_t);
	tvInsertStruct.item.lParam = (LPARAM) -1;

	HTREEITEM groupItem = (HTREEITEM) SendMessage (listHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
	groups.insert ({ group, groupItem });
}

void NodeTreeView::AddItem (const std::wstring& group, const std::wstring& text, LPARAM lParam)
{
	auto found = groups.find (group);
	if (DBGERROR (found == groups.end ())) {
		return;
	}

	TVINSERTSTRUCT tvInsertStruct;
	ZeroMemory (&tvInsertStruct, sizeof (TVINSERTSTRUCT));
	tvInsertStruct.hParent = found->second;
	tvInsertStruct.hInsertAfter = NULL;
	tvInsertStruct.item.mask = TVIF_TEXT | TVIF_PARAM;
	tvInsertStruct.item.pszText = (LPWSTR) text.c_str ();
	tvInsertStruct.item.cchTextMax = sizeof (tvInsertStruct.item.pszText) / sizeof (wchar_t);
	tvInsertStruct.item.lParam = lParam;
	SendMessage (listHandle, TVM_INSERTITEM, 0, (LPARAM) &tvInsertStruct);
}

void NodeTreeView::ExpandAll ()
{
	for (const auto& it : groups) {
		TreeView_Expand (listHandle, it.second, TVM_EXPAND);
	}
}

HWND NodeTreeView::GetListHandle ()
{
	return listHandle;
}

void AddNodeTreeAsCommands (const NodeTree& nodeTree, NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const NUIE::Point& position, NUIE::MenuCommandStructure& commands)
{
	class CreateNodeCommand : public BI::CreateNodeCommand
	{
	public:
		CreateNodeCommand (NUIE::NodeUIManager& uiManager, NUIE::NodeUIEnvironment& uiEnvironment, const std::wstring& name, const NUIE::Point& position, const CreatorFunction& creator) :
			BI::CreateNodeCommand (name, uiManager, uiEnvironment, position),
			creator (creator)
		{
		}

		virtual NUIE::UINodePtr CreateNode (const NUIE::Point& modelPosition) override
		{
			return creator (modelPosition);
		}

	private:
		CreatorFunction creator;
	};

	for (const NodeTree::Group& group : nodeTree.GetGroups ()) {
		NUIE::GroupMenuCommandPtr groupCommand (new NUIE::GroupMenuCommand (group.GetName ()));
		for (const NodeTree::Item& item : group.GetItems ()) {
			groupCommand->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (uiManager, uiEnvironment, item.GetName (), position, item.GetCreator ())));
		}
		commands.AddCommand (groupCommand);
	}
}

}
