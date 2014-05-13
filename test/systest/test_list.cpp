// Description: Testing for List
// DocumentationOf: list.h

#include "test_pastelsys.h"

#include "pastel/sys/list.h"
#include "pastel/sys/log.h"
#include "pastel/sys/pool_allocator.h"

#include <iostream>

using namespace Pastel;
using namespace std;

namespace
{

	typedef List<List_Settings<integer>> Set;
	typedef Set::Iterator Iterator;
	typedef Set::ConstIterator ConstIterator;

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
			testEqual();
		}

		void testSimple()
		{
			{
				Set a;
				a == a;
				a != a;
				a < a;
				a >= a;
				a <= a;
				a > a;
				a = a;

				Set b(a);
				a = b;
				a.clear();
				a.begin();
				a.end();
				b.swap(a);

				TEST_ENSURE_OP(a.size(), ==, 0);
				TEST_ENSURE(a.empty());
				reverse(a);
				sort(a);
				merge(a, b);
				unique(a);
			}
		}

		bool checkIterators(const Set& left,
			const Set& right)
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
			Set c;
			Set b = { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
			b.insertFront(2);
			b.insertFront(6);
			b.insertBack(-4);
			b.insertBack(5);
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

			b.splice(b.begin(), b, b.last());
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

			b.splice(b.begin(), b);
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

			c.splice(c.begin(), b);
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

			b.splice(b.begin(), c, c.last());
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
			
			b.splice(b.begin(), c, c.begin());
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
			b.splice(b.end(), c);
			*iter = -18;
			{
				TEST_ENSURE_OP(b.size(), ==, 14);
				TEST_ENSURE(b.back() == -18);
			}
		}

		void testUnique()
		{
			const integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

			Set a;
			for (integer value : valueSet)
			{
				a.insertBack(value);
			}
			{
				TEST_ENSURE(boost::equal(a, range(valueSet)));
			}

			unique(a);
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

			Set a;
			for (integer value : valueSet)
			{
				a.insertBack(value);
			}
			{
				TEST_ENSURE(boost::equal(a, range(valueSet)));
			}

			removeIf(a, std::bind2nd(std::less<integer>(), 4));
			{
				integer correctSet[] = 
				{
					4, 4, 4, 5, 4, 4, 4
				};
				TEST_ENSURE(boost::equal(a, range(correctSet)));
			}
		}

		void testEqual()
		{
			const integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

			Set a;

			TEST_ENSURE(a == a);

			for (integer value : valueSet)
			{
				a.insertBack(value);
			}

			TEST_ENSURE(a == a);

			Set b(a);
			TEST_ENSURE(b == a);

			b.eraseBack();
			TEST_ENSURE(b != a);

			a.eraseBack();
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
		testRunner().add("List", test);
	}

	CallFunction run(addTest);

}
