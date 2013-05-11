// Description: Testing for SkipList
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
			testEmpty();
			testSimple();
			testRandom();
		}

		typedef SkipList<integer> List;
		typedef List::Iterator Iterator;
		typedef List::ConstIterator ConstIterator;

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

			{
				List copyList(list);
				copyList.swap(list);
			}
			{
				List moveList(std::move(list));
			}
		}

		void testEmpty()
		{
			SkipList<void> list;
		}

		void testSimple()
		{
			List list;
			list.insert(1);
			list.insert(5);
			list.insert(3);
			list.insert(4);
			list.insert(8);
			list.insert(7);
			list.insert(6);
			list.insert(9);
			list.insert(2);

			{
				integer correctSet[] = 
				{
					1, 2, 3, 4, 5, 6, 7, 8, 9
				};

				// When I adapted Node* by the boost::iterator_adaptor,
				// it was adapted as a random-access iterator; then
				// the distance was given by the pointer difference,
				// which is a bug. This also affected boost::equal,
				// which compares the sizes of the ranges first.
				// Adding the bidirectional-traversal tag fixed the
				// bug. This line tests for that.
				TEST_ENSURE_OP(boost::distance(list), ==, 9);
				TEST_ENSURE(boost::equal(list, correctSet));

				TEST_ENSURE_OP(*list.lower_bound(0), ==, 1);
				TEST_ENSURE_OP(*list.lower_bound(1), ==, 1);
				TEST_ENSURE_OP(*list.lower_bound(2), ==, 2);
				TEST_ENSURE_OP(*list.lower_bound(8), ==, 8);
				TEST_ENSURE_OP(*list.lower_bound(9), ==, 9);
				TEST_ENSURE(list.lower_bound(10) == list.cend());

				TEST_ENSURE_OP(*list.upper_bound(0), ==, 1);
				TEST_ENSURE_OP(*list.upper_bound(1), ==, 2);
				TEST_ENSURE_OP(*list.upper_bound(2), ==, 3);
				TEST_ENSURE_OP(*list.upper_bound(8), ==, 9);
				TEST_ENSURE(list.upper_bound(9) == list.cend());
			}

			{
				integer correctSet[] = 
				{
					9, 8, 7, 6, 5, 4, 3, 2, 1
				};

				TEST_ENSURE(boost::equal(
					list | boost::adaptors::reversed, 
					correctSet));
			}

			TEST_ENSURE(list.find(3) != list.end());
			TEST_ENSURE(list.find(-1) == list.end());
			TEST_ENSURE(list.find(10) == list.end());
			TEST_ENSURE(list.find(9) != list.end());
			TEST_ENSURE(list.find(1) != list.end());
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
