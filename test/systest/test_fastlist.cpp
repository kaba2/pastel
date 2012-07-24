// Description: Testing for FastList
// DocumentationOf: fastlist.h

#include "pastelsystest.h"

#include "pastel/sys/fastlist.h"
#include "pastel/sys/log.h"
#include "pastel/sys/pool_allocator.h"

#include <iostream>

using namespace Pastel;
using namespace std;

namespace
{

	class A
	{
	public:
		A()
			: value_(0)
		{
		}

		A(int value)
			: value_(value)
		{
		}

		int value() const
		{
			return value_;
		}

		bool operator<(const A& that) const
		{
			return value_ < that.value_;
		}

		bool operator==(const A& that) const
		{
			return value_ == that.value_;
		}

		bool operator!=(const A& that) const
		{
			return !(*this == that);
		}

		bool operator==(int value) const
		{
			return value_ == value;
		}

		bool operator!=(int value) const
		{
			return !(*this == value);
		}

	private:
		int value_;
	};

	typedef FastList<A, PoolAllocator> Container;
	typedef Container::iterator Iterator;
	typedef Container::const_iterator ConstIterator;

	void print(const Container& that)
	{
		ConstIterator iter(that.begin());
		ConstIterator iterEnd(that.end());
		while (iter != iterEnd)
		{
			cout << iter->value() << ", ";
			++iter;
		}
		cout << "end." << endl;
	}

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

			print(c);

			// Disambiguation for insert.
			// Should add 3 pieces of 4.

			c.insert(c.begin(), 3, 4);

			print(c);

			// Disambiguation for assign.
			// Should assign 3 pieces of 2.

			c.assign(3, 2);

			print(c);
		}

		void checkIterators(const Container& left,
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
					ENSURE(iterRight != iter);
					++iterRight;
				}
				++iter;
			}
		}

		void testSplice()
		{
			Container c;
			Container b(10, 5, c.get_allocator());
			b.push_front(2);
			b.push_front(6);
			b.push_back(-4);
			b.push_back(5);

			REPORT1(b.size() != 14, b.size());

			print(b);

			b.splice(b.begin(), b, --b.end(), b.end());

			TEST_ENSURE_OP(b.size(), ==, 14);
			TEST_ENSURE(b.front() == 5);
			TEST_ENSURE(b.back() == -4);

			print(b);

			b.splice(b.begin(), b, b.begin(), b.end());

			TEST_ENSURE_OP(b.size(), ==, 14);
			TEST_ENSURE(b.front() == 5);
			TEST_ENSURE(b.back() == -4);

			print(b);

			b.splice(b.begin(), b, b.begin(), b.begin());

			TEST_ENSURE_OP(b.size(), ==, 14);
			TEST_ENSURE(b.front() == 5);
			TEST_ENSURE(b.back() == -4);

			print(b);

			c.splice(c.begin(), b, b.begin(), b.end());

			checkIterators(c, b);

			TEST_ENSURE_OP(b.size(), ==, 0);
			TEST_ENSURE_OP(c.size(), ==, 14);
			TEST_ENSURE(c.front() == 5);
			TEST_ENSURE(c.back() == -4);

			print(c);

			b.splice(b.begin(), c, --c.end(), c.end());

			checkIterators(c, b);

			TEST_ENSURE_OP(b.size(), ==, 1);
			TEST_ENSURE(b.front() == -4);
			TEST_ENSURE(b.back() == -4);
			TEST_ENSURE_OP(c.size(), ==, 13);
			TEST_ENSURE(c.front() == 5);
			TEST_ENSURE(c.back() == 5);

			print(b);
			print(c);

			b.splice(b.begin(), c, c.begin(), ++c.begin());

			TEST_ENSURE_OP(b.size(), ==, 2);
			TEST_ENSURE_OP(c.size(), ==, 12);

			print(b);

			FastList<A, PoolAllocator>::iterator iter(--c.end());
			b.splice(b.end(), c, c.begin(), c.end());
			*iter = -18;
			TEST_ENSURE_OP(b.size(), ==, 14);
			TEST_ENSURE(b.back() == -18);

			print(b);
		}

		void testUnique()
		{
			const A values[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};
			const int valueCount = sizeof(values) / sizeof(A);

			Container a(&values[0], &values[0] + valueCount);

			print(a);

			a.unique();

			print(a);
		}

		void testRemoveIf()
		{
			const A values[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};
			const int valueCount = sizeof(values) / sizeof(A);

			Container a(&values[0], &values[0] + valueCount);

			print(a);

			a.remove_if(std::bind2nd(std::less<A>(), 4));

			print(a);
		}

		void testResize()
		{
			Container a;
			a.resize(10, 4);

			print(a);

			a.resize(0);

			print(a);

			a.resize(4, 3);

			print(a);

			a.resize(4, 4);

			print(a);

			a.resize(5, 5);

			print(a);
		}

		void testRemove()
		{
			const A values[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};
			const int valueCount = sizeof(values) / sizeof(A);

			Container a(&values[0], &values[0] + valueCount);

			print(a);

			a.remove(4);

			print(a);
		}

		void testEqual()
		{
			const A values[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};
			const int valueCount = sizeof(values) / sizeof(A);

			Container a;

			TEST_ENSURE(a == a);

			a.assign(&values[0], &values[0] + valueCount);

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
