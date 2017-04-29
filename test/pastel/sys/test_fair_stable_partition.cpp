// Description: Testing for fair stable partitioning
// DocumentationOf: fair_stable_partition.h

#include "test/test_init.h"

#include <pastel/sys/sequence/fair_stable_partition.h>
#include <pastel/sys/trindicator.h>
#include <pastel/sys/indicator.h>

using vec = std::vector<int>;

TEST_CASE("fair_stable_partition (fair_stable_partition)")
{
	auto test = [](
		auto elementSet,
		const auto& trindicator,
		const auto& leftSet, 
		const auto& rightSet)
	{
		auto rightBegin = 
			fairStablePartition(
				range(elementSet.begin(), elementSet.end()), 
				trindicator);
		
		if (!boost::equal(leftSet, 
			range(elementSet.begin(), rightBegin)))
		{
			return false;
		}

		if (!boost::equal(rightSet, 
			range(rightBegin, elementSet.end())))
		{
			return false;
		}
		
		return true;
	};

	auto lessFive = [](auto element) {return element - 5;};

	REQUIRE(test(
		vec({1, 6, 3, 4, 6, 8, 9, 0, 4}),
		[](auto element) {return 0;},
		vec({1, 3, 6, 9, 4}),
		vec({6, 4, 8, 0})
	));

	REQUIRE(test(
		vec({1, 6, 3, 4, 6, 8, 9, 0, 4}),
		lessFive,
		vec({1, 3, 4, 0, 4}),
		vec({6, 6, 8, 9})
	));

	REQUIRE(test(
		vec({1, 5}),
		lessFive,
		vec({1}),
		vec({5})
	));

	REQUIRE(test(
		vec({1, 2, 5}),
		lessFive,
		vec({1, 2}),
		vec({5})
	));

	REQUIRE(test(
		vec({5, 6}),
		lessFive,
		vec({5}),
		vec({6})
	));

	REQUIRE(test(
		vec({5, 6, 7}),
		lessFive,
		vec({5}),
		vec({6, 7})
	));

	REQUIRE(test(
		vec({5, 5}),
		lessFive,
		vec({5}),
		vec({5})
	));

	REQUIRE(test(
		vec({5, 5, 5}),
		lessFive,
		vec({5, 5}),
		vec({5})
	));

	REQUIRE(test(
		vec({5, 5, 5, 5}),
		lessFive,
		vec({5, 5}),
		vec({5, 5})
	));
}

