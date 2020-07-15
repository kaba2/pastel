// Description: Testing for List
// DocumentationOf: list.h

#include "test/test_init.h"

#include "pastel/sys/list.h"
#include "pastel/sys/range.h"
#include "pastel/sys/indicator/all_indicator.h"

#include <iostream>

namespace
{

	using Set = List_Set<integer, integer>;
	using Iterator = Set::Iterator;
	using ConstIterator = Set::ConstIterator;

	template <typename Type>
	bool equal(const Set& left, 
		std::initializer_list<Type> right)
	{
		return ranges::equal(left, right);
	}

}

TEST_CASE("Iterators (List)")
{
	Set a({1, 2, 3});
	{
		REQUIRE(*a.begin() == 1);
		REQUIRE(*a.last() == 3);
		REQUIRE(a.last() == std::prev(a.end()));

		REQUIRE(ranges::equal(a.crange(), std::initializer_list<int>({1, 2, 3})));

		Iterator i = a.begin();
		ConstIterator j = i;
		REQUIRE(i == j);
		REQUIRE(i <= j);
		REQUIRE(i >= j);
		i < j;
		i > j;

		REQUIRE(j == i);
		REQUIRE(j <= i);
		REQUIRE(j >= i);
		j < i;
		j > i;
	}
	{
		REQUIRE(a.cend().isEnd());
	}
	{
		a.end().endData() = 4;
		REQUIRE(a.end().endData() == 4);
	}
	{
		Iterator iter;
		ENSURE(iter.empty());

		iter = a.end();
		ENSURE(iter.isEnd());

		iter.clear();
		ENSURE(iter.empty());
	}
	{
		*a.begin() = 4;
		REQUIRE(*a.begin() == 4);

		Iterator j = a.insertFront(*a.begin());
		REQUIRE(*j == 4);
	}
	{
		List_Set<integer> a;
		List_Set<integer, integer> b;
		List_Set<Empty, integer> c;
		auto i = a.begin();
		unused(i);
		// The following should not compile.
		//i = a.cbegin();
		//i = b.begin();
		//i = c.begin();
	}
}

TEST_CASE("Swap (List)")
{
	{
		Set a({1, 2, 3, 4});
		REQUIRE(testInvariants(a));

		Set b;
		REQUIRE(testInvariants(b));

		a.swap(b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(b, { 1, 2, 3, 4 }));
		REQUIRE(a.empty());

		swap(a, b);
	}
}

TEST_CASE("Clear (List)")
{
	{
		Set a({1, 2, 3, 4});
		REQUIRE(testInvariants(a));
		Iterator aEnd = a.end();

		a.clear();
		REQUIRE(testInvariants(a));
		REQUIRE(a.end() == aEnd);
	}
}

TEST_CASE("CopyAndMove (List)")
{
	{
		Set a({1, 2, 3, 4});
		REQUIRE(testInvariants(a));

		Set b(a);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(ranges::equal(a, b));

		Set c;
		REQUIRE(testInvariants(c));
		Iterator cEnd = c.end();

		c = b;
		REQUIRE(testInvariants(b));
		REQUIRE(testInvariants(c));
		REQUIRE(ranges::equal(b, c));
		REQUIRE(c.end() == cEnd);

		Set d = std::move(c);
		REQUIRE(testInvariants(c));
		REQUIRE(testInvariants(d));
		REQUIRE(ranges::equal(b, d));
		REQUIRE(c.empty());
		REQUIRE(c.end() == cEnd);

		c = std::move(d);
		REQUIRE(testInvariants(c));
		REQUIRE(testInvariants(d));
		REQUIRE(ranges::equal(b, c));
		REQUIRE(d.empty());
		REQUIRE(c.end() == cEnd);

		Set e;
		REQUIRE(testInvariants(e));

		e = { 1, 2, 3, 4 };
		REQUIRE(testInvariants(e));
		REQUIRE(equal(e, { 1, 2, 3, 4 }));
	}
}

TEST_CASE("Comparison (List)")
{
	{
		Set a;
		REQUIRE(testInvariants(a));
		REQUIRE(a == a);
		REQUIRE(!(a != a));
	}
	{
		Set a({1, 6, 3, 4, 5});
		REQUIRE(testInvariants(a));

		Set b({1, 6, 3, 4, 5});
		REQUIRE(testInvariants(b));

		REQUIRE(a == b);
		REQUIRE(!(a != b));
		REQUIRE(!(a < b));
		REQUIRE(!(a > b));
		REQUIRE(a <= b);
		REQUIRE(a >= b);
	}
	{
		Set a({1, 5, 3, 4, 5});
		REQUIRE(testInvariants(a));

		Set b({1, 5, 3, 6, 7});
		REQUIRE(testInvariants(b));

		REQUIRE(a != b);
		REQUIRE(a <= b);
		REQUIRE(a < b);
		REQUIRE(!(a >= b));
		REQUIRE(!(a > b));
	}
	{
		Set a({1, 6, 3, 4});
		REQUIRE(testInvariants(a));

		Set b({1, 6, 3, 4, 5});
		REQUIRE(testInvariants(b));

		REQUIRE(a != b);
		REQUIRE(!(a == b));
		REQUIRE(a < b);
		REQUIRE(!(a > b));
		REQUIRE(!(a >= b));
		REQUIRE(a <= b);
	}
}

