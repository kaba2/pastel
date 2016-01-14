// Description: Testing for sets
// Documentation: unit_testing.txt

#include "test/test_init.h"

#include "pastel/sys/set.h"
#include "pastel/sys/function.h"

#include <unordered_set>
#include <list>

namespace
{

	template <typename Set, typename Element>
	using CorrectElement = 
		std::is_same<
			Set_Element<Set>,
			Element
		>;

}

TEST_CASE("Interval (Set)")
{
	PASTEL_CONCEPT_CHECK(
		Interval_Set<integer>, Set_Concept);
	PASTEL_CONCEPT_CHECK(
		Interval_Set<integer>, Set_Concept());
	PASTEL_CONCEPT_CHECK(
		Interval_Set<integer>, Set_Concept(integer));
	PASTEL_CONCEPT_CHECK(
		Interval_Set<integer>, Set_Concept(real));
	PASTEL_STATIC_ASSERT(
		CorrectElement<Interval_Set<integer>, integer>::value);
	{
		integer n = 8;
		auto set = intervalSet((integer)3, (integer)3 + n);
		auto index = set.index();
		set.next(index);
		REQUIRE(set.n() == n);

		PASTEL_STATIC_ASSERT(
			CorrectElement<decltype(set), integer>::value);

		std::unordered_set<integer> actualSet;

		forEach(set, [&](integer a)
		{
			actualSet.insert(a);
			return true;
		});
		REQUIRE(actualSet.size() == n);

		for (integer i = 0; i < n; ++i)
		{
			REQUIRE(actualSet.count(3 + i) == 1);
		}
	}
}

TEST_CASE("Basic interval (Set)")
{
	integer n = 8;
	auto set = intervalSet((integer)3, (integer)3 + n);

	auto index = set.index();
	integer i = 3;
	while (!set.empty(index))
	{
		REQUIRE(set[index] == i);
		set.next(index);
		++i;
	}
}

TEST_CASE("Range (Set)")
{
	{
		std::list<integer> aSet = {3, 4, 5, 6, 7, 8, 9, 10, 11};
		integer n = aSet.size();
		auto set = rangeSet(aSet.begin(), aSet.end());

		auto index = set.index();
		set.next(index);
		REQUIRE(set.n() == n);

		PASTEL_STATIC_ASSERT(
			CorrectElement<decltype(set), integer>::value);

		std::unordered_set<integer> actualSet;

		forEach(set, [&](integer a)
		{
			actualSet.insert(a);
			return true;
		});
		REQUIRE(actualSet.size() == n);

		for (integer i = 0;i < n;++i)
		{
			REQUIRE(actualSet.count(3 + i) == 1);
		}
	}
}

TEST_CASE("Constant (Set)")
{
	PASTEL_CONCEPT_CHECK(
		Constant_Set<integer>, Set_Concept);
	PASTEL_CONCEPT_CHECK(
		Constant_Set<integer>, Set_Concept());
	PASTEL_CONCEPT_CHECK(
		Constant_Set<integer>, Set_Concept(integer));
	PASTEL_CONCEPT_CHECK(
		Constant_Set<integer>, Set_Concept(real));

	{
		integer n = 4;
		integer element = 7;
		Constant_Set<integer> set(n, element);

		PASTEL_STATIC_ASSERT(
			CorrectElement<decltype(set), integer>::value);

		integer m = 0;
		bool correct = forEach(set, [&](integer a)
		{
			if (a != element)
			{
				return false;
			}
			++m;

			return true;
		});

		REQUIRE(correct);
		REQUIRE(m == n);
	}
	{
		integer n = 0;
		integer element = 7;
		Constant_Set<integer> set(n, element);

		bool correct = forEach(set, [&](integer a)
		{
			return false;
		});

		REQUIRE(correct);
	}
}

TEST_CASE("Union (Set)")
{
	using Union = Union_Set<
			Constant_Set<integer>,
			Constant_Set<integer>>;
	PASTEL_CONCEPT_CHECK(Union, Set_Concept);
	PASTEL_CONCEPT_CHECK(Union, Set_Concept());
	PASTEL_CONCEPT_CHECK(Union, Set_Concept(integer));
	PASTEL_CONCEPT_CHECK(Union, Set_Concept(real));

	{
		integer nA = 5;
		integer aElement = 7;
		Constant_Set<integer> aSet(nA, aElement);

		integer nB = 3;
		integer bElement = 3;
		Constant_Set<integer> bSet(nB, bElement);

		auto abSet = unionSet(aSet, bSet);

		REQUIRE(abSet.n() == 5 + 3);
		PASTEL_STATIC_ASSERT(
			std::is_same<
				Set_Element<decltype(abSet)>,
				integer
			>::value);

		std::unordered_multiset<integer> actualSet;
		forEach(abSet, [&](integer a)
		{
			actualSet.insert(a);
			return true;
		});

		REQUIRE(actualSet.count(aElement) == 5);
		REQUIRE(actualSet.count(bElement) == 3);
	}
}

TEST_CASE("Sparse (Set)")
{
	integer n = 10;

 	auto inputSet = intervalSet((integer)0, n);

	auto subSet = sparseSet(
		inputSet, 2);

	std::unordered_set<integer> actualSet;
	forEach(subSet, [&](integer x)
	{
		actualSet.insert(x);
		return true;
	});

	REQUIRE(actualSet.size() == n / 2);
	for (integer i = 0;i < n;i += 2)
	{
		REQUIRE(actualSet.count(i) == 1);
	}
}

TEST_CASE("Transformed (Set)")
{
	std::vector<integer> dataSet = {0, 1, 2, 3, 4};
	integer n = dataSet.size();

	auto inputSet = intervalSet(
		dataSet.begin(), dataSet.end());

	auto dereferencedSet = transformedSet(
		inputSet, Dereference_Function());

	std::unordered_set<integer> actualSet;
	forEach(dereferencedSet, [&](integer x)
	{
		actualSet.insert(x);
		return true;
	});

	REQUIRE(actualSet.size() == n);
	for (integer i = 0;i < n;++i)
	{
		REQUIRE(actualSet.count(i) == 1);
	}
}
