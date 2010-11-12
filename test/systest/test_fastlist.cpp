// Description: Testing for FastList
// DocumentationOf: fastlist.h

#include "pastelsystest.h"

#include "pastel/sys/fastlist.h"
#include "pastel/sys/log.h"
#include "pastel/sys/poolallocator.h"

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

	template class FastList<int, PoolAllocator>;
	template class FastList<A, PoolAllocator>;

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

			REPORT1(a.size() != 0, a.size());
			REPORT(!a.empty());
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

		REPORT1(b.size() != 14, b.size());
		REPORT(b.front() != 5);
		REPORT(b.back() != -4);

		print(b);

		b.splice(b.begin(), b, b.begin(), b.end());

		REPORT1(b.size() != 14, b.size());
		REPORT(b.front() != 5);
		REPORT(b.back() != -4);

		print(b);

		b.splice(b.begin(), b, b.begin(), b.begin());

		REPORT1(b.size() != 14, b.size());
		REPORT(b.front() != 5);
		REPORT(b.back() != -4);

		print(b);

		c.splice(c.begin(), b, b.begin(), b.end());

		checkIterators(c, b);

		REPORT1(b.size() != 0, b.size());
		REPORT1(c.size() != 14, c.size());
		REPORT(c.front() != 5);
		REPORT(c.back() != -4);

		print(c);

		b.splice(b.begin(), c, --c.end(), c.end());

		checkIterators(c, b);

		REPORT1(b.size() != 1, b.size());
		REPORT(b.front() != -4);
		REPORT(b.back() != -4);
		REPORT1(c.size() != 13, c.size());
		REPORT(c.front() != 5);
		REPORT(c.back() != 5);

		print(b);
		print(c);

		b.splice(b.begin(), c, c.begin(), ++c.begin());

		REPORT1(b.size() != 2, b.size());
		REPORT1(c.size() != 12, c.size());

		print(b);

		FastList<A, PoolAllocator>::iterator iter(--c.end());
		b.splice(b.end(), c, c.begin(), c.end());
		*iter = -18;
		REPORT1(b.size() != 14, b.size());
		REPORT(b.back() != -18);

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

		REPORT(a != a);

		a.assign(&values[0], &values[0] + valueCount);

		REPORT(a != a);

		Container b(a);
		REPORT(b != a);

		b.pop_back();
		REPORT(b == a);

		a.pop_back();
		REPORT(b != a);

		a.front() = -5;
		REPORT(b == a);

		a.swap(b);
		REPORT(b == a);

		a = b;
		REPORT(b != a);

		a.swap(b);
		REPORT(b != a);
	}

	void testBegin()
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

	void testAdd()
	{
		sysTestList().add("FastList", testBegin);
	}

	CallFunction run(testAdd);

}
