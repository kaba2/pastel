// Description: Testing template
// Documentation: unit_testing.txt

#include "test/test_init.h"
#include "range/v3/all.hpp"

#include <iostream>

using namespace ranges;

template <typename Range>
void f(const Range& aSet)
{
	ranges::for_each(aSet, 
		[](auto x)
		{
			std::cout << x.first << x.second << " ";
		}
	);

	for (auto&& x : aSet)
	{
	}

	std::cout << aSet.size() << std::endl;
}

TEST_CASE("Ranges (Ranges)")
{
	std::vector<int> aSet = {4, 3, 2, 1};
	auto bSet = ranges::view::iota((integer)0, (integer)Infinity());
	//ranges::sort(aSet);
	f(ranges::view::zip(ranges::view::const_(aSet), bSet));
}
