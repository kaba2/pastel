// Description: Testing template
// Documentation: unit_testing.txt

#include "test/test_init.h"
#include "range/v3/all.hpp"
#include "pastel/sys/set/range_concept.h"

#include <iostream>
#include <list>

using namespace ranges;

template <ranges::input_range Range>
void f(const Range& aSet)
{
	ranges::for_each(aSet, 
		[](auto x)
		{
		}
	);

	for (auto&& x : aSet)
	{
	}
}

TEST_CASE("Ranges (Ranges)")
{
	std::vector<int> aSet = {4, 3, 2, 1};
	auto bSet = ranges::views::iota((integer)0, (integer)Infinity());
	//ranges::sort(aSet);
	f(ranges::views::zip(ranges::views::const_(aSet), addConst(bSet)));

	PASTEL_CONCEPT_CHECK(decltype(bSet), Range_Concept);
	PASTEL_CONCEPT_CHECK(std::initializer_list<int>, Range_Concept);
	PASTEL_CONCEPT_CHECK(ranges::iota_view<int>, Range_Concept);

	for (auto&& point : aSet)
	{
	}

	REQUIRE(ranges::size(aSet) == 4);

	std::list<int> cSet = {4, 3, 2, 1};
	REQUIRE(ranges::size(cSet) == 4);
}
