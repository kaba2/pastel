// Description: Testing for intervals
// DocumentationOf: interval.h

#include "test/test_init.h"

#include <pastel/sys/interval.h>

TEST_CASE("interval (interval)")
{
	using I = Interval<real>;

	I a(1, 4);
	{
		REQUIRE(a.min() == 1);
		REQUIRE(a.max() == 4);
	}
	{
		a.min() = 3;
		a.max() = 5;

		REQUIRE(a.min() == 3);
		REQUIRE(a.max() == 5);
	}
	{
		a.set(2, 6);
		REQUIRE(a.min() == 2);
		REQUIRE(a.max() == 6);
	}
}

