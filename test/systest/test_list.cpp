// Description: Testing for List
// DocumentationOf: list.h

#include "test_pastelsys.h"

#include "pastel/sys/list.h"
#include "pastel/sys/range.h"
#include "pastel/sys/all_indicator.h"

#include <iostream>

using namespace Pastel;
using namespace std;

namespace
{

	typedef List_Set<integer, integer> Set;
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
			testIterators();
			testCopyAndMove();
			testClear();
			testSwap();
			testComparison();
			testInsertAndErase();
			testSort();
			testUnique();
			testRemoveIf();
			testMerge();
			testReverse();

			testSplice();
			testEqual();
		}

		template <typename Type>
		bool equal(const Set& left, 
			std::initializer_list<Type> right) const
		{
			return boost::equal(left, right);
		}

		void testIterators()
		{
			Set a({1, 2, 3});
			{
				TEST_ENSURE_OP(*a.begin(), ==, 1);
				TEST_ENSURE_OP(*a.last(), ==, 3);
				TEST_ENSURE(a.last() == std::prev(a.end()));
				TEST_ENSURE(boost::equal(a.crange(), std::initializer_list<int>({1, 2, 3})));
			}
			{
				Iterator i = a.begin();
				ConstIterator j = i;
				TEST_ENSURE(i == j);
				TEST_ENSURE(i <= j);
				TEST_ENSURE(i >= j);
				i < j;
				i > j;

				TEST_ENSURE(j == i);
				TEST_ENSURE(j <= i);
				TEST_ENSURE(j >= i);
				j < i;
				j > i;
			}
			{
				TEST_ENSURE(a.cend().isEnd());
			}
			{
				a.end().endData() = 4;
				TEST_ENSURE_OP(a.end().endData(), == , 4);
			}
			{
				Iterator iter;
				ENSURE(iter.empty());

				iter = a.end();
				ENSURE(iter.isEnd());
				
				iter.clear();
				ENSURE(iter.empty());
			}
		}

		void testSwap()
		{
			{
				Set a({1, 2, 3, 4});
				TEST_ENSURE(testInvariants(a));

				Set b;
				TEST_ENSURE(testInvariants(b));

				a.swap(b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(b, { 1, 2, 3, 4 }));
				TEST_ENSURE(a.empty());

				swap(a, b);
			}
		}

		void testClear()
		{
			{
				Set a({1, 2, 3, 4});
				TEST_ENSURE(testInvariants(a));
				Iterator aEnd = a.end();

				a.clear();
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(a.end() == aEnd);
			}
		}

		void testCopyAndMove()
		{
			{
				Set a({1, 2, 3, 4});
				TEST_ENSURE(testInvariants(a));

				Set b(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(boost::equal(a, b));

				Set c;
				TEST_ENSURE(testInvariants(c));
				Iterator cEnd = c.end();

				c = b;
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(testInvariants(c));
				TEST_ENSURE(boost::equal(b, c));
				TEST_ENSURE(c.end() == cEnd);

				Set d = std::move(c);
				TEST_ENSURE(testInvariants(c));
				TEST_ENSURE(testInvariants(d));
				TEST_ENSURE(boost::equal(b, d));
				TEST_ENSURE(c.empty());
				TEST_ENSURE(c.end() == cEnd);

				c = std::move(d);
				TEST_ENSURE(testInvariants(c));
				TEST_ENSURE(testInvariants(d));
				TEST_ENSURE(boost::equal(b, c));
				TEST_ENSURE(d.empty());
				TEST_ENSURE(c.end() == cEnd);

				Set e;
				TEST_ENSURE(testInvariants(e));

				e = { 1, 2, 3, 4 };
				TEST_ENSURE(testInvariants(e));
				TEST_ENSURE(equal(e, { 1, 2, 3, 4 }));
			}
		}

		void testComparison()
		{
			{
				Set a;
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(a == a);
				TEST_ENSURE(!(a != a));
			}
			{
				Set a({1, 6, 3, 4, 5});
				TEST_ENSURE(testInvariants(a));

				Set b({1, 6, 3, 4, 5});
				TEST_ENSURE(testInvariants(b));

				TEST_ENSURE(a == b);
				TEST_ENSURE(!(a != b));
				TEST_ENSURE(!(a < b));
				TEST_ENSURE(!(a > b));
				TEST_ENSURE(a <= b);
				TEST_ENSURE(a >= b);
			}
			{
				Set a({1, 5, 3, 4, 5});
				TEST_ENSURE(testInvariants(a));

				Set b({1, 5, 3, 6, 7});
				TEST_ENSURE(testInvariants(b));

				TEST_ENSURE(a != b);
				TEST_ENSURE(a <= b);
				TEST_ENSURE(a < b);
				TEST_ENSURE(!(a >= b));
				TEST_ENSURE(!(a > b));
			}
			{
				Set a({1, 6, 3, 4});
				TEST_ENSURE(testInvariants(a));

				Set b({1, 6, 3, 4, 5});
				TEST_ENSURE(testInvariants(b));

				TEST_ENSURE(a != b);
				TEST_ENSURE(!(a == b));
				TEST_ENSURE(a < b);
				TEST_ENSURE(!(a > b));
				TEST_ENSURE(!(a >= b));
				TEST_ENSURE(a <= b);
			}
		}

		void testInsertAndErase()
		{
			Set a;
			Iterator four;

			{
				Iterator iter = a.insertBack(1);
				TEST_ENSURE_OP(*iter, ==, 1);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1 }));
			}

			{
				four = a.insertBack(4);
				TEST_ENSURE_OP(*four, ==, 4);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1, 4 }));
			}

			{
				Iterator iter = a.insertBack(8);
				TEST_ENSURE_OP(*iter, ==, 8);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1, 4, 8 }));
			}

			{
				Iterator iter = a.insertFront(7);
				TEST_ENSURE_OP(*iter, ==, 7);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {7, 1, 4, 8 }));
				TEST_ENSURE_OP(a.size(), ==, 4);
			}

			{
				a.insert(four, 9);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {7, 1, 9, 4, 8 }));
			}

			{
				a.insert(four, 10);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {7, 1, 9, 10, 4, 8 }));
			}

			{
				a.insert(a.end(), 11);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {7, 1, 9, 10, 4, 8, 11 }));
			}

			{
				a.eraseFront();
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {1, 9, 10, 4, 8, 11 }));
			}
			{
				a.eraseBack();
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {1, 9, 10, 4, 8 }));
			}
			{
				a.erase(four);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {1, 9, 10, 8 }));
			}
			{
				a.eraseFront();
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {9, 10, 8 }));
			}
			{
				a.eraseFront();
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {10, 8 }));
			}
			{
				a.eraseBack();
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {10}));
			}
			{
				a.eraseBack();
				TEST_ENSURE(testInvariants(a));
			}
		}

		void testSort()
		{
			{
				Set a;
				TEST_ENSURE(testInvariants(a));

				sort(a);
				TEST_ENSURE(testInvariants(a));
			}
			{
				Set a({0});
				TEST_ENSURE(testInvariants(a));

				sort(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 0 }));
			}
			{
				Set a({0, 1, 2, 3});
				TEST_ENSURE(testInvariants(a));

				sort(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 0, 1, 2, 3 }));
			}
			{
				Set a({3, 2, 1, 0});
				TEST_ENSURE(testInvariants(a));

				sort(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 0, 1, 2, 3 }));
			}
			{
				Set a({1, 5, 3, 2, 4, 0});
				TEST_ENSURE(testInvariants(a));

				sort(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 0, 1, 2, 3, 4, 5 }));
			}
		}

		void testUnique()
		{
			{
				Set a;
				TEST_ENSURE(testInvariants(a));

				unique(a);
				TEST_ENSURE(testInvariants(a));
			}

			{
				Set a({1});
				TEST_ENSURE(testInvariants(a));

				unique(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1 }));
			}

			{
				Set a({1, 1, 1});
				TEST_ENSURE(testInvariants(a));

				unique(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1 }));
			}

			{
				Set a({1, 1, 2, 2, 3, 4, 4, 5});
				TEST_ENSURE(testInvariants(a));

				unique(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1, 2, 3, 4, 5 }));
			}

			{
				Set a({1, 1, 5, 5, 4, 4, 3, 3, 3, 3});
				TEST_ENSURE(testInvariants(a));

				unique(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1, 5, 4, 3 }));
			}
		}

		void testRemoveIf()
		{
			using namespace std::placeholders;

			{
				Set a;
				TEST_ENSURE(testInvariants(a));

				removeIf(a, All_Indicator());
				TEST_ENSURE(testInvariants(a));
			}
			{
				Set a({1, 2, 5, 3, 4, 5, 6, 5});
				TEST_ENSURE(testInvariants(a));

				removeIf(a, std::bind(EqualTo(), _1, 5));
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {1, 2, 3, 4, 6}));
			}	

			{
				Set a({1, 2, 5, 3, 4, 5, 6, 5});
				TEST_ENSURE(testInvariants(a));

				removeIf(a, std::bind(LessThan(), _1, 4));
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {5, 4, 5, 6, 5}));
			}	

			{
				Set a({1, 2, 5, 3, 4, 5, 6, 5});
				TEST_ENSURE(testInvariants(a));

				removeIf(a, std::bind(GreaterThan(), _1, 6));
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, {1, 2, 5, 3, 4, 5, 6, 5}));
			}	
		}

		void testMerge()
		{
			{
				Set a;
				TEST_ENSURE(testInvariants(a));

				Set b;
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
			}
			{
				Set a({ 1 });
				TEST_ENSURE(testInvariants(a));

				Set b;
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(a, { 1 }));
			}		
			{
				Set a({ 1 });
				TEST_ENSURE(testInvariants(a));

				Set b({ 2 });
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(a, { 1, 2 }));
			}		
			{
				Set a({ 1, 3 });
				TEST_ENSURE(testInvariants(a));

				Set b({ 2 });
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(a, { 1, 2, 3 }));
			}		
			{
				Set a({ 2 });
				TEST_ENSURE(testInvariants(a));

				Set b({ 1, 3 });
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(a, { 1, 2, 3 }));
			}		
			{
				Set a({ 1, 4, 6, 7, 8, 9 });
				TEST_ENSURE(testInvariants(a));

				Set b({ 1, 1, 3, 5, 6, 8, 10 });
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(a, 
					{ 1, 1, 1, 3, 4, 5, 6, 6, 7, 8, 8, 9, 10 }));
			}		

			{
				Set a({ 1, 2, 3, 4 });
				TEST_ENSURE(testInvariants(a));

				Set b({ 5, 6 });
				TEST_ENSURE(testInvariants(b));

				merge(a, b);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(testInvariants(b));
				TEST_ENSURE(equal(a, { 1, 2, 3, 4, 5, 6 }));
			}		
		}

		void testReverse()
		{
			{
				Set a;
				TEST_ENSURE(testInvariants(a));

				reverse(a);
				TEST_ENSURE(testInvariants(a));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
			}
			{
				Set a({ 1 });
				TEST_ENSURE(testInvariants(a));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1 }));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1 }));
			}		
			{
				Set a({ 1, 2 });
				TEST_ENSURE(testInvariants(a));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 2, 1 }));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1, 2 }));
			}
			{
				Set a({ 1, 2, 3, 4, 5 });
				TEST_ENSURE(testInvariants(a));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 5, 4, 3, 2, 1 }));

				reverse(a);
				TEST_ENSURE(testInvariants(a));
				TEST_ENSURE(equal(a, { 1, 2, 3, 4, 5 }));
			}
		}

		void testSplice()
		{
			Set c;
			TEST_ENSURE(testInvariants(c));
			
			Set b = { 6, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, -4, 5};
			TEST_ENSURE(testInvariants(b));

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
			TEST_ENSURE(testInvariants(b));
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
			TEST_ENSURE(testInvariants(b));
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
			TEST_ENSURE(testInvariants(b));
			TEST_ENSURE(testInvariants(c));
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
			}

			b.splice(b.begin(), c, c.last());
			TEST_ENSURE(testInvariants(b));
			TEST_ENSURE(testInvariants(c));
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
			}
			
			b.splice(b.begin(), c, c.begin());
			TEST_ENSURE(testInvariants(b));
			TEST_ENSURE(testInvariants(c));
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
			}

			Iterator iter(--c.end());
			b.splice(b.end(), c);
			*iter = -18;
			TEST_ENSURE(testInvariants(b));
			TEST_ENSURE(testInvariants(c));
			{
				TEST_ENSURE_OP(b.size(), ==, 14);
				TEST_ENSURE(b.back() == -18);
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
