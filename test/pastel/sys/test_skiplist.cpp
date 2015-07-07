// Description: Testing for skip lists.
// DocumentationOf: skiplist.h

#include "test/test_init.h"

#include "pastel/sys/skiplist.h"
#include "pastel/sys/random/random_integer.h"

#include <boost/range/adaptor/reversed.hpp> 

#include <iostream>
#include <list>

namespace
{

	using List = SkipList_MultiSet<integer>;
	using Iterator = List::Iterator;
	using ConstIterator = List::ConstIterator;

	void testSimple(integer maxHeight)
	{
		List list;
		list.setMaxHeight(maxHeight);
		REQUIRE(testInvariants(list));

		ConstIterator listEnd = list.cend();

		{
			integer insertSet[] = 
			{
				1, 5, 3, 4, 4, 4, 8, 7, 6, 9, 2
			};

			for (integer i : insertSet)
			{
				list.insert(i);
				REQUIRE(testInvariants(list));
			}
		}

		{
			integer correctSet[] =
			{
				1, 2, 3, 4, 4, 4, 5, 6, 7, 8, 9
			};

			REQUIRE(list.size() == 11);
			REQUIRE(list.uniqueKeys() == 9);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(8);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				1, 2, 3, 4, 4, 4, 5, 6, 7, 9
			};

			REQUIRE(list.size() == 10);
			REQUIRE(list.uniqueKeys() == 8);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(4);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				1, 2, 3, 5, 6, 7, 9
			};

