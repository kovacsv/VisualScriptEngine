#include "NUIE_NodeTree.hpp"
#include "NE_Debug.hpp"

namespace NUIE
{

NodeTree::Item::Item (const std::wstring& name, const CreatorFunction& creator) :
	name (name),
	iconId (InvalidIconId),
	creator (creator)
{

}

NodeTree::Item::Item (const std::wstring& name, const IconId& iconId, const CreatorFunction& creator) :
	name (name),
	iconId (iconId),
	creator (creator)
{

}

const std::wstring& NodeTree::Item::GetName () const
{
	return name;
}

const IconId& NodeTree::Item::GetIconId () const
{
	return iconId;
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

size_t NodeTree::AddGroup (const std::wstring& groupName)
{
	groups.push_back (Group (groupName));
	return groups.size () - 1;
}

void NodeTree::AddItem (size_t groupIndex, const std::wstring& itemName, const CreatorFunction& creator)
{
	groups[groupIndex].AddItem (Item (itemName, creator));
}

void NodeTree::AddItem (size_t groupIndex, const std::wstring& itemName, const IconId& iconId, const CreatorFunction& creator)
{
	groups[groupIndex].AddItem (Item (itemName, iconId, creator));
}

const std::vector<NodeTree::Group>& NodeTree::GetGroups () const
{
	return groups;
}

const NodeTree EmptyNodeTree;

void AddNodeTreeToMenuStructure (const NodeTree& nodeTree, const Point& position, NodeEditor* nodeEditor, MenuCommandStructure& commands)
{
	class CreateNodeCommand : public SingleMenuCommand
	{
	public:
		CreateNodeCommand (NodeEditor* nodeEditor, const NE::LocString& name, const Point& position, const CreatorFunction& creator) :
			SingleMenuCommand (name, false),
			nodeEditor (nodeEditor),
			position (position),
			creator (creator)
		{

		}

		virtual bool WillModify () const override
		{
			return true;
		}

		virtual void DoModification () override
		{
			UINodePtr uiNode = creator (nodeEditor->ViewToModel (position));
			nodeEditor->AddNode (uiNode);
		}

	private:
		NodeEditor*			nodeEditor;
		Point				position;
		CreatorFunction		creator;
	};

	for (const NodeTree::Group& group : nodeTree.GetGroups ()) {
		NE::LocString groupMenuCommandName (group.GetName (), NE::LocString::Localization::DoNotLocalize);
		MultiMenuCommandPtr multiCommand (new MultiMenuCommand (groupMenuCommandName));
		for (const NodeTree::Item& item : group.GetItems ()) {
			NE::LocString menuCommandName (item.GetName (), NE::LocString::Localization::DoNotLocalize);
			multiCommand->AddChildCommand (MenuCommandPtr (new CreateNodeCommand (nodeEditor, menuCommandName, position, item.GetCreator ())));
		}
		commands.AddCommand (multiCommand);
	}
}

}