TEST_CASE("InsertAndErase (List)")
{
	Set a;
	Iterator four;

	{
		Iterator iter = a.insertBack(1);
		REQUIRE(*iter == 1);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1 }));
	}

	{
		four = a.insertBack(4);
		REQUIRE(*four == 4);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1, 4 }));
	}

	{
		Iterator iter = a.insertBack(8);
		REQUIRE(*iter == 8);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1, 4, 8 }));
	}

	{
		Iterator iter = a.insertFront(7);
		REQUIRE(*iter == 7);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {7, 1, 4, 8 }));
		REQUIRE(a.size() == 4);
	}

	{
		a.insert(four, 9);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {7, 1, 9, 4, 8 }));
	}

	{
		a.insert(four, 10);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {7, 1, 9, 10, 4, 8 }));
	}

	{
		a.insert(a.end(), 11);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {7, 1, 9, 10, 4, 8, 11 }));
	}

	{
		a.eraseFront();
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {1, 9, 10, 4, 8, 11 }));
	}
	{
		a.eraseBack();
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {1, 9, 10, 4, 8 }));
	}
	{
		a.erase(four);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {1, 9, 10, 8 }));
	}
	{
		a.eraseFront();
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {9, 10, 8 }));
	}
	{
		a.eraseFront();
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {10, 8 }));
	}
	{
		a.eraseBack();
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {10}));
	}
	{
		a.eraseBack();
		REQUIRE(testInvariants(a));
	}
}

TEST_CASE("Sort (List)")
{
	{
		Set a;
		REQUIRE(testInvariants(a));

		sort(a);
		REQUIRE(testInvariants(a));
	}
	{
		Set a({0});
		REQUIRE(testInvariants(a));

		sort(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 0 }));
	}
	{
		Set a({0, 1, 2, 3});
		REQUIRE(testInvariants(a));

		sort(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 0, 1, 2, 3 }));
	}
	{
		Set a({3, 2, 1, 0});
		REQUIRE(testInvariants(a));

		sort(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 0, 1, 2, 3 }));
	}
	{
		Set a({1, 5, 3, 2, 4, 0});
		REQUIRE(testInvariants(a));

		sort(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 0, 1, 2, 3, 4, 5 }));
	}
}

TEST_CASE("Unique (List)")
{
	{
		Set a;
		REQUIRE(testInvariants(a));

		unique(a);
		REQUIRE(testInvariants(a));
	}

	{
		Set a({1});
		REQUIRE(testInvariants(a));

		unique(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1 }));
	}

	{
		Set a({1, 1, 1});
		REQUIRE(testInvariants(a));

		unique(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1 }));
	}

	{
		Set a({1, 1, 2, 2, 3, 4, 4, 5});
		REQUIRE(testInvariants(a));

		unique(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1, 2, 3, 4, 5 }));
	}

	{
		Set a({1, 1, 5, 5, 4, 4, 3, 3, 3, 3});
		REQUIRE(testInvariants(a));

		unique(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1, 5, 4, 3 }));
	}
}

TEST_CASE("RemoveIf (List)")
{
	using namespace std::placeholders;

	{
		Set a;
		REQUIRE(testInvariants(a));

		removeIf(a, allIndicator());
		REQUIRE(testInvariants(a));
	}
	{
		Set a({1, 2, 5, 3, 4, 5, 6, 5});
		REQUIRE(testInvariants(a));

		removeIf(a, std::bind(EqualTo(), _1, 5));
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {1, 2, 3, 4, 6}));
	}	

	{
		Set a({1, 2, 5, 3, 4, 5, 6, 5});
		REQUIRE(testInvariants(a));

		removeIf(a, std::bind(LessThan(), _1, 4));
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {5, 4, 5, 6, 5}));
	}	

	{
		Set a({1, 2, 5, 3, 4, 5, 6, 5});
		REQUIRE(testInvariants(a));

		removeIf(a, std::bind(GreaterThan(), _1, 6));
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, {1, 2, 5, 3, 4, 5, 6, 5}));
	}	
}

