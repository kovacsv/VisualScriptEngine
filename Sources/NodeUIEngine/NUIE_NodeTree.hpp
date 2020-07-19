#ifndef NUIE_NODETREE_HPP
#define NUIE_NODETREE_HPP

#include "NUIE_Geometry.hpp"
#include "NUIE_UINode.hpp"
#include "NUIE_NodeEditor.hpp"

namespace NUIE
{

using CreatorFunction = std::function<NUIE::UINodePtr (const NUIE::Point& position)>;

class NodeTree
{
public:
	class Item
	{
	public:
		Item (const std::wstring& name, const CreatorFunction& creator);

		const std::wstring&		GetName () const;
		const CreatorFunction&	GetCreator () const;

	private:
		std::wstring		name;
		CreatorFunction		creator;
	};

	class Group
	{
	public:
		Group (const std::wstring& name);
		
		void						AddItem (const Item& item);

		const std::wstring&			GetName () const;
		const std::vector<Item>&	GetItems () const;

	private:
		std::wstring		name;
		std::vector<Item>	items;
	};

	NodeTree ();

	size_t						AddGroup (const std::wstring& groupName);
	void						AddItem (size_t groupIndex, const std::wstring& itemName, const CreatorFunction& creator);
	
	const std::vector<Group>&	GetGroups () const;

private:
	std::vector<Group> groups;
};

extern const NodeTree EmptyNodeTree;

void AddNodeTreeToMenuStructure (const NodeTree& nodeTree, const NUIE::Point& position, NUIE::NodeEditor* nodeEditor, MenuCommandStructure& commands);

}

#endif
