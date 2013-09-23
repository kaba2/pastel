// Description: Testing for skip lists.
// DocumentationOf: skiplist.h

#include "test_pastelsys.h"

#include "pastel/sys/skiplist.h"

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
			testSimple();
			testValue();
			testRandom();
		}

		typedef SkipList_Set<integer> List;
		typedef List::Iterator Iterator;
		typedef List::ConstIterator ConstIterator;

		void testSimple()
		{
			List list;
			ConstIterator listEnd = list.cend();

			// Test default-construction.
			{
				TEST_ENSURE(list.empty());
				TEST_ENSURE_OP(list.size(), ==, 0);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 0);
				TEST_ENSURE(list.cbegin() == list.cend());
			}

			list.insert(1);
			list.insert(5);
			list.insert(3);
			list.insert(4);
			list.insert(4);
			list.insert(4);
			list.insert(8);
			list.insert(7);
			list.insert(6);
			list.insert(9);
			list.insert(2);

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
				TEST_ENSURE_OP(list.size(), ==, 0);
				TEST_ENSURE_OP(list.uniqueKeys(), ==, 0);
				TEST_ENSURE(list.empty());
			}

			list.insert(1);
			list.insert(5);
			list.insert(3);
			list.insert(4);
			ConstIterator four = list.insert(4);
			list.insert(4);
			list.insert(8);
			list.insert(7);
			list.insert(6);
			list.insert(9);
			list.insert(2);

			integer correctSet[] =
			{
				1, 2, 3, 4, 4, 4, 5, 6, 7, 8, 9
			};

			// Test insert.
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

			// Test lower_bound, upper_bound, and find.
			{
				TEST_ENSURE_OP(*list.lower_bound(0), ==, 1);
				TEST_ENSURE_OP(*list.lower_bound(1), ==, 1);
				TEST_ENSURE_OP(*list.lower_bound(2), ==, 2);
				TEST_ENSURE_OP(*list.lower_bound(4), ==, 4);
				TEST_ENSURE_OP(*list.lower_bound(8), ==, 8);
				TEST_ENSURE_OP(*list.lower_bound(9), ==, 9);
				TEST_ENSURE(list.lower_bound(10) == list.cend());

				TEST_ENSURE_OP(*list.lower_bound(0, four), ==, 1);
				TEST_ENSURE_OP(*list.lower_bound(1, four), ==, 1);
				TEST_ENSURE_OP(*list.lower_bound(2, four), ==, 2);
				TEST_ENSURE_OP(*list.lower_bound(4, four), ==, 4);
				TEST_ENSURE_OP(*list.lower_bound(8, four), ==, 8);
				TEST_ENSURE_OP(*list.lower_bound(9, four), ==, 9);
				TEST_ENSURE(list.lower_bound(10, four) == list.cend());

				TEST_ENSURE_OP(*list.upper_bound(0), ==, 1);
				TEST_ENSURE_OP(*list.upper_bound(1), ==, 2);
				TEST_ENSURE_OP(*list.upper_bound(2), ==, 3);
				TEST_ENSURE_OP(*list.upper_bound(4), ==, 5);
				TEST_ENSURE_OP(*list.upper_bound(8), ==, 9);
				TEST_ENSURE(list.upper_bound(9) == list.cend());

				TEST_ENSURE_OP(*list.upper_bound(0, four), ==, 1);
				TEST_ENSURE_OP(*list.upper_bound(1, four), ==, 2);
				TEST_ENSURE_OP(*list.upper_bound(2, four), ==, 3);
				TEST_ENSURE_OP(*list.upper_bound(4, four), ==, 5);
				TEST_ENSURE_OP(*list.upper_bound(8, four), ==, 9);
				TEST_ENSURE(list.upper_bound(9, four) == list.cend());

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
				copyList = list;

				TEST_ENSURE_OP(list.size(), ==, 11);
				TEST_ENSURE(!list.empty());
				TEST_ENSURE(listEnd == list.cend());
				TEST_ENSURE(boost::equal(list, correctSet));
				
				TEST_ENSURE_OP(copyList.size(), ==, 11);
				TEST_ENSURE(!copyList.empty());
				TEST_ENSURE(listEnd != copyList.cend());
				TEST_ENSURE(boost::equal(copyList, correctSet));

				List moveList;
				moveList = std::move(list);

				TEST_ENSURE_OP(list.size(), ==, 0);
				TEST_ENSURE(list.empty());
				TEST_ENSURE(listEnd == list.cend());
				
				TEST_ENSURE_OP(moveList.size(), ==, 11);
				TEST_ENSURE(!moveList.empty());
				TEST_ENSURE(listEnd != moveList.cend());
				TEST_ENSURE(boost::equal(moveList, correctSet));

				moveList.swap(list);

				TEST_ENSURE_OP(list.size(), ==, 11);
				TEST_ENSURE(!list.empty());
				TEST_ENSURE(listEnd != list.cend());
				
				TEST_ENSURE_OP(moveList.size(), ==, 0);
				TEST_ENSURE(moveList.empty());
				TEST_ENSURE(listEnd == moveList.cend());
			}
		}

		void testValue()
		{
			SkipList_Map<integer, integer> list;

			{
				TEST_ENSURE(list.empty());
				TEST_ENSURE_OP(list.size(), ==, 0);
				TEST_ENSURE(list.cbegin() == list.cend());
			}

			list.insert(1, 1);
			list.insert(5, 2);
			list.insert(3, 3);
			list.insert(4, 4);
			list.insert(4, 5);
			list.insert(4, 6);
			list.insert(2, 7);
			list.insert(7, 8);
			list.insert(6, 9);

			integer correctSet[] = 
			{
				1, 7, 3, 4, 5, 6, 2, 9, 8
			};

			{
				TEST_ENSURE(!list.empty());
				TEST_ENSURE_OP(list.size(), ==, 9);
				TEST_ENSURE(boost::equal(list, correctSet));
			}
		}

		void testRandom()
		{
			List list;
			std::list<integer> dataSet;
			const integer listSizeSet[] = 
				{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 100, 1000};

			for (integer listSize : listSizeSet)
			{
				list.clear();
				dataSet.clear();
				for (integer i = 0;i < listSize;++i)
				{
					const integer n = randomInteger();
					dataSet.push_back(n);

					list.insert(n);

					if (list.size() > listSize)
					{
						list.erase(dataSet.front());
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
