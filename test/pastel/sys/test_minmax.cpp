// Description: Testing for minMax
// DocumentationOf: minmax.h

#include "test/test_init.h"

#include "pastel/sys/math/minmax.h"

namespace
{

		class Test
	{
	public:
		virtual void run()
		{
			testMinMax();
		}

		void testMinMax()
		{
			integer min = 0;
			integer max = 0;

			minMax(1, 2, 3, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

			minMax(1.0, 2, 3.0f, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

			minMax(1, 3, 2, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

			minMax(2, 1, 3, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

			minMax(2, 3, 1, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

			minMax(3, 1, 2, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

			minMax(3, 2, 1, min, max);
			REQUIRE(min == 1);
			REQUIRE(max == 3);

		}
	};

	TEST_CASE("MinMax", "[MinMax]")
	{
	}

}
