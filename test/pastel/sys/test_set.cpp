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
	//PASTEL_CONCEPT_CHECK(
	//	Interval_Set<integer>, Set_Concept(integer));
	//PASTEL_CONCEPT_CHECK(
	//	Interval_Set<integer>, Set_Concept(real));
	PASTEL_STATIC_ASSERT(
		CorrectElement<Interval_Set<integer>, integer>::value);
	{
		integer n = 8;
		auto set = intervalSet((integer)3, (integer)3 + n);
		auto index = set.begin();
		++index;
		REQUIRE(ranges::distance(set) == n);

		PASTEL_STATIC_ASSERT(
			CorrectElement<decltype(set), integer>::value);

		std::unordered_set<integer> actualSet;

		for(auto&& a : set)
		{
			actualSet.insert(a);
		}
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
	integer i = 3;
	for (auto&& element : set)
	{
		REQUIRE(element == i);
		++i;
	}
}

TEST_CASE("Range (Set)")
{
	{
		std::list<integer> set = {3, 4, 5, 6, 7, 8, 9, 10, 11};
		integer n = set.size();
		REQUIRE(ranges::size(set) == n);

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
	{
		integer n = 4;
		integer element = 7;
		auto set = constantSet(n, element);

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
		auto set = constantSet(n, element);

		bool correct = forEach(set, [&](integer a)
		{
			return false;
		});

		REQUIRE(correct);
	}
}

TEST_CASE("Union (Set)")
{
	{
		integer nA = 5;
		integer aElement = 7;
		auto aSet = constantSet(nA, aElement);
		PASTEL_CONCEPT_CHECK(decltype(aSet), Set_Concept);
		REQUIRE(ranges::size(aSet) == 5);

		integer nB = 3;
		integer bElement = 3;
		auto bSet = constantSet(nB, bElement);
		PASTEL_CONCEPT_CHECK(decltype(bSet), Set_Concept);
		REQUIRE(ranges::size(bSet) == 3);

		auto abSet = unionSet(aSet, bSet);
		PASTEL_CONCEPT_CHECK(decltype(abSet), Set_Concept);

		REQUIRE(ranges::size(abSet) == 5 + 3);
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
	for(auto&& element : subSet)
	{
		actualSet.insert(element);
	}

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
