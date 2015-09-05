// Description: Testing for sign
// DocumentationOf: sign.h

#include "test/test_init.h"

#include "pastel/sys/math/sign.h"

TEST_CASE("sign (sign)")
{
	REQUIRE(sign(-100) == -1);
	REQUIRE(sign(100) == 1);

	REQUIRE(sign(-1) == -1);
	REQUIRE(sign(0) == 0);
	REQUIRE(sign(1) == 1);

	REQUIRE(sign(-1.0) == -1);
	REQUIRE(sign(0.0) == 0);
	REQUIRE(sign(1.0) == 1);

	REQUIRE(sign(-1.0f) == -1);
	REQUIRE(sign(0.0f) == 0);
	REQUIRE(sign(1.0f) == 1);

	REQUIRE(sign((float)Infinity()) == 1);
	REQUIRE(sign(-(float)Infinity()) == -1);
	REQUIRE(sign((float)Nan()) == 0);
	REQUIRE(sign(-0.0f) == 0);
}