			REQUIRE(list.size() == 7);
			REQUIRE(list.uniqueKeys() == 7);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(2);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				1, 3, 5, 6, 7, 9
			};

			REQUIRE(list.size() == 6);
			REQUIRE(list.uniqueKeys() == 6);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(1);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				3, 5, 6, 7, 9
			};

			REQUIRE(list.size() == 5);
			REQUIRE(list.uniqueKeys() == 5);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(9);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				3, 5, 6, 7
			};

			REQUIRE(list.size() == 4);
			REQUIRE(list.uniqueKeys() == 4);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(6);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				3, 5, 7
			};

			REQUIRE(list.size() == 3);
			REQUIRE(list.uniqueKeys() == 3);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(5);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				3, 7
			};

			REQUIRE(list.size() == 2);
			REQUIRE(list.uniqueKeys() == 2);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(7);
		{
			REQUIRE(testInvariants(list));

			integer correctSet[] =
			{
				3
			};

			REQUIRE(list.size() == 1);
			REQUIRE(list.uniqueKeys() == 1);
			REQUIRE(boost::equal(list, correctSet));
		}

		list.erase(3);
		{
			REQUIRE(testInvariants(list));

			REQUIRE(list.size() == 0);
			REQUIRE(list.uniqueKeys() == 0);
			REQUIRE(list.empty());
		}

		ConstIterator one = list.insert(1).first;
		REQUIRE(testInvariants(list));
		list.insert(5);
		REQUIRE(testInvariants(list));
		list.insert(3);
		REQUIRE(testInvariants(list));
		list.insert(4);
		REQUIRE(testInvariants(list));
		ConstIterator four = list.insert(4).first;
		REQUIRE(testInvariants(list));
		list.insert(4);
		REQUIRE(testInvariants(list));
		list.insert(8);
		REQUIRE(testInvariants(list));
		list.insert(7);
		REQUIRE(testInvariants(list));
		list.insert(6);
		REQUIRE(testInvariants(list));
		list.insert(9);
		REQUIRE(testInvariants(list));
		list.insert(2);
		REQUIRE(testInvariants(list));

		{
			REQUIRE(four.equivalents() == 3);
			REQUIRE(one.equivalents() == 1);
		}

		integer correctSet[] =
		{
			1, 2, 3, 4, 4, 4, 5, 6, 7, 8, 9
		};

		{
			// When I adapted Node* by the boost::iterator_adaptor,
			// it was adapted as a random-access iterator; then
			// the distance was given by the pointer difference,
			// which is a bug. This also affected boost::equal,
			// which compares the sizes of the ranges first.
			// Adding the bidirectional-traversal tag fixed the
			// bug. This line tests for that.
			REQUIRE(boost::distance(list) == 11);
			REQUIRE(boost::equal(list, correctSet));

			// Test iterator decrement.
			REQUIRE(boost::equal(
				list | boost::adaptors::reversed, 
				correctSet | boost::adaptors::reversed));
		}

		// Test lowerBound(), upperBound(), and find().
		{
			REQUIRE(*list.lowerBound(0) == 1);
			REQUIRE(*list.lowerBound(1) == 1);
			REQUIRE(*list.lowerBound(2) == 2);
			REQUIRE(*list.lowerBound(4) == 4);
			REQUIRE(*list.lowerBound(8) == 8);
			REQUIRE(*list.lowerBound(9) == 9);
			REQUIRE(list.lowerBound(10) == list.cend());

			REQUIRE(*list.lowerBound(0, four) == 1);
			REQUIRE(*list.lowerBound(1, four) == 1);
			REQUIRE(*list.lowerBound(2, four) == 2);
			REQUIRE(*list.lowerBound(4, four) == 4);
			REQUIRE(*list.lowerBound(8, four) == 8);
			REQUIRE(*list.lowerBound(9, four) == 9);
			REQUIRE(list.lowerBound(10, four) == list.cend());

			REQUIRE(*list.upperBound(0) == 1);
			REQUIRE(*list.upperBound(1) == 2);
			REQUIRE(*list.upperBound(2) == 3);
			REQUIRE(*list.upperBound(4) == 5);
			REQUIRE(*list.upperBound(8) == 9);
			REQUIRE(list.upperBound(9) == list.cend());

			REQUIRE(*list.upperBound(0, four) == 1);
			REQUIRE(*list.upperBound(1, four) == 2);
			REQUIRE(*list.upperBound(2, four) == 3);
			REQUIRE(*list.upperBound(4, four) == 5);
			REQUIRE(*list.upperBound(8, four) == 9);
			REQUIRE(list.upperBound(9, four) == list.cend());

			REQUIRE(list.find(0) == list.end());
			REQUIRE(list.find(1) != list.end());
			REQUIRE(*list.find(1) == 1);

			REQUIRE(list.find(2) != list.end());
			REQUIRE(*list.find(2) == 2);

			REQUIRE(list.find(4) != list.end());
			REQUIRE(*list.find(4) == 4);

			REQUIRE(list.find(8) != list.end());
			REQUIRE(*list.find(8) == 8);

			REQUIRE(list.find(9) != list.end());
			REQUIRE(*list.find(9) == 9);

			REQUIRE(list.find(10) == list.end());
		}

		// Test copy-construction, and move-construction.
		{
			List copyList(list);
			REQUIRE(testInvariants(copyList));
			REQUIRE(testInvariants(list));
			REQUIRE(copyList.maxHeight() == list.maxHeight());
			REQUIRE(copyList.height() == list.height());
			REQUIRE(copyList.size() == list.size());
			REQUIRE(copyList.uniqueKeys() == list.uniqueKeys());

			REQUIRE(list.size() == 11);
			REQUIRE(!list.empty());
			REQUIRE(listEnd == list.cend());
			REQUIRE(boost::equal(list, correctSet));

			REQUIRE(copyList.size() == 11);
			REQUIRE(!copyList.empty());
			REQUIRE(listEnd != copyList.cend());
			REQUIRE(boost::equal(copyList, correctSet));

			List moveList(std::move(copyList));
			REQUIRE(testInvariants(moveList));
			REQUIRE(testInvariants(copyList));

			REQUIRE(copyList.size() == 0);
			REQUIRE(copyList.empty());

			REQUIRE(moveList.size() == 11);
			REQUIRE(!moveList.empty());
			REQUIRE(boost::equal(moveList, correctSet));
		}

		// Test copy-assign, move-assign, and swap.
		{
			List copyList;
			REQUIRE(testInvariants(copyList));

			copyList = list;
			REQUIRE(testInvariants(copyList));
			REQUIRE(testInvariants(list));
			REQUIRE(copyList.maxHeight() == list.maxHeight());
			REQUIRE(copyList.height() == list.height());
			REQUIRE(copyList.size() == list.size());
			REQUIRE(copyList.uniqueKeys() == list.uniqueKeys());

			REQUIRE(list.size() == 11);
			REQUIRE(!list.empty());
			REQUIRE(boost::equal(list, correctSet));

			REQUIRE(copyList.size() == 11);
			REQUIRE(!copyList.empty());
			REQUIRE(boost::equal(copyList, correctSet));

			List moveList;
			REQUIRE(testInvariants(moveList));

			moveList = std::move(list);
			REQUIRE(testInvariants(moveList));
			REQUIRE(testInvariants(list));
			REQUIRE(moveList.maxHeight() == maxHeight);

			REQUIRE(list.size() == 0);
			REQUIRE(list.empty());

			REQUIRE(moveList.size() == 11);
			REQUIRE(!moveList.empty());
			REQUIRE(boost::equal(moveList, correctSet));

			moveList.swap(list);
			REQUIRE(testInvariants(moveList));
			REQUIRE(testInvariants(list));

			REQUIRE(list.size() == 11);
			REQUIRE(!list.empty());

			REQUIRE(moveList.size() == 0);
			REQUIRE(moveList.empty());
		}
	}

	void testCount(integer maxHeight)
	{
		List list;
		list.setMaxHeight(maxHeight);

		list.insert(1);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 0);
			REQUIRE(list.count(1) == 1);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(2);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 0);
			REQUIRE(list.count(1) == 1);
			REQUIRE(list.count(2) == 1);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(0);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 1);
			REQUIRE(list.count(1) == 1);
			REQUIRE(list.count(2) == 1);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(1);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 1);
			REQUIRE(list.count(1) == 2);
			REQUIRE(list.count(2) == 1);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(0);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 2);
			REQUIRE(list.count(1) == 2);
			REQUIRE(list.count(2) == 1);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(2);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 2);
			REQUIRE(list.count(1) == 2);
			REQUIRE(list.count(2) == 2);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(1);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 2);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 2);
			REQUIRE(list.count(3) == 0);
		}

		list.insert(0);
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 3);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 2);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(2));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 3);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 1);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(0));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 2);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 1);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(2));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 2);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(0));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 1);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(0));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 0);
			REQUIRE(list.count(1) == 3);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(1));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 0);
			REQUIRE(list.count(1) == 2);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(1));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 0);
			REQUIRE(list.count(1) == 1);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}

		list.erase(list.lowerBound(1));
		{
			REQUIRE(testInvariants(list));
			REQUIRE(list.count(-1) == 0);
			REQUIRE(list.count(0) == 0);
			REQUIRE(list.count(1) == 0);
			REQUIRE(list.count(2) == 0);
			REQUIRE(list.count(3) == 0);
		}
	}

	void testValueMap(integer maxHeight)
	{
		SkipList_MultiMap<integer, integer> list;
		list.setMaxHeight(maxHeight);

		REQUIRE(testInvariants(list));

		list.insert(1, 1);
		REQUIRE(testInvariants(list));
		list.insert(5, 2);
		REQUIRE(testInvariants(list));
		list.insert(3, 3);
		REQUIRE(testInvariants(list));
		list.insert(4, 4);
		REQUIRE(testInvariants(list));
		list.insert(4, 5);
		REQUIRE(testInvariants(list));
		list.insert(4, 6);
		REQUIRE(testInvariants(list));
		list.insert(2, 7);
		REQUIRE(testInvariants(list));
		list.insert(7, 8);
		REQUIRE(testInvariants(list));
		list.insert(6, 9);
		REQUIRE(testInvariants(list));

		integer correctSet[] = 
		{
			1, 7, 3, 4, 5, 6, 2, 9, 8
		};

		{
			// A skip-list with values has iterators
			// which derereference to the value.
			REQUIRE(!list.empty());
			REQUIRE(list.size() == 9);
			REQUIRE(boost::equal(list, correctSet));
		}
	}

	void testValueSet(integer maxHeight)
	{
		SkipList_MultiSet<integer> list;
		list.setMaxHeight(maxHeight);

		REQUIRE(testInvariants(list));

		list.insert(1);
		REQUIRE(testInvariants(list));
		list.insert(5);
		REQUIRE(testInvariants(list));
		list.insert(3);
		REQUIRE(testInvariants(list));
		list.insert(4);
		REQUIRE(testInvariants(list));
		list.insert(4);
		REQUIRE(testInvariants(list));
		list.insert(4);
		REQUIRE(testInvariants(list));
		list.insert(2);
		REQUIRE(testInvariants(list));
		list.insert(7);
		REQUIRE(testInvariants(list));
		list.insert(6);
		REQUIRE(testInvariants(list));

		integer correctSet[] = 
		{
			1, 2, 3, 4, 4, 4, 5, 6, 7
		};

		{
			// A skip-list without values has iterators
			// which derereference to the key.
			REQUIRE(!list.empty());
			REQUIRE(list.size() == 9);
			REQUIRE(boost::equal(list, correctSet));
		}
	}

	void testRandom(integer maxHeight)
	{
		List list;
		list.setMaxHeight(maxHeight);

		std::list<integer> dataSet;
		integer listSizeSet[] = 
			{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100};

		for (integer listSize : listSizeSet)
		{
			list.clear();
			REQUIRE(testInvariants(list));

			dataSet.clear();

			for (integer i = 0;i < 2 * listSize;++i)
			{
				integer n = randomInteger(0);
				dataSet.push_back(n);

				list.insert(n);
				REQUIRE(testInvariants(list));

				if (list.size() > listSize)
				{
					list.erase(dataSet.front());
					REQUIRE(testInvariants(list));
					dataSet.pop_front();
				}
			}
		}
	}

}

TEST_CASE("various (SkipList)")
{
	for (integer i = 0; i < 10; ++i)
	{
		testSimple(i);
		testValueSet(i);
		testValueMap(i);
		testCount(i);
		testRandom(i);
	}
}

