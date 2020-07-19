#include "NUIE_NodeTree.hpp"
#include "NE_Debug.hpp"

namespace NUIE
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

size_t NodeTree::AddGroup (const std::wstring& groupName)
{
	groups.push_back (Group (groupName));
	return groups.size () - 1;
}

void NodeTree::AddItem (size_t groupIndex, const std::wstring& itemName, const CreatorFunction& creator)
{
	groups[groupIndex].AddItem (Item (itemName, creator));
}

const std::vector<NodeTree::Group>& NodeTree::GetGroups () const
{
	return groups;
}

const NodeTree EmptyNodeTree;

void AddNodeTreeToMenuStructure (const NodeTree& nodeTree, const NUIE::Point& position, NUIE::NodeEditor* nodeEditor, MenuCommandStructure& commands)
{
	class CreateNodeCommand : public NUIE::SingleMenuCommand
	{
	public:
		CreateNodeCommand (NUIE::NodeEditor* nodeEditor, const NE::LocString& name, const NUIE::Point& position, const NUIE::CreatorFunction& creator) :
			NUIE::SingleMenuCommand (name, false),
			nodeEditor (nodeEditor),
			position (position),
			creator (creator)
		{

		}

		virtual void Do () override
		{
			NUIE::UINodePtr uiNode = creator (nodeEditor->ViewToModel (position));
			nodeEditor->AddNode (uiNode);
		}

	private:
		NUIE::NodeEditor*		nodeEditor;
		NUIE::Point				position;
		NUIE::CreatorFunction	creator;
	};

	for (const NUIE::NodeTree::Group& group : nodeTree.GetGroups ()) {
		NE::LocString groupMenuCommandName (group.GetName (), NE::LocString::Localization::DoNotLocalize);
		NUIE::MultiMenuCommandPtr multiCommand (new NUIE::MultiMenuCommand (groupMenuCommandName));
		for (const NUIE::NodeTree::Item& item : group.GetItems ()) {
			NE::LocString menuCommandName (item.GetName (), NE::LocString::Localization::DoNotLocalize);
			multiCommand->AddChildCommand (NUIE::MenuCommandPtr (new CreateNodeCommand (nodeEditor, menuCommandName, position, item.GetCreator ())));
		}
		commands.AddCommand (multiCommand);
	}
}

}
