// Description: Testing for Gf2
// DocumentationOf: gf2.h

#include "test/test_init.h"

#include "pastel/sys/gf2.h"

TEST_CASE("Gf2 (Gf2)")
{
	{
		using F = Gf2<1>;

		REQUIRE(F(0) + F(0) == F(0));
		REQUIRE(F(0) + F(1) == F(1));
		REQUIRE(F(1) + F(0) == F(1));
		REQUIRE(F(1) + F(1) == F(0));

		REQUIRE(F(0) * F(0) == F(0));
		REQUIRE(F(0) * F(1) == F(0));
		REQUIRE(F(1) * F(0) == F(0));
		REQUIRE(F(1) * F(1) == F(1));

		REQUIRE((F(0) << 1) == F(0));
		REQUIRE((F(1) << 1) == F(0));

		REQUIRE((F(0) << 2) == F(0));
		REQUIRE((F(1) << 2) == F(0));
	}

	{
		using F = Gf2<2>;

		REQUIRE(F(0) + F(0) == F(0));
		REQUIRE(F(0) + F(1) == F(1));
		REQUIRE(F(0) + F(2) == F(2));
		REQUIRE(F(0) + F(3) == F(3));
		REQUIRE(F(1) + F(0) == F(1));
		REQUIRE(F(1) + F(1) == F(0));
		REQUIRE(F(1) + F(2) == F(3));
		REQUIRE(F(1) + F(3) == F(2));
		REQUIRE(F(2) + F(0) == F(2));
		REQUIRE(F(2) + F(1) == F(3));
		REQUIRE(F(2) + F(2) == F(0));
		REQUIRE(F(2) + F(3) == F(1));
		REQUIRE(F(3) + F(0) == F(3));
		REQUIRE(F(3) + F(1) == F(2));
		REQUIRE(F(3) + F(2) == F(1));
		REQUIRE(F(3) + F(3) == F(0));

		REQUIRE(F(0) * F(0) == F(0));
		REQUIRE(F(0) * F(1) == F(0));
		REQUIRE(F(0) * F(2) == F(0));
		REQUIRE(F(0) * F(3) == F(0));
		REQUIRE(F(1) * F(0) == F(0));
		REQUIRE(F(1) * F(1) == F(1));
		REQUIRE(F(1) * F(2) == F(2));
		REQUIRE(F(1) * F(3) == F(3));
		REQUIRE(F(2) * F(0) == F(0));
		REQUIRE(F(2) * F(1) == F(2));
		REQUIRE(F(2) * F(2) == F(3));
		REQUIRE(F(2) * F(3) == F(1));
		REQUIRE(F(3) * F(0) == F(0));
		REQUIRE(F(3) * F(1) == F(3));
		REQUIRE(F(3) * F(2) == F(1));
		REQUIRE(F(3) * F(3) == F(2));

		REQUIRE((F(0) << 1) == F(0) * F(2));
		REQUIRE((F(1) << 1) == F(1) * F(2));
		REQUIRE((F(2) << 1) == F(2) * F(2));
		REQUIRE((F(3) << 1) == F(3) * F(2));

		REQUIRE((F(0) << 2) == F(0) * F(2) * F(2));
		REQUIRE((F(1) << 2) == F(1) * F(2) * F(2));
		REQUIRE((F(2) << 2) == F(2) * F(2) * F(2));
		REQUIRE((F(3) << 2) == F(3) * F(2) * F(2));
	}
}

