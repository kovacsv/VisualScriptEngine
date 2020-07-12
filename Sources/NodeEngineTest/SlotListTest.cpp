#include "SimpleTest.hpp"
#include "NE_SlotList.hpp"

using namespace NE;

namespace SlotListTest
{

class TestSlot
{
public:
	TestSlot (const NE::SlotId& slotId) :
		slotId (slotId)
	{

	}

	const NE::SlotId& GetId () const
	{
		return slotId;
	}

private:
	NE::SlotId slotId;
};

static std::vector<NE::SlotId> GetSlotIds (const SlotList<TestSlot>& slotList)
{
	std::vector<NE::SlotId> slotIds;
	slotList.Enumerate ([&] (const std::shared_ptr<const TestSlot>& slot) {
		slotIds.push_back (slot->GetId ());
		return true;
	});
	return slotIds;
}

TEST (SlotListInsertTest)
{
	SlotList<TestSlot> slotList;
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("a"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("b"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("c"))));

	std::vector<NE::SlotId> slotIds = GetSlotIds (slotList);
	ASSERT (slotIds == std::vector<NE::SlotId> ({ NE::SlotId ("a"), NE::SlotId ("b"), NE::SlotId ("c") }));
}

TEST (SlotListInsertBeforeTestMiddle)
{
	SlotList<TestSlot> slotList;
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("a"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("b"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("c"))));
	slotList.InsertBefore (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("d"))), NE::SlotId ("b"));

	std::vector<NE::SlotId> slotIds = GetSlotIds (slotList);
	ASSERT (slotIds == std::vector<NE::SlotId> ({ NE::SlotId ("a"), NE::SlotId ("d"), NE::SlotId ("b"), NE::SlotId ("c") }));
}

TEST (SlotListInsertBeforeTestBeginning)
{
	SlotList<TestSlot> slotList;
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("a"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("b"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("c"))));
	slotList.InsertBefore (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("d"))), NE::SlotId ("a"));

	std::vector<NE::SlotId> slotIds = GetSlotIds (slotList);
	ASSERT (slotIds == std::vector<NE::SlotId> ({ NE::SlotId ("d"), NE::SlotId ("a"), NE::SlotId ("b"), NE::SlotId ("c") }));
}

TEST (SlotListInsertAfterTestMiddle)
{
	SlotList<TestSlot> slotList;
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("a"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("b"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("c"))));
	slotList.InsertAfter (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("d"))), NE::SlotId ("a"));

	std::vector<NE::SlotId> slotIds = GetSlotIds (slotList);
	ASSERT (slotIds == std::vector<NE::SlotId> ({ NE::SlotId ("a"), NE::SlotId ("d"), NE::SlotId ("b"), NE::SlotId ("c") }));
}

TEST (SlotListInsertAfterTestEnd)
{
	SlotList<TestSlot> slotList;
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("a"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("b"))));
	slotList.Insert (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("c"))));
	slotList.InsertAfter (std::shared_ptr<TestSlot> (new TestSlot (NE::SlotId ("d"))), NE::SlotId ("c"));

	std::vector<NE::SlotId> slotIds = GetSlotIds (slotList);
	ASSERT (slotIds == std::vector<NE::SlotId> ({ NE::SlotId ("a"), NE::SlotId ("b"), NE::SlotId ("c"), NE::SlotId ("d") }));
}

}
