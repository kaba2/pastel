// Description: Testing for dividing infinity.
// DocumentationOf: divide_infinity.h

#include "test/test_init.h"

#include <pastel/sys/math/divide_infinity.h>

TEST_CASE("uint8 (divideInfinity)")
{
	auto test = [&](uint8 n, uint8 correct)
	{
		return divideInfinity<uint8>(n) == correct;
	};

	REQUIRE(test(2, 128));
	REQUIRE(test(3, 85));
	REQUIRE(test(4, 64));
	REQUIRE(test(5, 51));
	REQUIRE(test(6, 42));
	REQUIRE(test(7, 36));
	REQUIRE(test(8, 32));
	REQUIRE(test(9, 28));
	REQUIRE(test(10, 25));
	REQUIRE(test(11, 23));
	REQUIRE(test(12, 21));
	REQUIRE(test(13, 19));
	REQUIRE(test(14, 18));
	REQUIRE(test(15, 17));
	REQUIRE(test(16, 16));
	REQUIRE(test(17, 15));
	REQUIRE(test(18, 14));
	REQUIRE(test(19, 13));
	REQUIRE(test(20, 12));
	REQUIRE(test(21, 12));
	REQUIRE(test(22, 11));
	REQUIRE(test(23, 11));
	REQUIRE(test(24, 10));
	REQUIRE(test(25, 10));
	REQUIRE(test(26, 9));
	REQUIRE(test(27, 9));
	REQUIRE(test(28, 9));
	REQUIRE(test(29, 8));
	REQUIRE(test(30, 8));
	REQUIRE(test(31, 8));
	REQUIRE(test(32, 8));

	REQUIRE(test(64, 4));
	REQUIRE(test(128, 2));
	REQUIRE(test(255, 1));
}

TEST_CASE("uint16 (divideInfinity)")
{
	auto test = [&](uint16 n, uint16 correct)
	{
		return divideInfinity<uint16>(n) == correct;
	};

	REQUIRE(test(2, 32768));
	REQUIRE(test(3, 21845));
	REQUIRE(test(4, 16384));
	REQUIRE(test(5, 13107));
	REQUIRE(test(6, 10922));
	REQUIRE(test(7, 9362));
	REQUIRE(test(8, 8192));
	REQUIRE(test(9, 7281));
	REQUIRE(test(10, 6553));
	REQUIRE(test(11, 5957));
	REQUIRE(test(12, 5461));
	REQUIRE(test(13, 5041));
	REQUIRE(test(14, 4681));
	REQUIRE(test(15, 4369));
	REQUIRE(test(16, 4096));
	REQUIRE(test(17, 3855));
	REQUIRE(test(18, 3640));
	REQUIRE(test(19, 3449));
	REQUIRE(test(20, 3276));
	REQUIRE(test(21, 3120));
	REQUIRE(test(22, 2978));
	REQUIRE(test(23, 2849));
	REQUIRE(test(24, 2730));
	REQUIRE(test(25, 2621));
	REQUIRE(test(26, 2520));
	REQUIRE(test(27, 2427));
	REQUIRE(test(28, 2340));
	REQUIRE(test(29, 2259));
	REQUIRE(test(30, 2184));
	REQUIRE(test(31, 2114));
	REQUIRE(test(32, 2048));

	REQUIRE(test(16384, 4));
	REQUIRE(test(32768, 2));
	REQUIRE(test(65535, 1));
}

#include "pastel/sys/integer/multi_integer.h"

TEST_CASE("Unsigned_Integer<1> (divideInfinity)")
{
	auto test = [&](integer n)
	{
		return divideInfinity<Unsigned_Integer<1, uint8>>(n);
	};

	CHECK(test(2) == 1);
	CHECK(test(3) == 0);
	CHECK(test(4) == 0);
}

TEST_CASE("Signed_Integer<1> (divideInfinity)")
{
	auto test = [&](integer n)
	{
		return divideInfinity<Signed_Integer<1, uint8>>(n);
	};

	CHECK(test(2) == 0);
	CHECK(test(3) == 0);
	CHECK(test(4) == 0);
}

TEST_CASE("Unsigned_Integer<2> (divideInfinity)")
{
	auto test = [&](integer n)
	{
		return divideInfinity<Unsigned_Integer<2, uint8>>(n);
	};

	CHECK(test(2) == 2);
	CHECK(test(3) == 1);
	CHECK(test(4) == 1);
	CHECK(test(5) == 0);
	CHECK(test(6) == 0);
}

TEST_CASE("Signed_Integer<2> (divideInfinity)")
{
	auto test = [&](integer n)
	{
		return divideInfinity<Signed_Integer<2, uint8>>(n);
	};

	divideInfinity<Signed_Integer<2, uint8>>(3);

	CHECK(test(2) == 1);
	CHECK(test(3) == 0);
	CHECK(test(4) == 0);
	CHECK(test(5) == 0);
	CHECK(test(6) == 0);
}

