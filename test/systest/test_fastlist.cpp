// Description: Testing for FastList
// DocumentationOf: fastlist.h

#include "test_pastelsys.h"

#include "pastel/sys/fastlist.h"
#include "pastel/sys/log.h"
#include "pastel/sys/pool_allocator.h"

#include <iostream>

using namespace Pastel;
using namespace std;

namespace
{

	typedef FastList<integer, PoolAllocator> Container;
	typedef Container::iterator Iterator;
	typedef Container::const_iterator ConstIterator;

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
			testSplice();
			testUnique();
			testRemoveIf();
			testResize();
			testDisambiguation();
			testRemove();
			testEqual();
		}

		void testSimple()
		{
			// Purpose:
			// Run functions with an empty container,
			// nothing should happen.

			{
				Container a;
				a == a;
				a != a;
				a < a;
				a >= a;
				a <= a;
				a > a;
				a = a;

				Container b(a);
				a = b;
				a.clear();
				a.begin();
				a.end();
				a.rbegin();
				a.rend();
				b.swap(a);

				TEST_ENSURE_OP(a.size(), ==, 0);
				TEST_ENSURE(a.empty());
				a.reverse();
				a.sort();
				a.merge(b);
				a.unique();
			}
		}

		void testDisambiguation()
		{
			// Purpose:
			// See if the constructor can
			// correctly disambiguate between
			// the iterator construction
			// and the resizing construction.

			Container c(3, 3);
			{
				integer correctSet[] = {3, 3, 3};
				TEST_ENSURE(boost::equal(c, range(correctSet)));
			}

			// Disambiguation for insert.
			// Should add 3 pieces of 4.
			c.insert(c.begin(), 3, 4);
			{
				integer correctSet[] = {4, 4, 4, 3, 3, 3};
				TEST_ENSURE(boost::equal(c, range(correctSet)));
			}

			// Disambiguation for assign.
			// Should assign 3 pieces of 2.
			c.assign(3, 2);
			{
				integer correctSet[] = {2, 2, 2};
				TEST_ENSURE(boost::equal(c, range(correctSet)));
			}
		}

		bool checkIterators(const Container& left,
			const Container& right)
		{
			ConstIterator iter(left.begin());
			ConstIterator iterEnd(left.end());
			while (iter != iterEnd)
			{
				ConstIterator iterRight(right.begin());
				ConstIterator rightEnd(right.end());
				while (iterRight != rightEnd)
				{
					if (iterRight == iter)
					{
						return false;
					}
					++iterRight;
				}
				++iter;
			}
			
			return true;
		}

		void testSplice()
		{
			Container c;
			Container b(10, 5, c.get_allocator());
			b.push_front(2);
			b.push_front(6);
			b.push_back(-4);
			b.push_back(5);
			{
				integer correctSet[] = 
				{
					6, 2, 
					5, 5, 5, 5, 5, 
					5, 5, 5, 5, 5,
					-4, 5
				};
				TEST_ENSURE_OP(b.size(), ==, 14);
				TEST_ENSURE(boost::equal(b, correctSet));
			}

			b.splice(b.begin(), b, --b.end(), b.end());
			{
				integer correctSet[] = 
				{
					5, 
					6, 2, 
					5, 5, 5, 5, 5, 
					5, 5, 5, 5, 5,
					-4
				};
				TEST_ENSURE_OP(b.size(), ==, 14);
				TEST_ENSURE(boost::equal(b, correctSet));
			}

			b.splice(b.begin(), b, b.begin(), b.end());
			{
				integer correctSet[] = 
				{
					5, 
					6, 2, 
					5, 5, 5, 5, 5, 
					5, 5, 5, 5, 5,
					-4
				};
				TEST_ENSURE_OP(b.size(), ==, 14);
				TEST_ENSURE(boost::equal(b, correctSet));
			}

			c.splice(c.begin(), b, b.begin(), b.end());
			{
				integer correctSet[] = 
				{
					5, 
					6, 2, 
					5, 5, 5, 5, 5, 
					5, 5, 5, 5, 5,
					-4
				};
				TEST_ENSURE_OP(c.size(), ==, 14);
				TEST_ENSURE(boost::equal(c, correctSet));
				TEST_ENSURE(b.empty());
				TEST_ENSURE_OP(b.size(), ==, 0);
				TEST_ENSURE(checkIterators(c, b));
			}

			b.splice(b.begin(), c, --c.end(), c.end());
			{
				integer bCorrectSet[] = 
				{
					-4
				};
				integer cCorrectSet[] =
				{
					5, 
					6, 2, 
					5, 5, 5, 5, 5, 
					5, 5, 5, 5, 5
				};

				TEST_ENSURE_OP(b.size(), ==, 1);
				TEST_ENSURE_OP(c.size(), ==, 13);
				TEST_ENSURE(checkIterators(c, b));
			}
			
			b.splice(b.begin(), c, c.begin(), ++c.begin());
			{
				integer bCorrectSet[] = 
				{
					5, -4
				};
				integer cCorrectSet[] =
				{
					6, 2, 
					5, 5, 5, 5, 5, 
					5, 5, 5, 5, 5
				};

				TEST_ENSURE_OP(b.size(), ==, 2);
				TEST_ENSURE_OP(c.size(), ==, 12);
				TEST_ENSURE(checkIterators(c, b));
			}

			Iterator iter(--c.end());
			b.splice(b.end(), c, c.begin(), c.end());
			*iter = -18;
			{
				TEST_ENSURE_OP(b.size(), ==, 14);
				TEST_ENSURE(b.back() == -18);
			}
		}

		void testUnique()
		{
			const integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

			Container a(std::begin(valueSet), std::end(valueSet));
			{
				TEST_ENSURE(boost::equal(a, range(valueSet)));
			}

			a.unique();
			{
				integer correctSet[] = 
				{
					1, 2, 3, 4, 5, 4
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
			}
		}

		void testRemoveIf()
		{
			const integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

			Container a(std::begin(valueSet), std::end(valueSet));
			{
				TEST_ENSURE(boost::equal(a, range(valueSet)));
			}

			a.remove_if(std::bind2nd(std::less<integer>(), 4));
			{
				integer correctSet[] = 
				{
					4, 4, 4, 5, 4, 4, 4
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
			}
		}

		void testResize()
		{
			Container a;
			a.resize(10, 4);
			{
				integer correctSet[] = 
				{
					4, 4, 4, 4, 4,
					4, 4, 4, 4, 4
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
				TEST_ENSURE_OP(a.size(), ==, 10);
			}

			a.resize(0);
			{
				TEST_ENSURE(a.empty());
				TEST_ENSURE_OP(a.size(), ==, 0);
			}

			a.resize(4, 3);
			{
				integer correctSet[] = 
				{
					3, 3, 3, 3
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
				TEST_ENSURE_OP(a.size(), ==, 4);
			}

			a.resize(4, 4);
			{
				integer correctSet[] = 
				{
					3, 3, 3, 3
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
				TEST_ENSURE_OP(a.size(), ==, 4);
			}

			a.resize(5, 5);
			{
				integer correctSet[] = 
				{
					3, 3, 3, 3, 5
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
				TEST_ENSURE_OP(a.size(), ==, 5);
			}
		}

		void testRemove()
		{
			integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

			Container a(std::begin(valueSet), std::end(valueSet));
			{
				TEST_ENSURE(boost::equal(a, range(valueSet)));
			}

			a.remove(4);
			{
				integer correctSet[] = 
				{
					1, 2, 3, 3, 3, 5
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
			}
		}

		void testEqual()
		{
			const integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

			Container a;

			TEST_ENSURE(a == a);

			a.assign(std::begin(valueSet), std::end(valueSet));

			TEST_ENSURE(a == a);

			Container b(a);
			TEST_ENSURE(b == a);

			b.pop_back();
			TEST_ENSURE(b != a);

			a.pop_back();
			TEST_ENSURE(b == a);

			a.front() = -5;
			TEST_ENSURE(b != a);

			a.swap(b);
			TEST_ENSURE(b != a);

			a = b;
			TEST_ENSURE(b == a);

			a.swap(b);
			TEST_ENSURE(b == a);
		}
	};

	void test()
	{
		Test test;
		test.run();
	}

	void addTest()
	{
		testRunner().add("FastList", test);
	}

	CallFunction run(addTest);

}
