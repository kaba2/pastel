// Description: Testing for skip lists.
// DocumentationOf: skiplist.h

#include "test_pastelsys.h"

#include "pastel/sys/skiplist.h"
#include "pastel/sys/random_integer.h"

#include <boost/range/adaptor/reversed.hpp> 

#include <iostream>
#include <list>

using namespace Pastel;

namespace
{

	class Test
		: public TestSuite
	{
	public:
		Test()
			: TestSuite(&testReport())
		{
		}

		virtual void run()
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

		typedef SkipList_Set<integer> List;
		typedef List::Iterator Iterator;
		typedef List::ConstIterator ConstIterator;

		void testSimple(integer maxHeight)
		{
			List list;
			list.setMaxHeight(maxHeight);
			TEST_ENSURE(validInvariants(list));

			ConstIterator listEnd = list.cend();

			{
				integer insertSet[] = 
				{
					1, 5, 3, 4, 4, 4, 8, 7, 6, 9, 2
				};

				for (integer i : insertSet)
				{
					list.insert(i);
					TEST_ENSURE(validInvariants(list));
				}
			}

			{
				integer correctSet[] =
				{
					1, 2, 3, 4, 4, 4, 5, 6, 7, 8, 9
				};

				TEST_ENSURE_OP(list.size(), ==, 11);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 9);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(8);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					1, 2, 3, 4, 4, 4, 5, 6, 7, 9
				};

				TEST_ENSURE_OP(list.size(), ==, 10);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 8);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(4);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					1, 2, 3, 5, 6, 7, 9
				};

				TEST_ENSURE_OP(list.size(), ==, 7);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 7);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(2);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					1, 3, 5, 6, 7, 9
				};

				TEST_ENSURE_OP(list.size(), ==, 6);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 6);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(1);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					3, 5, 6, 7, 9
				};

				TEST_ENSURE_OP(list.size(), ==, 5);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 5);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(9);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					3, 5, 6, 7
				};

				TEST_ENSURE_OP(list.size(), ==, 4);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 4);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(6);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					3, 5, 7
				};

				TEST_ENSURE_OP(list.size(), ==, 3);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 3);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(5);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					3, 7
				};

				TEST_ENSURE_OP(list.size(), ==, 2);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 2);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(7);
			{
				TEST_ENSURE(validInvariants(list));

				integer correctSet[] =
				{
					3
				};

				TEST_ENSURE_OP(list.size(), ==, 1);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 1);
				TEST_ENSURE(boost::equal(list, correctSet));
			}

			list.erase(3);
			{
				TEST_ENSURE(validInvariants(list));

				TEST_ENSURE_OP(list.size(), ==, 0);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 0);
				TEST_ENSURE(list.empty());
			}

			ConstIterator one = list.insert(1);
			TEST_ENSURE(validInvariants(list));
			list.insert(5);
			TEST_ENSURE(validInvariants(list));
			list.insert(3);
			TEST_ENSURE(validInvariants(list));
			list.insert(4);
			TEST_ENSURE(validInvariants(list));
			ConstIterator four = list.insert(4);
			TEST_ENSURE(validInvariants(list));
			list.insert(4);
			TEST_ENSURE(validInvariants(list));
			list.insert(8);
			TEST_ENSURE(validInvariants(list));
			list.insert(7);
			TEST_ENSURE(validInvariants(list));
			list.insert(6);
			TEST_ENSURE(validInvariants(list));
			list.insert(9);
			TEST_ENSURE(validInvariants(list));
			list.insert(2);
			TEST_ENSURE(validInvariants(list));

			{
				TEST_ENSURE_OP(four.equivalents(), ==, 3);
				TEST_ENSURE_OP(one.equivalents(), ==, 1);
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
				TEST_ENSURE_OP(boost::distance(list), ==, 11);
				TEST_ENSURE(boost::equal(list, correctSet));

				// Test iterator decrement.
				TEST_ENSURE(boost::equal(
					list | boost::adaptors::reversed, 
					correctSet | boost::adaptors::reversed));
			}

			// Test lowerBound(), upperBound(), and find().
			{
				TEST_ENSURE_OP(*list.lowerBound(0), ==, 1);
				TEST_ENSURE_OP(*list.lowerBound(1), ==, 1);
				TEST_ENSURE_OP(*list.lowerBound(2), ==, 2);
				TEST_ENSURE_OP(*list.lowerBound(4), ==, 4);
				TEST_ENSURE_OP(*list.lowerBound(8), ==, 8);
				TEST_ENSURE_OP(*list.lowerBound(9), ==, 9);
				TEST_ENSURE(list.lowerBound(10) == list.cend());

				TEST_ENSURE_OP(*list.lowerBound(0, four), ==, 1);
				TEST_ENSURE_OP(*list.lowerBound(1, four), ==, 1);
				TEST_ENSURE_OP(*list.lowerBound(2, four), ==, 2);
				TEST_ENSURE_OP(*list.lowerBound(4, four), ==, 4);
				TEST_ENSURE_OP(*list.lowerBound(8, four), ==, 8);
				TEST_ENSURE_OP(*list.lowerBound(9, four), ==, 9);
				TEST_ENSURE(list.lowerBound(10, four) == list.cend());

				TEST_ENSURE_OP(*list.upperBound(0), ==, 1);
				TEST_ENSURE_OP(*list.upperBound(1), ==, 2);
				TEST_ENSURE_OP(*list.upperBound(2), ==, 3);
				TEST_ENSURE_OP(*list.upperBound(4), ==, 5);
				TEST_ENSURE_OP(*list.upperBound(8), ==, 9);
				TEST_ENSURE(list.upperBound(9) == list.cend());

				TEST_ENSURE_OP(*list.upperBound(0, four), ==, 1);
				TEST_ENSURE_OP(*list.upperBound(1, four), ==, 2);
				TEST_ENSURE_OP(*list.upperBound(2, four), ==, 3);
				TEST_ENSURE_OP(*list.upperBound(4, four), ==, 5);
				TEST_ENSURE_OP(*list.upperBound(8, four), ==, 9);
				TEST_ENSURE(list.upperBound(9, four) == list.cend());

				TEST_ENSURE(list.find(0) == list.end());
				TEST_ENSURE(
					list.find(1) != list.end() &&
					*list.find(1) == 1);
				TEST_ENSURE(
					list.find(2) != list.end() &&
					*list.find(2) == 2);
				TEST_ENSURE(
					list.find(4) != list.end() &&
					*list.find(4) == 4);
				TEST_ENSURE(
					list.find(8) != list.end() &&
					*list.find(8) == 8);
				TEST_ENSURE(
					list.find(9) != list.end() &&
					*list.find(9) == 9);
				TEST_ENSURE(list.find(10) == list.end());
			}

			// Test copy-construction, and move-construction.
			{
				List copyList(list);
				TEST_ENSURE(validInvariants(copyList));
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(copyList.maxHeight(), == , list.maxHeight());
				TEST_ENSURE_OP(copyList.height(), == , list.height());
				TEST_ENSURE_OP(copyList.size(), == , list.size());
				TEST_ENSURE_OP(copyList.uniqueKeys(), == , list.uniqueKeys());

				ConstIterator copyEnd = copyList.cend();
				
				TEST_ENSURE_OP(list.size(), ==, 11);
				TEST_ENSURE(!list.empty());
				TEST_ENSURE(listEnd == list.cend());
				TEST_ENSURE(boost::equal(list, correctSet));
				
				TEST_ENSURE_OP(copyList.size(), ==, 11);
				TEST_ENSURE(!copyList.empty());
				TEST_ENSURE(listEnd != copyList.cend());
				TEST_ENSURE(boost::equal(copyList, correctSet));

				List moveList(std::move(copyList));
				TEST_ENSURE(validInvariants(moveList));
				TEST_ENSURE(validInvariants(copyList));

				TEST_ENSURE_OP(copyList.size(), ==, 0);
				TEST_ENSURE(copyList.empty());
				TEST_ENSURE(copyEnd == copyList.cend());
				
				TEST_ENSURE_OP(moveList.size(), ==, 11);
				TEST_ENSURE(!moveList.empty());
				TEST_ENSURE(copyEnd != moveList.cend());
				TEST_ENSURE(boost::equal(moveList, correctSet));
			}

			// Test copy-assign, move-assign, and swap.
			{
				List copyList;
				TEST_ENSURE(validInvariants(copyList));

				copyList = list;
				TEST_ENSURE(validInvariants(copyList));
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(copyList.maxHeight(), == , list.maxHeight());
				TEST_ENSURE_OP(copyList.height(), == , list.height());
				TEST_ENSURE_OP(copyList.size(), == , list.size());
				TEST_ENSURE_OP(copyList.uniqueKeys(), == , list.uniqueKeys());

				TEST_ENSURE_OP(list.size(), ==, 11);
				TEST_ENSURE(!list.empty());
				TEST_ENSURE(listEnd == list.cend());
				TEST_ENSURE(boost::equal(list, correctSet));
				
				TEST_ENSURE_OP(copyList.size(), ==, 11);
				TEST_ENSURE(!copyList.empty());
				TEST_ENSURE(listEnd != copyList.cend());
				TEST_ENSURE(boost::equal(copyList, correctSet));

				List moveList;
				TEST_ENSURE(validInvariants(moveList));

				moveList = std::move(list);
				TEST_ENSURE(validInvariants(moveList));
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(moveList.maxHeight(), == , maxHeight);
				TEST_ENSURE_OP(list.maxHeight(), == , maxHeight);

				TEST_ENSURE_OP(list.size(), ==, 0);
				TEST_ENSURE(list.empty());
				TEST_ENSURE(listEnd == list.cend());
				
				TEST_ENSURE_OP(moveList.size(), ==, 11);
				TEST_ENSURE(!moveList.empty());
				TEST_ENSURE(listEnd != moveList.cend());
				TEST_ENSURE(boost::equal(moveList, correctSet));

				moveList.swap(list);
				TEST_ENSURE(validInvariants(moveList));
				TEST_ENSURE(validInvariants(list));

				TEST_ENSURE_OP(list.size(), ==, 11);
				TEST_ENSURE(!list.empty());
				TEST_ENSURE(listEnd != list.cend());
				
				TEST_ENSURE_OP(moveList.size(), ==, 0);
				TEST_ENSURE(moveList.empty());
				TEST_ENSURE(listEnd == moveList.cend());
			}
		}

		void testCount(integer maxHeight)
		{
			List list;
			list.setMaxHeight(maxHeight);

			list.insert(1);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 0);
				TEST_ENSURE_OP(list.count(1), ==, 1);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(2);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 0);
				TEST_ENSURE_OP(list.count(1), ==, 1);
				TEST_ENSURE_OP(list.count(2), ==, 1);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(0);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 1);
				TEST_ENSURE_OP(list.count(1), ==, 1);
				TEST_ENSURE_OP(list.count(2), ==, 1);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(1);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 1);
				TEST_ENSURE_OP(list.count(1), ==, 2);
				TEST_ENSURE_OP(list.count(2), ==, 1);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(0);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 2);
				TEST_ENSURE_OP(list.count(1), ==, 2);
				TEST_ENSURE_OP(list.count(2), ==, 1);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(2);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 2);
				TEST_ENSURE_OP(list.count(1), ==, 2);
				TEST_ENSURE_OP(list.count(2), ==, 2);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(1);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 2);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 2);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.insert(0);
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 3);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 2);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(2));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), == , 0);
				TEST_ENSURE_OP(list.count(0), ==, 3);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 1);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(0));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 2);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 1);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(2));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 2);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(0));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 1);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(0));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 0);
				TEST_ENSURE_OP(list.count(1), ==, 3);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(1));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 0);
				TEST_ENSURE_OP(list.count(1), ==, 2);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(1));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 0);
				TEST_ENSURE_OP(list.count(1), ==, 1);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}

			list.erase(list.lowerBound(1));
			{
				TEST_ENSURE(validInvariants(list));
				TEST_ENSURE_OP(list.count(-1), ==, 0);
				TEST_ENSURE_OP(list.count(0), ==, 0);
				TEST_ENSURE_OP(list.count(1), ==, 0);
				TEST_ENSURE_OP(list.count(2), ==, 0);
				TEST_ENSURE_OP(list.count(3), ==, 0);
			}
		}

		void testValueMap(integer maxHeight)
		{
			SkipList_Map<integer, integer> list;
			list.setMaxHeight(maxHeight);

			TEST_ENSURE(validInvariants(list));

			list.insert(1, 1);
			TEST_ENSURE(validInvariants(list));
			list.insert(5, 2);
			TEST_ENSURE(validInvariants(list));
			list.insert(3, 3);
			TEST_ENSURE(validInvariants(list));
			list.insert(4, 4);
			TEST_ENSURE(validInvariants(list));
			list.insert(4, 5);
			TEST_ENSURE(validInvariants(list));
			list.insert(4, 6);
			TEST_ENSURE(validInvariants(list));
			list.insert(2, 7);
			TEST_ENSURE(validInvariants(list));
			list.insert(7, 8);
			TEST_ENSURE(validInvariants(list));
			list.insert(6, 9);
			TEST_ENSURE(validInvariants(list));

			integer correctSet[] = 
			{
				1, 7, 3, 4, 5, 6, 2, 9, 8
			};

			{
				// A skip-list with values has iterators
				// which derereference to the value.
				TEST_ENSURE(!list.empty());
				TEST_ENSURE_OP(list.size(), ==, 9);
				TEST_ENSURE(boost::equal(list, correctSet));
			}
		}

		void testValueSet(integer maxHeight)
		{
			SkipList_Set<integer> list;
			list.setMaxHeight(maxHeight);

			TEST_ENSURE(validInvariants(list));

			list.insert(1);
			TEST_ENSURE(validInvariants(list));
			list.insert(5);
			TEST_ENSURE(validInvariants(list));
			list.insert(3);
			TEST_ENSURE(validInvariants(list));
			list.insert(4);
			TEST_ENSURE(validInvariants(list));
			list.insert(4);
			TEST_ENSURE(validInvariants(list));
			list.insert(4);
			TEST_ENSURE(validInvariants(list));
			list.insert(2);
			TEST_ENSURE(validInvariants(list));
			list.insert(7);
			TEST_ENSURE(validInvariants(list));
			list.insert(6);
			TEST_ENSURE(validInvariants(list));

			integer correctSet[] = 
			{
				1, 2, 3, 4, 4, 4, 5, 6, 7
			};

			{
				// A skip-list without values has iterators
				// which derereference to the key.
				TEST_ENSURE(!list.empty());
				TEST_ENSURE_OP(list.size(), ==, 9);
				TEST_ENSURE(boost::equal(list, correctSet));
			}
		}

		void testRandom(integer maxHeight)
		{
			List list;
			list.setMaxHeight(maxHeight);

			std::list<integer> dataSet;
			const integer listSizeSet[] = 
				{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100};

			for (integer listSize : listSizeSet)
			{
				list.clear();
				TEST_ENSURE(validInvariants(list));

				dataSet.clear();
				for (integer i = 0;i < 2 * listSize;++i)
				{
					const integer n = randomInteger();
					dataSet.push_back(n);

					list.insert(n);
					TEST_ENSURE(validInvariants(list));

					if (list.size() > listSize)
					{
						list.erase(dataSet.front());
						TEST_ENSURE(validInvariants(list));
						dataSet.pop_front();
					}
				}
			}
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("SkipList", test);
	}

	CallFunction run(addTest);

}
