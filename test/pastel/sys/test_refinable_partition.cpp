// Description: Testing for refinable partition
// DocumentationOf: refinable_partition.h

#include "test/test_init.h"

#include <pastel/sys/refinable_partition/refinable_partition.h>

#include <algorithm>
#include <iostream>

namespace
{

	typedef RefinablePartition<integer, integer>
		Partition;
	typedef Partition::Set_Iterator 
		Set_Iterator;
	typedef Partition::Set_ConstIterator 
		Set_ConstIterator;
	typedef Partition::Element_Iterator 
		Element_Iterator;
	typedef Partition::Element_ConstIterator 
		Element_ConstIterator;

	template <integer N, typename Type>
	bool same(Type (&data)[N], 
		Partition::Set_ConstIterator set)
	{
		if (N != set->elements())
		{
			return false;
		}

		std::vector<Type> aSet(
			std::begin(data), std::end(data));
		std::sort(aSet.begin(), aSet.end());

		auto iter = set->cbegin();			
		auto end = set->cend();

		std::vector<Type> bSet;
		while(iter != end)
		{
			bSet.push_back(**iter);
			++iter;
		}
		std::sort(bSet.begin(), bSet.end());

		return std::equal(aSet.begin(),	aSet.end(),
			bSet.begin());
	}

}

TEST_CASE("Various (RefinablePartition)")
{
	integer data[] = {0, 1, 2, 3, 4};

	Partition partition;

	Set_Iterator set =
		partition.addSet();

	partition.insert(
		set, std::begin(data), std::end(data));
	{
		REQUIRE(partition.splits() == 0);
		REQUIRE(partition.sets() == 1);
		REQUIRE(partition.elements() == 5);
		REQUIRE(same(data, partition.setBegin()));
		REQUIRE(*partition.setBegin() == 0);
	}

	partition.mark(
		partition.elementBegin());
	{
		REQUIRE(partition.splits() == 1);
	}

	partition.mark(
		std::next(partition.elementBegin(), 2));
	{
		REQUIRE(partition.splits() == 1);
	}

	partition.mark(
		std::next(partition.elementBegin(), 4));
	{
		REQUIRE(partition.setBegin()->marked() == 3);
		REQUIRE(partition.setBegin()->unmarked() == 2);
	}

	partition.split();
	{
		REQUIRE(partition.splits() == 0);
		REQUIRE(partition.sets() == 2);
		REQUIRE(partition.elements() == 5);

		// In the splitting, the smaller part
		// should get the new set.
		int data[] = {0, 2, 4};
		REQUIRE(same(data, partition.setBegin()));

		auto set = partition.setBegin();
		++set;

		int data2[] = {1, 3};
		REQUIRE(same(data2, set));
		REQUIRE(set->marked() == 0);
		REQUIRE(set->unmarked() == 2);
		REQUIRE(set->elements() == 2);

		REQUIRE(std::next(partition.elementBegin())->set() == set);
		REQUIRE(std::next(partition.elementBegin(), 3)->set() == set);
	}

	partition.mark(
		std::next(partition.elementBegin(), 1));
	partition.mark(
		std::next(partition.elementBegin(), 3));
	{
		Partition::Set_ConstIterator set = 
			partition.setBegin();
		++set;

		REQUIRE(set->elements() == 2);
		REQUIRE(set->marked() == 2);
		REQUIRE(set->unmarked() == 0);
		REQUIRE(partition.splits() == 1);
	}

	partition.split();
	{
		// Nothing should happen when all
		// the elements in a set are marked.

		REQUIRE(partition.sets() == 2);

		REQUIRE(partition.splits() == 0);
	}

	partition.clear();
	REQUIRE(partition.splits() == 0);
	REQUIRE(partition.sets() == 0);
	REQUIRE(partition.elements() == 0);
}

TEST_CASE("Copy (RefinablePartition)")
{
	Partition partition;
	Set_ConstIterator set = partition.addSet(0);
	partition.insertOne(set, 0);
	partition.insertOne(set, 1);
	partition.insertOne(set, 2);

	partition.mark(partition.elementBegin());

	Partition copy(partition);
	{
		REQUIRE(partition.elements() == copy.elements());
		REQUIRE(partition.splits() == copy.splits());
		REQUIRE(partition.sets() == copy.sets());

		Set_ConstIterator set =
			partition.cSetBegin();
		Set_ConstIterator copySet =
			copy.cSetBegin();

		REQUIRE(set->elements() == copySet->elements());
		REQUIRE(set->marked() == copySet->marked());
		REQUIRE(set->unmarked() == copySet->unmarked());
	}
}

namespace
{

	void print(const Partition& partition)
	{
		std::for_each(
			partition.cSetBegin()->cbegin(),
			partition.cSetBegin()->cend(),
			[&](const Element_ConstIterator& element)
		{
			std::cout << *element << " ";
		});
		std::cout << std::endl;
	}

}

TEST_CASE("Remove (RefinablePartition)")
{
	integer data[] = {0, 1, 2, 3, 4};

	Partition partition;

	partition.addSet(
		std::begin(data), std::end(data));

	partition.erase(
		std::next(partition.elementBegin()));
	{
		Set_ConstIterator set =
			partition.setBegin();

		REQUIRE(set->elements() == 4);
		REQUIRE(set->marked() == 0);
		REQUIRE(set->unmarked() == 4);
		REQUIRE(partition.elements() == 4);
		REQUIRE(partition.sets() == 1);

		integer data[] = {0, 2, 3, 4};
		REQUIRE(same(data, set));
	}

	partition.mark(
		std::next(partition.elementBegin()));
	{
		Set_ConstIterator set =
			partition.setBegin();

		REQUIRE(set->elements() == 4);
		REQUIRE(set->marked() == 1);
		REQUIRE(set->unmarked() == 3);
		REQUIRE(partition.elements() == 4);
		REQUIRE(partition.splits() == 1);
		REQUIRE(partition.sets() == 1);
	}

	partition.erase(
		std::next(partition.elementBegin()));
	{
		Set_ConstIterator set =
			partition.setBegin();

		REQUIRE(set->elements() == 3);
		REQUIRE(set->marked() == 0);
		REQUIRE(set->unmarked() == 3);
		REQUIRE(partition.elements() == 3);
		REQUIRE(partition.splits() == 0);
		REQUIRE(partition.sets() == 1);

		integer data[] = {0, 3, 4};
		REQUIRE(same(data, set));
	}

	partition.erase(
		partition.elementBegin());
	partition.erase(
		partition.elementBegin());
	partition.erase(
		partition.elementBegin());
	{
		Set_ConstIterator set =
			partition.setBegin();

		REQUIRE(set->elements() == 0);
		REQUIRE(set->marked() == 0);
		REQUIRE(set->unmarked() == 0);
		REQUIRE(partition.elements() == 0);
		REQUIRE(partition.splits() == 0);
		REQUIRE(partition.sets() == 1);
	}

	partition.erase(
		partition.setBegin());
	{
		REQUIRE(partition.elements() == 0);
		REQUIRE(partition.splits() == 0);
		REQUIRE(partition.sets() == 0);
	}

	Set_Iterator set = partition.addSet(
		std::begin(data), std::end(data));

	partition.erase(set);
	{
		REQUIRE(partition.elements() == 0);
		REQUIRE(partition.splits() == 0);
		REQUIRE(partition.sets() == 0);
	}

}
