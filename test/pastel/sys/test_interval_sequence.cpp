// Description: Testing for interval sequences
// DocumentationOf: interval_sequence.h

#include "test/test_init.h"

#include "pastel/sys/interval_sequence.h"
#include "pastel/sys/range.h"

TEST_CASE("Simple (interval_sequence)")
{
	integer aSet[] = 
	{
		0, 2,
		2, 2,
		3, 6,
		6, 8,
		10, 11
	};

	integer bSet[] = 
	{
		-1, -1,
		0, 1,
		2, 2,
		4, 5,
		6, 9,
		11, 12
	};

	{
		std::vector<integer> cSet;

		auto report = [&](const std::pair<integer, integer>& that)
		{
			cSet.push_back(that.first);
			cSet.push_back(that.second);
		};
		difference(aSet, bSet, report);

		integer correctSet[] = 
		{
			1, 2,
			3, 4,
			5, 6,
			10, 11
		};

		REQUIRE(ranges::equal(cSet, correctSet));
	}

	std::vector<integer> emptySet;
	{
		std::vector<integer> cSet;
		auto report = [&](const std::pair<integer, integer>& that)
		{
			cSet.push_back(that.first);
			cSet.push_back(that.second);
		};
		difference(emptySet, bSet, report);
		REQUIRE(cSet.empty());
	}
	{
		std::vector<integer> cSet;
		auto report = [&](const std::pair<integer, integer>& that)
		{
			cSet.push_back(that.first);
			cSet.push_back(that.second);
		};
		difference(aSet, emptySet, report);

		integer correctSet[] = 
		{
			0, 2,
			3, 6,
			6, 8,
			10, 11
		};

		REQUIRE(ranges::equal(cSet, correctSet));
	}
}