TEST_CASE("Merge (List)")
{
	{
		Set a;
		REQUIRE(testInvariants(a));

		Set b;
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
	}
	{
		Set a({ 1 });
		REQUIRE(testInvariants(a));

		Set b;
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(a, { 1 }));
	}		
	{
		Set a({ 1 });
		REQUIRE(testInvariants(a));

		Set b({ 2 });
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(a, { 1, 2 }));
	}		
	{
		Set a({ 1, 3 });
		REQUIRE(testInvariants(a));

		Set b({ 2 });
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(a, { 1, 2, 3 }));
	}		
	{
		Set a({ 2 });
		REQUIRE(testInvariants(a));

		Set b({ 1, 3 });
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(a, { 1, 2, 3 }));
	}		
	{
		Set a({ 1, 4, 6, 7, 8, 9 });
		REQUIRE(testInvariants(a));

		Set b({ 1, 1, 3, 5, 6, 8, 10 });
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(a, 
			{ 1, 1, 1, 3, 4, 5, 6, 6, 7, 8, 8, 9, 10 }));
	}		

	{
		Set a({ 1, 2, 3, 4 });
		REQUIRE(testInvariants(a));

		Set b({ 5, 6 });
		REQUIRE(testInvariants(b));

		merge(a, b);
		REQUIRE(testInvariants(a));
		REQUIRE(testInvariants(b));
		REQUIRE(equal(a, { 1, 2, 3, 4, 5, 6 }));
	}		
}

TEST_CASE("Reverse (List)")
{
	{
		Set a;
		REQUIRE(testInvariants(a));

		reverse(a);
		REQUIRE(testInvariants(a));

		reverse(a);
		REQUIRE(testInvariants(a));
	}
	{
		Set a({ 1 });
		REQUIRE(testInvariants(a));

		reverse(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1 }));

		reverse(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1 }));
	}		
	{
		Set a({ 1, 2 });
		REQUIRE(testInvariants(a));

		reverse(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 2, 1 }));

		reverse(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1, 2 }));
	}
	{
		Set a({ 1, 2, 3, 4, 5 });
		REQUIRE(testInvariants(a));

		reverse(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 5, 4, 3, 2, 1 }));

		reverse(a);
		REQUIRE(testInvariants(a));
		REQUIRE(equal(a, { 1, 2, 3, 4, 5 }));
	}
}

TEST_CASE("Splice (List)")
{
	Set c;
	REQUIRE(testInvariants(c));

	Set b = { 6, 2, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, -4, 5};
	REQUIRE(testInvariants(b));

	{
		integer correctSet[] = 
		{
			6, 2, 
			5, 5, 5, 5, 5, 
			5, 5, 5, 5, 5,
			-4, 5
		};
		REQUIRE(b.size() == 14);
		REQUIRE(ranges::equal(b, correctSet));
	}

	b.splice(b.begin(), b, b.last());
	REQUIRE(testInvariants(b));
	{
		integer correctSet[] = 
		{
			5, 
			6, 2, 
			5, 5, 5, 5, 5, 
			5, 5, 5, 5, 5,
			-4
		};
		REQUIRE(b.size() == 14);
		REQUIRE(ranges::equal(b, correctSet));
	}

	b.splice(b.begin(), b);
	REQUIRE(testInvariants(b));
	{
		integer correctSet[] = 
		{
			5, 
			6, 2, 
			5, 5, 5, 5, 5, 
			5, 5, 5, 5, 5,
			-4
		};
		REQUIRE(b.size() == 14);
		REQUIRE(ranges::equal(b, correctSet));
	}

	c.splice(c.begin(), b);
	REQUIRE(testInvariants(b));
	REQUIRE(testInvariants(c));
	{
		integer correctSet[] = 
		{
			5, 
			6, 2, 
			5, 5, 5, 5, 5, 
			5, 5, 5, 5, 5,
			-4
		};
		REQUIRE(c.size() == 14);
		REQUIRE(ranges::equal(c, correctSet));
		REQUIRE(b.empty());
		REQUIRE(b.size() == 0);
	}

	b.splice(b.begin(), c, c.last());
	REQUIRE(testInvariants(b));
	REQUIRE(testInvariants(c));
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

		REQUIRE(b.size() == 1);
		REQUIRE(ranges::equal(b, bCorrectSet));

		REQUIRE(c.size() == 13);
		REQUIRE(ranges::equal(c, cCorrectSet));
	}

	b.splice(b.begin(), c, c.begin());
	REQUIRE(testInvariants(b));
	REQUIRE(testInvariants(c));
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

		REQUIRE(b.size() == 2);
		REQUIRE(ranges::equal(b, bCorrectSet));

		REQUIRE(c.size() == 12);
		REQUIRE(ranges::equal(c, cCorrectSet));
	}

	Iterator iter(--c.end());
	b.splice(b.end(), c);
	*iter = -18;
	REQUIRE(testInvariants(b));
	REQUIRE(testInvariants(c));
	{
		REQUIRE(b.size() == 14);
		REQUIRE(b.back() == -18);
	}
}

TEST_CASE("Equal (List)")
{
	integer valueSet[] = {1, 2, 3, 3, 3, 4, 4, 4, 5, 4, 4, 4};

	Set a;

	REQUIRE(a == a);

	for (integer value : valueSet)
	{
		a.insertBack(value);
	}

	REQUIRE(a == a);

	Set b(a);
	REQUIRE(b == a);

	b.eraseBack();
	REQUIRE(b != a);

	a.eraseBack();
	REQUIRE(b == a);

	a.front() = -5;
	REQUIRE(b != a);

	a.swap(b);
	REQUIRE(b != a);

	a = b;
	REQUIRE(b == a);

	a.swap(b);
	REQUIRE(b == a);
}
