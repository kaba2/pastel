// Description: Testing for fair stable partitioning
// DocumentationOf: fair_stable_partition.h

#include "test/test_init.h"

#include <pastel/sys/sequence/fair_stable_partition.h>
#include <pastel/sys/trindicator.h>
#include <pastel/sys/indicator.h>

TEST_CASE("fair_stable_partition (fair_stable_partition)")
{
	using V = std::vector<std::pair<integer, integer>>;

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

	auto lessFive = [](auto element) {return element.first - 5;};
	
	REQUIRE(test(
		V({{1, 0}, {6, 1}, {3, 2}, {4, 3}, {6, 4}, {8, 5}, {9, 6}, {0, 7}, {4, 8}}),
		[](auto element) {return 0;},
		V({{1, 0}, {6, 1}, {3, 2}, {4, 3}, {6, 4}}),
		V({{8, 5}, {9, 6}, {0, 7}, {4, 8}})
	));

	REQUIRE(test(
		V({{1, 0}, {6, 1}, {3, 2}, {4, 3}, {6, 4}, {8, 5}, {9, 6}, {0, 7}, {4, 8}}),
		lessFive,
		V({{1, 0}, {3, 2}, {4, 3}, {0, 7}, {4, 8}}),
		V({{6, 1}, {6, 4}, {8, 5}, {9, 6}})
	));

	REQUIRE(test(
		V({{1, 0}, {5, 1}}),
		lessFive,
		V({{1, 0}}),
		V({{5, 1}})
	));

	REQUIRE(test(
		V({{1, 0}, {2, 1}, {5, 2}}),
		lessFive,
		V({{1, 0}, {2, 1}}),
		V({{5, 2}})
	));

	REQUIRE(test(
		V({{5, 0}, {6, 1}}),
		lessFive,
		V({{5, 0}}),
		V({{6, 1}})
	));

	REQUIRE(test(
		V({{5, 0}, {6, 1}, {7, 2}}),
		lessFive,
		V({{5, 0}}),
		V({{6, 1}, {7, 2}})
	));

	REQUIRE(test(
		V({{5, 0}, {5, 1}}),
		lessFive,
		V({{5, 0}}),
		V({{5, 1}})
	));

	REQUIRE(test(
		V({{5, 0}, {5, 1}, {5, 2}}),
		lessFive,
		V({{5, 0}, {5, 1}}),
		V({{5, 2}})
	));

	REQUIRE(test(
		V({{5, 0}, {5, 1}, {5, 2}, {5, 3}}),
		lessFive,
		V({{5, 0}, {5, 1}}),
		V({{5, 2}, {5, 3}})
	));
}

